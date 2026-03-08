;------------------------------------
; File: des_main_subs.inc
;------------------------------------

;-------------------------
; initialize the display.
;-------------------------
Init_Display:
    SET_MODE 8
    SET_NONSCALED_GRAPHICS 
    SET_GCOL_BG_COL grey
    SET_GCOL_COL black           
    SET_TXT_BG_COL black
    CLS
    HIDE_CURSOR
    ret

;----------------------------------------
; Set Graphics ViewPort for game window.
;----------------------------------------
Set_Graphics_VP:
    ld a, 24           ; vdu 24,x1;y1;x2;y2; set graphics viewport
    rst.lil $10        ; send command to VDU
    ld hl, 64          ; left (X1)
    call vdu_write_word
    ld hl, 210         ; bottom (Y1)
    call vdu_write_word
    ld hl, 255         ; right (X2)
    call vdu_write_word
    ld hl, 83          ; top (Y2)
    call vdu_write_word
    ret

;--------------------------------------
; get_tile_at_coords: Our Players check
;--------------------------------------
get_tile_at_coords:
    push hl
    push de
    push bc
    ; HL contains X, DE contains Y
    ld a, l                  ; get X coordinate
    cp 192                   ; 
    jp nc, is_a_wall         ; if X >= 192, it's a wall
    srl a
    srl a
    srl a
    srl a
    ld b, a                  ; 
    ld a, e                  ; Y Coordinate from E
    cp 128                   ; if Y >= 128, it's a wall
    jp nc, is_a_wall         ; if Y >= 128, it's a wall
    srl a
    srl a
    srl a
    srl a
    ld c, a                   
    add a, a
    add a, c
    add a, a
    add a, a
    add a, b
    ld hl, ram_buffer         ; get tile from ram buffer
    ld bc, 0                  ; 
    ld c, a                   ; 
    add hl, bc                ; 

    ld a, (hl)                ; get tile from ram buffer
    pop bc
    pop de
    pop hl
    ret

is_a_wall:
    ld a, $0D                 ; return tile 13 (wall)   
    pop bc
    pop de
    pop hl
    ret

;---------------------------------------------------------
; Display HUD
;---------------------------------------------------------
Draw_Health_Value:
     
    SET_GCOL_COL grey           ; set gcol colour to grey for the clearing the health value.

    ; draw grey rectangle over current health value.
    ld a, 25
    rst.lil $10
    ld a, 69
    rst.lil $10
    ld a, 106
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 86
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 25
    rst.lil $10
    ld a, 101
    rst.lil $10
    ld a, 130
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 95
    rst.lil $10
    ld a, 0
    rst.lil $10

    SET_GCOL_COL black          ; reset gcol colour to black.

    ld a, 25
    rst.lil $10
    ld a, 4                     ; vdu 25,4,x;y; plot move command.
    rst.lil $10
    ld a, 106
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 86
    rst.lil $10
    ld a, 0
    rst.lil $10
            
    TEXT_AT_GRAPHICS_CURSOR     ; set text at graphics cursor.

    ; Print the actual health number
    ld a, (player_health)
    call Print_A_Number
    TEXT_AT_TEXT_CURSOR         ; reset back to text at text cursor.
    ret

;-----------------------------------------------------------
; Display_Message In PopUps
; Input: HL = Pointer to the message string
;-----------------------------------------------------------
Display_Message:
    ld b, 23                ; start row for newline text
    push hl                 ; Save string pointer (SE131)
    SET_TXT_BG_COL 8
    SET_TXT_COL black

    Select_Sprite_8bit 0
    Hide_Sprite

    call Draw_Sm_Popup      ; Draw the 192x48 border

    ; Position cursor inside the window for row 1
    TABTO 9, 22

    pop hl                  ; Restore string pointer
    call Print_Desol_String ; Call the printer
    PLAY_SOUND 0, 127, 1000, 10
    ret 
;--------------------------
; Print Desol String
;--------------------------
Print_Desol_String:
.string_loop:
    ld a, (hl)
    or a
    ret z                  ; Stop at null
    cp '|'                 ; Check for Newline first
    jp z, .newline
    cp $20                 ; Skip control codes
    jp c, .skip_it
    ; --- Character Printing ---
    push af
    cp $21               
    jr c, .skip_sound
.skip_sound:
    pop af
    rst.lil $10
.next_char:
    inc hl
    jp .string_loop
.newline:
    TABTO 9, b
    inc b
    inc hl                 ; Skip the '|'
    jp .string_loop
.skip_it:
    inc hl
    jp .string_loop
    ret

;--------------------------
; Print A String
;--------------------------
print_string:
    ld a, (hl)
    or a
    ret z
    rst.lil $10
    inc hl
    jr print_string

;------------------------------------------------------
; Helper to send 16-bit words (little endian) to VDU.
;------------------------------------------------------
vdu_write_word:
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
    ret

;-----------------------------------------------------
; Print_A_Number: Prints the 8-bit value in A (0-100)
;-----------------------------------------------------
Print_A_Number:
    ld b, '0'
.h_loop:
    cp 100
    jr c, .h_done
    sub 100
    inc b
    jr .h_loop
.h_done:
    ld c, a             
    ld a, b
    rst.lil $10         
    ld a, c
    ld b, '0'
.t_loop:
    cp 10
    jr c, .t_done
    sub 10
    inc b
    jr .t_loop
.t_done:
    ld c, a
    ld a, b
    rst.lil $10         
    ld a, c
    add a, '0'
    rst.lil $10         
    ret

