;------------------------------------
; File: des_main_subs.inc
;------------------------------------

;---------------------------------
; Reset all varibles and buffers
;---------------------------------
CleanStart:
    ; reset the alien kill table, all alive
    ld hl, Alien_Kill_Table
    ld bc, 80                ; Number of rooms to clear small_alien_hp
    ld a, 3                  ; 1-3 = Alive, 0 = Dead
.init_kill_table:
    ld (hl), a
    inc hl
    dec bc
    ld a, b
    or c
    ld a, 3                  ; restore a for the next loop
    jr nz, .init_kill_table

    ; player vars to reset
    ld a, 1
    ld (player_is_alive), a
    ld a, 0
    ld (look_shoot_mode), a
    ld (ui_win_active), a
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a
    
    ld (current_room_id), a
    ld (player_dir), a
    ld (is_moving), a;
    ld (anim_frame), a;
    
    ; All none zero vars to reset
    ld hl, room_00
    ld (current_room_ptr), hl
    ld a, 79
    ld (player_x), a
    ld a, 67
    ld (player_y), a
    ld a, player_start_hp
    ld (player_health), a

    ret    
    

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
Set_Game_VP:
    ld a, 24           ; vdu 24,x1;y1;x2;y2; set graphics viewport
    rst.lil $10        ; send command to VDU
    ld hl, (game_screen_offset_x)          ; left (X1)
    call vdu_write_word
    ld hl, 210         ; bottom (Y1)
    call vdu_write_word
    ld hl, 254         ; right (X2)
    call vdu_write_word
    ld hl, (game_screen_offset_y)          ; top (Y2)
    call vdu_write_word
    ret

Reset_VP:
    ld a, 26                    ; reset vp to whole screen
    rst.lil $10
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
    cp 192                    
    jp nc, is_a_wall         ; if X >= 192, it's a wall
    srl a
    srl a
    srl a
    srl a
    ld b, a                   
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
    ld bc, 0                  
    ld c, a                    
    add hl, bc                 

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
    ;--------------------------------------------------
    ; Rect 1 (Health Value area) 
    ;--------------------------------------------------
    ; draw grey rectangle to mask old Health Value.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 106, 86, 130, 95, grey

    ;Plot_For_Text plot_x, plot_y
    Plot_For_Text 106, 86
            
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

    pop hl                  ; Restore string pointer
    call Print_Desol_String ; Call the printer

    ; --- Restore Screen colours---
    SET_TXT_BG_COL black
    SET_TXT_COL bright_white

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
    ret z         ; ends at the 0
    rst.lil $10   ; print a char
    inc hl        ; next char
    jr print_string

;------------------------------------------------------
; Helper to send 16-bit words to VDU.
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

