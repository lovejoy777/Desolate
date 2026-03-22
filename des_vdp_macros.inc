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
    ld a, 0                ; Command 1 0: Write to buffer
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

macro Add_Frame_To_Sprite_DE
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 38            ; Command: Add frame to sprite 16bit
    rst.lil $10
    ld a, e            ; Bitmap id, Default frame E
    rst.lil $10
    ld a, d            ; Bitmap id, Default frame
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

macro MAKEBUFFEREDBITMAP id, width, height, data, colourCode

	; create buffer

	.db 	23,0,$A0 							; write block
	.dw 	64000 + id 							; ID (word)
	.db 	3	 								; 'create' command
	.dw 	width *  height / 8 				; data size = length 320 x 64 / 8 bits as b/w Decode_Room_Metadata

	; clear in case VDP already has one

	.db 	23,0,$A0 							; write block
	.dw 	64000 + id 							; ID (word)
	.db 	2	 								; 'create' command

	; write data to the buffer

	.db 	23,0,$A0 							; write block
	.dw 	64000 + id 							; ID (word)
	.db 	0	 								; 'write' command

	.dw 	width *  height / 8 				; data size = length 320 x 64 / 8 bits as b/w
			
	incbin data

	; convert the buffer to a single colour bitmap

	.db 	18,0 								; select colour to use
	.db 	colourCode							; 1 of 64 possible colours from palette

	; convert buffer block to bitmap with ID

	.db 	23,27,$20 							; select buffer bitmap to use
	.dw 	64000 + id 							; ID (word)

	; create a bitmap from that ID

	.db 	23,27,$21 							; create bitmap from buffer
	.dw 	width, height				 		; width, height (both words)
	.db 	2 		

endmacro