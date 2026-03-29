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

    ; Plot for Text
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


;--------------------------
; Print A fractional String
;--------------------------
print_frac_string: 

;----------------------
; fractional text loop    
;----------------------         
.frac_loop:
    ; Delay for printing chars speed
    V_SYNC
    V_SYNC

    ; Load char from string
    ld a, (hl) 
    or a
    ; check 0, the termination string
    ret z         
    ; push current char to stack
    push af      

;----------------------------------------
; compare current char
; jp if it's a special chars
; fall through as a default char if not
;----------------------------------------
    ; "|" char $7C (124 dec) = new line
    cp $7C
    jp z, .newline_char_pxs
    ; "space" $20 (32 dec) = 5 pixels
    cp $20
    jp z, .space_char_pxs
    ; "i" char $69 (105 dec) = 6 pixels 
    cp $69
    jp z, .i_char_pxs
    ; "l" char $6C (108 dec)
    cp $6C
    jp z, .l_char_pxs
    ; "m" char $6D (109 dec)
    cp $6D
    jp z, .m_char_pxs
    ; "w" char $77 (119 dec)
    cp $77
    jp z, .w_char_pxs

;-------------------------------------------------
; fall through if current char is a default char
; And jp depending on what the last char was
;-------------------------------------------------
    ld a, (last_char_spacing)
    cp $00                   ; last char was "default"
    jp z, .default_char_d_pxs
    cp $01                   ; last char was "space"
    jp z, .default_char_s_pxs
    cp $02                   ; last char was "i"
    jp z, .default_char_i_pxs
    cp $03                   ; last char was "l"
    jp z, .default_char_l_pxs
    cp $04                   ; last char was "m"
    jp z, .default_char_m_pxs
    cp $05                   ; last char was "w"
    jp z, .default_char_w_pxs

;-----------------------------------------
; set spacing for current default chars
; depending on the last char
;-----------------------------------------
; last char was a default char
.default_char_d_pxs:
    ; move 7 pixels right
    ld a, 7             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was a "space"
.default_char_s_pxs:
    ; move 5 pixels right
    ld a, 5             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "i"
.default_char_i_pxs:
    ; move 6 pixels right
    ld a, 6             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "l"
.default_char_l_pxs:
    ; move 6 pixels right
    ld a, 6             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "m"
.default_char_m_pxs:
    ; move 6 pixels right
    ld a, 8             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "w"
.default_char_w_pxs:
    ; move 6 pixels right
    ld a, 8             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char

;-----------------
; special chars
;-----------------
;-----------------------------------------
; set spacing for current special char
; depending on the last char
;-----------------------------------------

.space_char_pxs:
    ; move 5 pixels right
    ld a, 5             
    add a, b
    ld b, a
    jp .print_space_char

.i_char_pxs:
    ld a, (last_char_spacing)
    cp $04                   ; last char was "m"
    jp z, .i_char_m_pxs
    ld a, (last_char_spacing)
    cp $05                   ; last char was "w"
    jp z, .i_char_w_pxs

    ; move 6 pixels right
    ld a, 6             ;pixels to move
    add a, b
    ld b, a
    jp .print_i_char

.l_char_pxs:
    ld a, (last_char_spacing)
    cp $04                   ; last char was "m"
    jp z, .l_char_m_pxs
    ld a, (last_char_spacing)
    cp $05                   ; last char was "w"
    jp z, .l_char_w_pxs

    ; move 6 pixels right
    ld a, 6                        ;pixels to move
    add a, b
    ld b, a
    jp .print_l_char

.m_char_pxs:
    ; move 7 pixels right
    ld a, 7             ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char

.i_char_m_pxs:
    ; move 8 pixels right
    ld a, 8                                  ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char

.l_char_m_pxs:
    ; move 8 pixels right
    ld a, 8                                  ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char

.w_char_pxs:
    ; move 7 pixels right
    ld a, 7             ;pixels to move
    add a, b
    ld b, a
    jp .print_w_char

.i_char_w_pxs:
    ; move 8 pixels right
    ld a, 8             ;pixels to move
    add a, b
    ld b, a
    jp .print_w_char

.l_char_w_pxs:
    ; move 8 pixels right
    ld a, 8             ;pixels to move
    add a, b
    ld b, a
    jp .print_w_char

