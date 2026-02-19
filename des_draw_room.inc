;------------------------------------
; File: des_draw_room.inc
;------------------------------------

;------------------------------
; DECODE & DRAW ROOM
;------------------------------
Draw_Room:
    push ix
    push iy
    ld hl, (current_room_ptr)
    ld de, ram_buffer          
    ld b, 96                   
decode_main:
    ld a, (hl)            
    cp $FF              
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
    ld iy, 83                  
    ld b, 8                    
row_loop:
    push bc
    ld ix, 64                      
    ld b, 12                   
draw_col_loop:
    push bc
    push hl
    ld a, (hl)                 ; Get Tile ID from ram_buffer
    ld hl, 0                   ; 24-bit register for ADL mode
    ld l, a
    ld bc, 100                 ; Add 100 for background tile buffers
    add hl, bc  
    ex de, hl                  ; DE = Final VDP Buffer ID

    ;SELECT_BUFFER_DE
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 32            ; Command 32: Select Buffer
    rst.lil $10
    ld a, e             ; ID Low
    rst.lil $10
    ld a, d             ; ID High
    rst.lil $10   

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

;-------------------------------------------
; Find & Load Room
;-------------------------------------------
Find_And_Load_Room:
    ld b, a                
    ld a, (current_room_id)
    ld c, a                
    ld ix, Room_Links      
.loop:
    ld a, (ix + 0)         
    cp $FF                 
    ret z                  
    cp c                   
    jr nz, .next
    ld a, (ix + 2)         
    cp b
    jr nz, .next
    ld a, (ix + 1)         
    ld (current_room_id), a    
    ld a, (ix + 4)         
    ld (player_x), a
    ld a, (ix + 5)         
    ld (player_x + 1), a
    ld a, (ix + 6)         
    ld (player_y), a
    ld a, (ix + 7)         
    ld (player_y + 1), a
    ld hl, (ix + 8)            
    ld (current_room_ptr), hl
    ld a, (ix + 3)             
    ld (player_dir), a         
    xor a
    ld (is_moving), a          
    pop hl                     
    jp Trigger_Room_Load
.next:
    ld de, 12             
    add ix, de
    jr .loop

Found_South_Door:
    ld a, 0               
    call Find_And_Load_Room
    ret
Found_North_Door:
    ld a, 1               
    call Find_And_Load_Room
    ret
Found_East_Door:
    ld a, 2               
    call Find_And_Load_Room
    ret
Found_West_Door:
    ld a, 3               
    call Find_And_Load_Room

    ret