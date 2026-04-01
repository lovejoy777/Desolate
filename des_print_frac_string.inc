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
    ; "M" char $4D (77 dec)
    cp $4D
    jp z, .M_char_pxs
    ; "w" char $77 (119 dec)
    cp $77
    jp z, .w_char_pxs
    ; "W" char $57 (87 dec)
    cp $57
    jp z, .W_char_pxs

    ; "full stop" char $2E (46 dec)
    cp $2E
    jp z, .i_char_pxs
    ; "apostrophe" char $27 (39 dec)
    cp $27
    jp z, .i_char_pxs

;-------------------------------------------------
; fall through if current char is a default char
; And jp depending on what the last char was
;-------------------------------------------------
    ld a, (last_char_spacing)
    cp $00  ; last char was "default", (not a special)
    jp z, .default_char_d_pxs

    cp $01  ; last char was "spacebar"
    jp z, .default_char_s_pxs

    ; Special Letters
    cp $02  ; last char was "i"
    jp z, .default_char_i_pxs
    ;cp $03  ; last char was "I"            
   ; jp z, .default_char_I_pxs

    cp $04  ; last char was "l"
    jp z, .default_char_l_pxs
    ;cp $05  ; last char was "L"
    ;jp z, .default_char_l_pxs
    
    cp $06  ; last char was "m"
    jp z, .default_char_m_pxs
    cp $07  ; last char was "M"
    jp z, .default_char_M_pxs

    cp $08 ; last char was "w"
    jp z, .default_char_w_pxs
    cp $09 ; last char was "W"
    jp z, .default_char_W_pxs

    ; Punctuation
    cp $0A ; last char was "."
    jp z, .default_char_i_pxs
    cp $0B ; last char was "'"
    jp z, .default_char_i_pxs

;-----------------------------------------
; set spacing for current default chars
; depending on the last char
;-----------------------------------------
; last char was a default char
.default_char_d_pxs:
    ; move pixels right
    ld a, 7             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was a "space"
.default_char_s_pxs:
    ; move pixels right
    ld a, 5             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char

; last char was an "i"
.default_char_i_pxs:
    ; move pixels right
    ld a, 6             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "l"
.default_char_l_pxs:
    ; move pixels right
    ld a, 6             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char

; last char was an "m"
.default_char_m_pxs:
    ; move pixels right
    ld a, 8             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "M"
.default_char_M_pxs:
    ; move pixels right
    ld a, 8             ; pixels to move
    add a, b
    ld b, 10
    jp .print_default_char

; last char was an "w"
.default_char_w_pxs:
    ; move pixels right
    ld a, 8             ; pixels to move
    add a, b
    ld b, a
    jp .print_default_char
; last char was an "w"
.default_char_W_pxs:
    ; move pixels right
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
    ; move pixels right
    ld a, 5             
    add a, b
    ld b, a
    jp .print_space_char

.i_char_pxs:
    ; lowercase "m"
    ld a, (last_char_spacing)
    cp $06                   ; last char was "m"
    jp z, .i_char_m_pxs
    ; uppercase "M"
    ld a, (last_char_spacing)
    cp $07                   ; last char was "M"
    jp z, .i_char_m_pxs

    ; lowercase "w"
    ld a, (last_char_spacing)
    cp $08                   ; last char was "w"
    jp z, .i_char_w_pxs
    ; uppercase "W"
    ld a, (last_char_spacing)
    cp $09                   ; last char was "W"
    jp z, .i_char_w_pxs


    ; move pixels right
    ld a, 6             ;pixels to move
    add a, b
    ld b, a
    jp .print_i_char

.l_char_pxs:
    ; lowercase "m"
    ld a, (last_char_spacing)
    cp $06                   ; last char was "m"
    jp z, .i_char_m_pxs
    ; uppercase "M"
    ld a, (last_char_spacing)
    cp $07                   ; last char was "M"
    jp z, .i_char_m_pxs

    ; lowercase "w"
    ld a, (last_char_spacing)
    cp $08                   ; last char was "w"
    jp z, .i_char_w_pxs
    ; uppercase "W"
    ld a, (last_char_spacing)
    cp $09                   ; last char was "W"
    jp z, .i_char_w_pxs

    ; move pixels right
    ld a, 6                        ;pixels to move
    add a, b
    ld b, a
    jp .print_l_char

.m_char_pxs:
    ; pixels right
    ld a, 7             ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char
.M_char_pxs:
    ; pixels right
    ld a, 8             ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char

.w_char_pxs:
    ; move pixels right
    ld a, 7             ;pixels to move
    add a, b
    ld b, a
    jp .print_w_char
.W_char_pxs:
    ; move pixels right
    ld a, 8             ;pixels to move
    add a, b
    ld b, a
    jp .print_w_char

.l_char_l_pxs:
    ; move pixels right
    ld a, 8                                  ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char

.i_char_m_pxs:
    ; move pixels right
    ld a, 8                                  ;pixels to move
    add a, b
    ld b, a
    jp .print_m_char

.i_char_w_pxs:
    ; move pixels right
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
; Plot and print "i,fs,ap etc" chars
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
    ld a, $04
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "m & M" char
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
    ld a, $06
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop

;-------------------------------------------------------------
; Plot and print "w & W" char
; and set last char to "w" char ($05), for the next char)
;------------------------------------------------------------
.print_w_char:
    
    ; plot next chars pos, b=x, c=y
    plot_text_pos b, c    ; plot next chars pos
    ; get char from stack
    pop af
    ; print the char
    rst.lil $10   
    ; next char
    inc hl
    ; set last char to "w" spacing
    ld a, $08
    ld (last_char_spacing), a
    ; return to main loop
    jp .frac_loop




;-------------------------------------------------------------
; Plot to New Line 10 px down, and x depending on where
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
