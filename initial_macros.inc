; -----------------------------------------------------------------------------
; SYSTEM & MOS MACROS FILE.
; -----------------------------------------------------------------------------

; Call a MOS API function
; Arguments:
;   arg1: MOS command ID (e.g., $08 for sysvars, $13 for vblank)
macro MOSCALL arg1
	ld a, arg1
	rst.lil $08
endmacro

; Wait for Vertical Blanking Interval (VSYNC)
; Uses VDU 23, 0, &C3 to wait for VSYNC (and swap buffers if double buffered).
; This ensures the VDP command queue is synchronized with the display.
macro V_SYNC
    ld a, 23
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, $C3
    rst.lil $10
endmacro

; Clear the screen
macro CLS
	ld a, 12                    ; clear screen
	rst.lil $10
endmacro

; Clear the graphics area
macro CLG
	ld a, 16                    ; clear screen
	rst.lil $10
endmacro

; Set Video Mode
; Arguments:
;   mode: Screen mode (e.g., 8 for 320x240 64-colour)
macro SET_MODE mode
	ld a, 22					; set mode...
	rst.lil $10
	ld a, mode					; to mode
	rst.lil $10
endmacro

macro TEXT_AT_TEXT_CURSOR
    ld a, 4
    rst.lil $10
endmacro

macro TEXT_AT_GRAPHICS_CURSOR
    ld a, 5
    rst.lil $10
endmacro


; Set Text Foreground Colour
; Arguments:
;   col: Colour index (0-63 in Mode 8)
macro SET_TXT_COL col
	ld a, 17					; set text colour
	rst.lil $10
    ld a, col                   ; text colour to use		    
	rst.lil $10
endmacro

; Set Text Background Colour
; Arguments:
;   col: Colour index (0-63 in Mode 8)
macro SET_TXT_BG_COL col
	ld a, 17					; set text colour
	rst.lil $10
    ld a, col                   ; background colour to use
    add a, 128					    
	rst.lil $10
endmacro

; Set Graphics Foreground Colour (GCOL)
; Arguments:
;   col: Colour index
macro SET_GCOL_COL col
	ld a, 18					; set graphics colour
	rst.lil $10
    ld a, 0                     ; mode 0
    rst.lil $10
	ld a, col					; colour to use
	rst.lil $10
endmacro

macro SET_GCOL_BG_COL col
	ld a, 18					; set graphics colour
	rst.lil $10
    ld a, 0                     ; mode 0
    rst.lil $10
	ld a, col		    ; colour to use
    add a, 128
	rst.lil $10
endmacro

; Disable Graphics Scaling
; Ensures 1 logical unit = 1 pixel. Essential for direct pixel mapping.
; Sends VDU 23, 0, &C0, 0
macro SET_NONSCALED_GRAPHICS     ; set to non-scaled graphics
    ld a, 23                   
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 192
    rst.lil $10
    ld a, 0
    rst.lil $10
endmacro

; Hide the flashing text cursor
macro HIDE_CURSOR               ; Hide Text cursor
	ld a, 23					
	rst.lil $10
	ld a, 1		
	rst.lil $10
    ld a, 0		
	rst.lil $10
endmacro

; Show the flashing text cursor
macro SHOW_CURSOR              ; Show Text cursor
	ld a, 23
    rst.lil $10
    ld a, 1
    rst.lil $10
    ld a, 1
    rst.lil $10
endmacro

; Move text cursor to X, Y coordinates
; Arguments:
;   x: Column (0-based)
;   y: Row (0-based)
macro TABTO x, y
    ld a, 31
    rst.lil $10
    ld a, x
    rst.lil $10
    ld a, y
    rst.lil $10
endmacro

macro CLEAR_ALL_BUFFERS
    ; --- NEW: CLEAR VDP BUFFERS ---
    ld a, 23
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, $A0              ; Buffer Management
    rst.lil $10
    ld a, $FF              ; ID -1 (Low)
    rst.lil $10
    ld a, $FF              ; ID -1 (High)
    rst.lil $10
    ld a, 2                ; Command: Clear All
    rst.lil $10
endmacro

; -----------------------------------------------------------------------------
; COLOUR CONSTANTS
; -----------------------------------------------------------------------------
black:          equ     0
red:            equ     1
green:          equ     2
yellow:         equ     3
blue:           equ     4
magenta:        equ     5
cyan:           equ     6
grey:           equ     7
dark_grey:      equ     8
bright_red:     equ     9
bright_green:   equ     10
bright_yellow:  equ     11
bright_blue:    equ     12
bright_magenta: equ     13
bright_cyan:    equ     14
bright_white:   equ     15