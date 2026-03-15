; select buffer 8bit
macro Select_Buffer_8bit bid
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, bid
    rst.lil $10
endmacro

; Load colour bitmap data into current buffer
macro Write_To_Buffer_8bit width, height
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 1               ; command: Load colour bitmap data into current buffer
    rst.lil $10
    ld a, width           ; width   
    rst.lil $10
    ld a, 0            
    rst.lil $10
    ld a, height          ; Height
    rst.lil $10
    ld a, 0           
    rst.lil $10
endmacro

macro SELECT_BUFFER_DE
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
endmacro

; Initiate Buffer Upload (16-bit ID)
; Sends the VDP command header to start writing data to a buffer.
; Arguments:
macro Write_To_Buffer_16bit hreg, lreg
    ld a, 23
    rst.lil $10
    ld a, 0                ; Command 1 0: Write to buffer
    rst.lil $10 
    ld a, $A0              ; VDP Command: Buffer Upload
    rst.lil $10

    ld a, lreg             ; Bitmap ID Low (from DE register)
    rst.lil $10
    ld a, hreg             ; Bitmap ID High (from DE register)
    rst.lil $10
    ld a, 0                ; Command 2 0: Write to buffer
    rst.lil $10
    ld a, 32               ; Length hi
    rst.lil $10
    ld a, 0                ; Command 2 0: Write to buffer
    rst.lil $10
endmacro

macro Create_Bitmap width, height, mode
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 33            ; was 33
    rst.lil $10
    ld a, width            ; Width 16
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, height            ; Height 16
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, mode             ; Type 2 (Paletted)
    rst.lil $10
endmacro

macro Select_Sprite_8bit sid
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 4             ; Command: Select Sprite
    rst.lil $10
    ld a, sid            ; Sprite Id
    rst.lil $10
endmacro

macro Add_Frame_To_Sprite bid
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 6            ; Command: Add frame to sprite
    rst.lil $10
    ld a, bid            ; Bitmap id, Default frame
    rst.lil $10
endmacro

macro Show_Sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 11            ; Command: Update sprites to GPU
    rst.lil $10
endmacro

macro Hide_Sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 12            ; Hide Sprite
    rst.lil $10
endmacro

macro Activate_amount_sprites amount 
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 7            ; Command: Activate the amount of sprites
    rst.lil $10
    ld a, amount            ; Amount of sprites to activate
    rst.lil $10
endmacro

macro Load_Next_Frame
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 8       ; Command: Load the next frame
    rst.lil $10
endmacro

macro Update_GPU
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 15           ; Command: update gpu
    rst.lil $10
endmacro

macro Draw_Tile tile_x, tile_y
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Draw Bitmap
    rst.lil $10
    ld hl, tile_x
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
    ld hl, tile_y
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
endmacro

macro Hide_Shoot_Indicator
    SET_GCOL_COL grey
    ;---------------------------------------------------
    ; draw grey rectangle over shoot (right) indicator.
    ld a, 25
    rst.lil $10
    ld a, 69
    rst.lil $10

    ld a, 225
    rst.lil $10
    ld a, 0
    rst.lil $10

    ld a, 85
    rst.lil $10
    ld a, 0
    rst.lil $10

    ld a, 25
    rst.lil $10
    ld a, 101
    rst.lil $10
    ld a, 232
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 95
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; draw grey rectangle over shoot (left) indicator.
    ; top left
    ld a, 25      ;plot
    rst.lil $10
    ld a, 69      ; point in current colour
    rst.lil $10
    ld a, 203     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 85      ; y
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right
    ld a, 25      ; plot
    rst.lil $10
    ld a, 101     ; fill rectangle
    rst.lil $10
    ld a, 210     ; x
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 95     ; y
    rst.lil $10
    ld a, 0
    rst.lil $10

    SET_GCOL_COL black          ; reset gcol colour to black.

endmacro
macro Hide_Look_Indicator
    SET_GCOL_COL grey           
    ;-------------------------------------------------
    ; draw grey rectangle over look (left) indicator.
    ; top left
    ld a, 25
    rst.lil $10
    ld a, 69
    rst.lil $10
    ld a, 182
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 85
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right
    ld a, 25
    rst.lil $10
    ld a, 101
    rst.lil $10
    ld a, 189
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 95
    rst.lil $10
    ld a, 0
    rst.lil $10

    ;--------------------------------------------------
    ; draw grey rectangle over look (right) indicator.
    ; top left
    ld a, 25
    rst.lil $10
    ld a, 69
    rst.lil $10
    ld a, 203
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 85
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; bottom right
    ld a, 25
    rst.lil $10
    ld a, 101
    rst.lil $10
    ld a, 210
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 95
    rst.lil $10
    ld a, 0
    rst.lil $10

    SET_GCOL_COL black            ; reset gcol colour to black.
endmacro

