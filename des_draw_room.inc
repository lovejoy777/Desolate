;------------------------------------
; File: des_draw_room.inc
; Logic: decompress and draw room tiles
;------------------------------------
;--------------------------
; Trigger a Room Reload.
;--------------------------
Trigger_Room_Load:
    CLG                            ; clear graphics in the game screen.
    xor a                          ; clear accumulator
    ld (is_moving), a              ; Force the player to stop moving
    ld (move_timeout), a           ; Reset the timer
    ld (main_menu_is_active), a    ; Set Main Menu Flag to 0

    call Decode_Room_Metadata      ; from des_metadata_logic.inc
    call Check_Alien_Presence      ; Checks room metadata and sets sm_alien_is_active & big_alien_is_active
    
    call Draw_Room                 ; from des_draw_room.inc
    call Draw_Health_Value         ; from main_subs.inc
    call Look_Shoot_Mode           ; from des_ui_logic.inc 

    
    
    
    jp main_loop

;--------------
; Draw Room
;--------------
Draw_Room:
    push ix
    push iy
    ld hl, (current_room_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes
decode_main:
    ld a, (hl)            
    cp $FF              ; repeat flag
    jr nz, literal_byte
    
    inc hl
    ld c, (hl)            
    inc hl
    ld a, (hl)            
repeat_loop:
    ld (de), a            
    inc de
    dec b                 
    jp z, drawing_start   
    dec c                 
    jr nz, repeat_loop
    inc hl
    jr decode_main
literal_byte:
    ld (de), a
    inc de
    inc hl
    djnz decode_main     

drawing_start:
    ld hl, ram_buffer
    ld iy, (game_screen_offset_y)                  
    ld b, 8                    
row_loop:
    push bc
    ld ix, (game_screen_offset_x)                      
    ld b, 12                   
draw_col_loop:
    push bc
    push hl
    ld a, (hl)                 ; Get Tile ID from ram_buffer
    ld hl, 0                   ; 24-bit register for ADL mode
    ld l, a
    ld bc, 100                 ; Add 100 for background tile buffers Tileset1
    add hl, bc  
    ex de, hl                  ; DE = Final VDP Buffer ID

    SELECT_BUFFER_DE
    
    ;DRAW_BITMAP_IX_IY 
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3
    rst.lil $10 ; Draw Bitmap Command
    
    push ix
    pop hl      ; Move IX to HL to send
    ld a, l
    rst.lil $10 ; X Low
    ld a, h
    rst.lil $10 ; X High
    
    push iy
    pop hl      ; Move IY to HL to send
    ld a, l
    rst.lil $10 ; Y Low
    ld a, h
    rst.lil $10 ; Y High         

    ld de, 16                  ; Move X by 16 pixels
    add ix, de
    
    pop hl
    inc hl                     ; Next tile in ram_buffer
    pop bc
    djnz draw_col_loop
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop
    pop iy
    pop ix

    ret

;------------------------------
; Find_And_Load_Room
; Input: A = Direction (0:S, 1:N, 2:E, 3:W)
;------------------------------
Find_And_Load_Room:
    push af
    ld hl, current_room_metadata

    ; Determine the base offset for the direction from the 49 bytes metadata
    cp 0                  ; SOUTH
    ld de, $23            ; Corrected: $1C + $07 = $23
    jr z, .do_load
    cp 1                  ; NORTH
    ld de, $24            ; Corrected: $1D + $07 = $24
    jr z, .do_load
    cp 2                  ; EAST
    ld de, $26            ; Corrected: $1F + $07 = $26
    jr z, .do_load     
    cp 3                  ; West
    ld de, $25            ; Corrected: $1E + $07 = $25
    jr z, .do_load

.do_load:
    add hl, de
    ld a, (hl)            ; A = New Room ID from the metadata buffer
    
    ; Basic exit check: $61 denotes "no exit"
    cp $61
    jr z, .no_exit

    ; Update current room id tracking
    ld (current_room_id), a

    ; Update the Room Pointer for the Draw_Room routine (ID * 3)
    ld hl, Room_Pointer_Table
    ld d, 0
    ld e, a
    add hl, de ;*2
    add hl, de ;*4
    add hl, de ;*8
    ld hl, (hl)           ; Load the 24-bit tile address
    ld (current_room_ptr), hl

    pop af                ; Restore direction for player repositioning
    call Set_Player_Spawn_Coords
    ld (player_dir), a    ; set player direction for after room transition.
    call Trigger_Room_Load
    ret

.no_exit:
    pop af
    ret

;-----------------------------------------------------------
; Set_Player_Spawn_Coords
; Sets X/Y based on which side of the screen we entered from
;-----------------------------------------------------------
Set_Player_Spawn_Coords:
    cp 0                  ; Hit South door
    jr z, .spawn_north
    cp 1                  ; Hit North door
    jr z, .spawn_south
    cp 2                  ; Hit East door
    jr z, .spawn_west
    cp 3                  ; Hit West door
    jr z, .spawn_east

.spawn_north:
    ;ld hl, 79             ; X center
    ;ld (player_x), hl
    ld hl, 34             ; Y top
    ld (player_y), hl
    ret
.spawn_south:
    ;ld hl, 79             ; X center
   ; ld (player_x), hl
    ld hl, 82             ; Y bottom
    ld (player_y), hl
    ret
.spawn_west:
    ld hl, 10             ; X left
    ld (player_x), hl
    ld hl, 48             ; Y center
    ld (player_y), hl
    ret
.spawn_east:
    ld hl, 154            ; X right
    ld (player_x), hl
    ld hl, 48             ; Y center
    ld (player_y), hl
    
    ret
