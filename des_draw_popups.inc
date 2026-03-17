; =============================================================================
; FILE: des_draw_popups.inc
; Purpose: Draw the pop up window tiles from Tileset2 data block to the screen
; =============================================================================

;---------------------------------------
; Draw Small Popup Window from tileset2 
;---------------------------------------
Draw_Small_Popup:
    
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 67, 103, 252, 202, grey

    push ix
    push iy

    ld hl, (inventory_popup_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes (12x8)
decode_smp:
    ld a, (hl)            
    cp $FF              ; repeat flag
    jr nz, literal_byte_smp
    
    inc hl
    ld c, (hl)            
    inc hl
    ld a, (hl)            
repeat_loop_smp:
    ld (de), a            
    inc de
    dec b                 
    jp z, drawing_start_smp   
    dec c                 
    jr nz, repeat_loop_smp
    inc hl
    jr decode_smp            
literal_byte_smp:
    ld (de), a
    inc de
    inc hl
    djnz decode_smp     

drawing_start_smp:
    ld hl, ram_buffer
    ld iy, (game_screen_offset_y)                  ; y coords                  
    ld b, 8                    
row_loop_smp:
    push bc
    ld ix, (game_screen_offset_x)                  ; x coords    
    ld b, 12                   
draw_col_loop_smp:
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
    djnz draw_col_loop_smp
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop_smp

    pop iy
    pop ix

    ret

;--------------------------------------------
; Draw Inventory Popup Window from tileset2 
;--------------------------------------------
Draw_Inventory_Popup:
    
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 69, 103, 252, 202, grey

    push ix
    push iy

    ld hl, (inventory_popup_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes (12x8)
decode_ipu:
    ld a, (hl)            
    cp $FF              ; repeat flag
    jr nz, literal_byte_ipu
    
    inc hl
    ld c, (hl)            
    inc hl
    ld a, (hl)            
repeat_loop_ipu:
    ld (de), a            
    inc de
    dec b                 
    jp z, drawing_start_ipu   
    dec c                 
    jr nz, repeat_loop_ipu
    inc hl
    jr decode_ipu            
literal_byte_ipu:
    ld (de), a
    inc de
    inc hl
    djnz decode_ipu     

drawing_start_ipu:
    ld hl, ram_buffer
    ld iy, (game_screen_offset_y)                  ; y coords                  
    ld b, 8                    
row_loop_ipu:
    push bc
    ld ix, (game_screen_offset_x)                  ; x coords    
    ld b, 12                   
draw_col_loop_ipu:
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
    djnz draw_col_loop_ipu
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop_ipu

    pop iy
    pop ix

    ret


;---------------------------------------------------
; Draw Data Cart Reader Popup Window from tileset2 
;---------------------------------------------------
Draw_Data_Cart_Popup:

    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 69, 73, 244, 202, grey
    
    push ix
    push iy
    
    
    ld hl, (data_cart_popup_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes (12x8)
decode_dcr:
    ld a, (hl)            
    cp $FF              ; repeat flag
    jr nz, literal_byte_dcr
    
    inc hl
    ld c, (hl)            
    inc hl
    ld a, (hl)            
repeat_loop_dcr:
    ld (de), a            
    inc de
    dec b                 
    jp z, drawing_start_dcr   
    dec c                 
    jr nz, repeat_loop_dcr
    inc hl
    jr decode_dcr            
literal_byte_dcr:
    ld (de), a
    inc de
    inc hl
    djnz decode_dcr     

drawing_start_dcr:
    ld hl, ram_buffer
    ld iy, (game_screen_offset_y)                  ; y coords                  
    ld b, 8                    
row_loop_dcr:
    push bc
    ld ix, (game_screen_offset_x)                  ; x coords    
    ld b, 12                   
draw_col_loop_dcr:
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
    djnz draw_col_loop_dcr
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop_dcr

    pop iy
    pop ix

    ret


;---------------------------------------------------
; Draw Door Lock Popup Window from tileset2 
;---------------------------------------------------
Draw_Door_Lock_Popup:
    
    ; Left Panel
    Draw_Filled_Rect 65, 83, 170, 202, grey
    ; Right Panel
    Draw_Filled_Rect 179, 120, 250, 202, grey

    push ix
    push iy

    ld hl, (door_lock_popup_ptr)
    ld de, ram_buffer          
    ld b, 96            ; 96 bytes (12x8)
decode_dlp:
    ld a, (hl)            
    cp $FF              ; repeat flag
    jr nz, literal_byte_dlp
    
    inc hl
    ld c, (hl)            
    inc hl
    ld a, (hl)            
repeat_loop_dlp:
    ld (de), a            
    inc de
    dec b                 
    jp z, drawing_start_dlp   
    dec c                 
    jr nz, repeat_loop_dlp
    inc hl
    jr decode_dlp            
literal_byte_dlp:
    ld (de), a
    inc de
    inc hl
    djnz decode_dlp     

drawing_start_dlp:
    ld hl, ram_buffer
    ld iy, (game_screen_offset_y)                  ; y coords                  
    ld b, 8                    
row_loop_dlp:
    push bc
    ld ix, (game_screen_offset_x)                  ; x coords    
    ld b, 12                   
draw_col_loop_dlp:
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
    djnz draw_col_loop_dlp
    
    ; --- Handle Row Increment (Y axis) ---
    ld de, 16
    add iy, de
    pop bc
    djnz row_loop_dlp

    pop iy
    pop ix

    ret
    