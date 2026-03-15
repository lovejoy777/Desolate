Draw_Main_Menu_BG:
    push ix
    push iy
    ld hl, (main_menu_bg_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes
decode_mmbg:
    ld a, (hl)            
    cp $FF              ; repeat flag
    jr nz, literal_byte_mmbg
    
    inc hl
    ld c, (hl)            
    inc hl
    ld a, (hl)            
repeat_loop_mmbg:
    ld (de), a            
    inc de
    dec b                 
    jp z, drawing_start_mmbg   
    dec c                 
    jr nz, repeat_loop_mmbg
    inc hl
    jr decode_mmbg
literal_byte_mmbg:
    ld (de), a
    inc de
    inc hl
    djnz decode_mmbg     

drawing_start_mmbg:
    ld hl, ram_buffer
    ld iy, 83                  
    ld b, 8                    
row_loop_mmbg:
    push bc
    ld ix, 64                      
    ld b, 12                   
draw_col_loop_mmbg:
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
    ld a, 3                  ; Command: Draw Bitmap
    rst.lil $10 
    
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
    djnz draw_col_loop_mmbg
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop_mmbg
    pop iy
    pop ix

    ret

;-------------------
; Draw Menu Screen 
;-------------------
Draw_Main_Menu:
    push ix
    push iy
    ld hl, (main_menu_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes (12x8)
decode_mm:
    ld a, (hl)            
literal_byte_mm:
    ld (de), a
    inc de
    inc hl
    djnz decode_mm     

drawing_start_mm:
    ld hl, ram_buffer
    ld iy, 83                  ; x coords                  
    ld b, 8                    
row_loop_mm:
    push bc
    ld ix, 64                  ; y coords    
    ld b, 12                   
draw_col_loop_mm:
    push bc
    push hl
    ld a, (hl)                 ; Get Tile ID from ram_buffer
    ld hl, 0                   ; 24-bit register for ADL mode
    ld l, a
    ld bc, 300                 ; Add 300 for menu tile buffers tileset2
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
    djnz draw_col_loop_mm
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop_mm
    pop iy
    pop ix

    ret

Draw_Main_Menu_BG_Fix:
    SET_GCOL_COL grey  
    ;--------------------------------------------------
    ; Rect 1 (Banner area) 
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 64    ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 93      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 254     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 118     ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; Rect 2 (Banner area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ;SET_GCOL_COL bright_white

    ld a, 25      ; plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 226    ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 89      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 252     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 92    ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; Rect 3 (Menu area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 70    ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 126      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 154     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 204     ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; Rect 4 (Face 1 area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 216    ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 146      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 246     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 174    ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; Rect 5 (Face 2 area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 226    ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 174      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 246     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 184    ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; Rect 6 (Face 3 area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 236    ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 184      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right of rect
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 252     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 194     ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    ; reset gcol colour to black.
    SET_GCOL_COL black          
    ret