;------------------------------
; Plot And Print Chars
;-------------------------------------------------------------
; Plot and print default chars
; and set last char to default char ($00), for the next char
;-------------------------------------------------------------
.print_default_char:
    ; plot next chars pos
    plot_text_pos b, c   
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10
    ; next char
    inc hl
    ; set last char to "default" spacing
    ld a, $00
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "space" char
; and set last char to "space" char ($01), for the next char
;------------------------------------------------------------
.print_space_char:
    ; plot next chars pos
    plot_text_pos b, c
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10
    ; next char
    inc hl
    ; set last char to "space" spacing
    ld a, $01
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "i" char
; and set last char to "i" char ($02), for the next char)
;------------------------------------------------------------
.print_i_char:
    ; plot next chars pos
    plot_text_pos b, c    ; plot next chars pos
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10   
    ; next char
    inc hl
    ; set last char to "i"" spacing
    ld a, $02
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "l" char
; and set last char to default char ($03), for the next char)
;------------------------------------------------------------
.print_l_char:
    ; plot next chars pos
    plot_text_pos b, c    ; plot next chars pos
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10   
    ; next char
    inc hl
    ; set last char to "l"" spacing
    ld a, $03
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "m" char
; and set last char to "m" char ($04), for the next char)
;------------------------------------------------------------
.print_m_char:
    
    ; plot next chars pos
    plot_text_pos b, c    ; plot next chars pos
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10   
    ; next char
    inc hl
    ; set last char to "m"" spacing
    ld a, $04
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "w" char
; and set last char to "w" char ($05), for the next char)
;------------------------------------------------------------
.print_w_char:
    
    ; plot next chars pos
    plot_text_pos b, c    ; plot next chars pos
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10   
    ; next char
    inc hl
    ; set last char to "w" spacing
    ld a, $05
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop


;-------------------------------------------------------------
; Plot to New Line 10 px down
; and set last char to default char ($00), for the next char)
;------------------------------------------------------------
; new line = "|"
.newline_char_pxs:
    ; move 10 pixels down "Y"
    ld a, 10             ;pixels to move
    add a, c
    ld c, a

    ld a, (info_mode_is_active)
    cp $01
    jp z, .info_spacing_x
    
    ld a, (story_mode_is_active)
    cp $01
    jp z, .story_spacing_x


    
    ; fall into story spacing 

.story_spacing_x:
    ld b, 56

    pop af
    ; next char
    inc hl
    ; set last char to default spacing
    ld a, $00
    ld (last_char_spacing), a

    jp .frac_loop

    ; fall into .end_newline

.end_newline:

    ret

.info_spacing_x:
    ld b, 78

    pop af
    ; next char
    inc hl
    ; set last char to default spacing
    ld a, $00
    ld (last_char_spacing), a

    jp .frac_loop

    jp .end_newline

;---------------------------------------------------------------

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

;--------------------------------------------------------
; DEBUG TEMP CODE
;---------------------------------------------------------
; Debug_Overlay_Titles:
;---------------------------------------------------------
Debug_Overlay_Titles:
    ld a, (debug_is_active)
    cp $01
    jp nz, .no_debug_titles

    push af
    push bc
    push de
    push hl
    push ix

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

    ;-----------------------------------
    ; Look/Shoot Mode
    ;-----------------------------------
    TABTO 0, 6                  
    ld hl, Debug_LSM_String         
    call print_string

    ;-----------------------------------
    ; Small Alien HP
    ;-----------------------------------
    TABTO 0, 7                  
    ld hl, Debug_SHP_String         
    call print_string

    ;-----------------------------------
    ; Big Alien HP
    ;-----------------------------------
    TABTO 0, 8                  
    ld hl, Debug_BHP_String         
    call print_string

    ;-----------------------------------
    ; Aliens Amount Killed
    ;-----------------------------------
    TABTO 0, 9                  
    ld hl, Debug_KIL_String         
    call print_string

    pop ix
    pop hl
    pop de
    pop bc
    pop af

.no_debug_titles:

    ret
;---------------------------------------------------------
; Debug_Overlay Values:
;---------------------------------------------------------
Debug_Overlay_Values:
    ld a, (debug_is_active)
    cp $01
    jp nz, .no_debug_values
    ;--------------------------
    ; Current Room Number
    ;--------------------------
    TABTO 4, 0
    SET_TXT_COL bright_white
    ld a, (current_room_id)
    call Print_A_Number
    ;--------------------------
    ; Small Alien Active
    ;--------------------------
    TABTO 4, 1
    SET_TXT_COL bright_white
    ld a, (small_alien_is_active)
    call Print_A_Number
    ;--------------------------
    ; Big alien active
    ;--------------------------
    TABTO 4, 2
    SET_TXT_COL bright_white
    ld a, (big_alien_is_active)
    call Print_A_Number
    ;--------------------------
    ; Player current direction
    ;--------------------------
    TABTO 4, 3
    SET_TXT_COL bright_white
    ld a, (player_dir)
    call Print_A_Number
    ;--------------------------
    ; Tile ID under Player
    ;--------------------------
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
    ; Branching for look-ahead from the new center base Decode_Room_Metadata
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

    ;--------------------------
    ; Look Mode
    ;--------------------------
    TABTO 4, 6
    SET_TXT_COL bright_white
    ld a, (look_shoot_mode)
    call Print_A_Number

    ;--------------------------
    ; Small Alien HP
    ;--------------------------
    TABTO 4, 7
    SET_TXT_COL bright_white
    ld a, (small_alien_hp)
    call Print_A_Number

    ;--------------------------
    ; Big Alien health
    ;--------------------------
    TABTO 4, 8
    SET_TXT_COL bright_white
    ld a, (big_alien_hp)
    call Print_A_Number

    ;--------------------------
    ; Aliens Killed Draw_Health_Value
    ;--------------------------
    TABTO 4, 9
    SET_TXT_COL bright_white
    ld a, (alien_amount_killed)
    call Print_A_Number

.no_debug_values:

    ret