;--------------------------
; Prints a Hex Byte
;--------------------------
Print_Hex_Byte:
    push af
    rra
    rra
    rra
    rra
    call .nibble
    pop af
.nibble:
    and $0F
    add a, $30
    cp $3A
    jr c, .print
    add a, 7
.print:
    rst.lil $10
    ret

;--------------------------
; Simple Random number
;--------------------------
Get_Random_0_3:
    ld a, (alien_random_seed)
    ld b, a
    add a, a
    add a, b
    add a, 7            ; Simple math to scramble the seed
    ld (alien_random_seed), a
    and 00000011b       ; Mask it so we only get 0, 1, 2, or 3
    ret

;----------------------------------------------------------

;--------------------------------------------------------
; DEBUG TEMP CODE
;---------------------------------------------------------
; Debug_Overlay_Titles:
;---------------------------------------------------------
Debug_Overlay_Titles:
    push af
    push bc
    push de
    push hl
    push ix

    ;SET_TXT_BG_COL black
    
    ;----------------------------
    ; Current Room Number Title
    ;----------------------------
    TABTO 0, 0                  
    ld hl, Debug_CRN_String         
    call print_string     
    
    ;--------------------------
    ; Small Alien Active Title
    ;--------------------------
    TABTO 0, 1                  
    ld hl, Debug_SAA_String         
    call print_string
    
    ;--------------------------
    ; Big alien active Title
    ;--------------------------
    TABTO 0, 2              
    ld hl, Debug_BAA_String         
    call print_string

    ;--------------------------------
    ; Player current direction Title
    ;--------------------------------
    TABTO 0, 3                  
    ld hl, Debug_PCD_String         
    call print_string

    ;----------------------------
    ; Tile ID under Player Title
    ;----------------------------
    TABTO 0, 4               
    ld hl, Debug_TPO_String         
    call print_string

    ;-----------------------------------
    ; Tile ID In Front of Player Title
    ;-----------------------------------
    TABTO 0, 5                  
    ld hl, Debug_TIP_String         
    call print_string

    pop ix
    pop hl
    pop de
    pop bc
    pop af
    ret
;---------------------------------------------------------
; Debug_Overlay Values:
;---------------------------------------------------------
Debug_Overlay_Values:
    push af
    push bc
    push de
    push hl
    push ix

    ;SET_TXT_BG_COL black
    
    ;--------------------------
    ; Current Room Number
    ;--------------------------
    TABTO 0, 0                  
    ld hl, Debug_CRN_String         
    call print_string           
    TABTO 4, 0
    SET_TXT_COL bright_white
    ld a, (current_room_id)
    call Print_A_Number
    
    ;--------------------------
    ; Small Alien Active
    ;--------------------------
    TABTO 0, 1                  
    ld hl, Debug_SAA_String         
    call print_string
    TABTO 4, 1
    SET_TXT_COL bright_white
    ld a,(sm_alien_is_active)
    call Print_A_Number
    
    ;--------------------------
    ; Big alien active
    ;--------------------------
    TABTO 0, 2              
    ld hl, Debug_BAA_String         
    call print_string
    TABTO 4, 2
    SET_TXT_COL bright_white
    ld a,(big_alien_is_active)
    call Print_A_Number

    ;--------------------------
    ; Player current direction
    ;--------------------------
    TABTO 0, 3                  
    ld hl, Debug_PCD_String         
    call print_string
    TABTO 4, 3
    SET_TXT_COL bright_white
    ld a, (player_dir)
    call Print_A_Number

    ;--------------------------
    ; Tile ID under Player
    ;--------------------------
    TABTO 0, 4               
    ld hl, Debug_TPO_String         
    call print_string
    TABTO 4, 4
    SET_TXT_COL bright_white
    ld hl, (player_x)
    ld de, (player_y)
    ; Normalize base to Center-Mass (16, 24)
    ld a, l
    add a, 16           
    ld l, a
    ld a, e
    add a, 24          
    ld e, a
    call get_tile_at_coords 
    call Print_A_Number

    ;------------------------------
    ; Tile ID In Front of Player
    ;------------------------------
    TABTO 0, 5                  
    ld hl, Debug_TIP_String         
    call print_string
    TABTO 4, 5
    SET_TXT_COL bright_white
    ld hl, (player_x)
    ld de, (player_y)
    ; Normalize base to Center-Mass (16, 24)
    ld a, l
    add a, 16
    ld l, a
    ld a, e
    add a, 24
    ld e, a
    ld a, (player_dir)
    ; Branching for look-ahead from the new center base
    cp 0                ; Down
    jr z, .dbg_down
    cp 1                ; Up
    jr z, .dbg_up
    cp 2               ; RIGHT
    jr z, .dbg_right
    cp 3                ; LEFT
    jr z, .dbg_left
    jr .dbg_print       ; Fallback
.dbg_down:
    ld a, e
    add 8              ; Look 8px Down from center
    ld e, a
    jr .dbg_print
.dbg_up:
    ld a, e
    sub a, 8           ; Look 8px Up from center
    ld e, a
    jr .dbg_print
.dbg_right:
    ld a, l
    add a, 10           ; Look 10px right from center
    ld l, a
    jr .dbg_print
.dbg_left:
    ld a, l
    sub 10              ; Look 10px left from center
    ld l, a

.dbg_print:
    call get_tile_at_coords
    call Print_A_Number

    pop ix
    pop hl
    pop de
    pop bc
    pop af
    ret