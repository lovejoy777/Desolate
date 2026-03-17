; =============================================================================
; FILE: des_draw_banner.inc
; Purpose: Draw the already-loaded Buffer 30 to the screen
; =============================================================================

Draw_Banner_Image:
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 63, 20, 254, 67, grey
    ; Select Buffer 30.
    ; select and draw the title top
	ld a,63
	ld (bitmap_x),a
    ld a,20
	ld (bitmap_y),a 
	ld a,30                 ; bitmap/buffer id
	ld (which_bitmap),a 
	ld a,250
	ld (which_bitmap + 1),a

    ; draw bitmap
    push hl
	push bc
	ld hl, draw_bmStart
	ld bc, draw_bmEnd - draw_bmStart
	rst.lil $18
	pop bc
	pop hl
	ret 

draw_bmStart:
    		.db 23, 27, $20		 	; select bitmap (16 bit id)
which_bitmap:	.dw 0			 	; bitmap number 30

		.db 25, $ED	         	    ; plot bitmap at x,y
bitmap_x:	.dw 0	      	        ; to x pos (a word not a byte) 63
bitmap_y:	.dw 0		         	; to y pos (a word not a byte) 83
draw_bmEnd: