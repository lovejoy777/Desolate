;--------------------------
; Draw Main Menu Screen BG 
;--------------------------
; Draw 96 bytes full (12x8) 16x16 tiles from Tileset1 
Draw_Main_Menu_BG:
    push ix
    push iy

    ld hl, (main_menu_bg_ptr)
    ld de, ram_buffer          
    ld bc, 0
    ld b, 96            ; 96 total bytes
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
    ld iy, (game_screen_offset_y)    ; y  
    ld bc, 0          
    ld b, 8      ; 8 tiles             
row_loop_mmbg:
    push bc
    ld ix, (game_screen_offset_x)    ; x                  
    ld b, 12     ; 12 tiles            
draw_col_loop_mmbg:
    push bc
    push hl
    ld a, (hl)                 ; Get Tile ID from ram_buffer
    ld hl, 0                   ; 24-bit register for ADL mode
    ld bc, 0
    ld l, a
    ld bc, 100                 ; Add 100 for background tile buffers Tileset1
    add hl, bc  
    ex de, hl                  ; DE = Final VDP Buffer ID

    SELECT_BUFFER_DE

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

;-----------------------
; Draw Main Menu Screen 
;-----------------------
Draw_Main_Menu:

    call Draw_Main_Menu_BG_Fix

    push ix
    push iy

    ld hl, 0
    ld bc, 0
    ld de, 0
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
    ld iy, (game_screen_offset_y)                  ; y coords  
    ld bc, 0                
    ld b, 8                    
row_loop_mm:
    push bc
    ld ix, (game_screen_offset_x)                  ; x coords 
    ld bc, 0   
    ld b, 12                   
draw_col_loop_mm:
    push bc
    push hl
    ld a, (hl)                 ; Get Tile ID from ram_buffer
    ld hl, 0                   ; 24-bit register for ADL mode
    ld bc, 0
    ld l, a
    ld bc, 300                 ; Add 300 for menu tile buffers tileset2
    add hl, bc  
    ex de, hl                  ; DE = Final VDP Buffer ID

    SELECT_BUFFER_DE

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
    ;--------------------------------------------------
    ; Rect 1 (Banner area) 
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; top left of rect
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 64, 93, 254, 118, grey
    ;--------------------------------------------------
    ; Rect 2 (Banner area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 226,89, 252, 92, grey
    ;--------------------------------------------------
    ; Rect 3 (Menu area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 70, 126, 154, 204, grey
    ;--------------------------------------------------
    ; Rect 4 (Face 1 area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 216, 146, 246, 174, grey
    ;--------------------------------------------------
    ; Rect 5 (Face 2 area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 226, 174, 246, 184, grey
    ;--------------------------------------------------
    ; Rect 6 (Face 3 area)
    ;--------------------------------------------------
    ; draw grey rectangle to mask bg overlap pixels.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 235, 184, 252, 194, grey 

    ret

