; -----------------------------------------------------------------------------
; SYSTEM & MOS MACROS FILE.
; -----------------------------------------------------------------------------

; Call a MOS API function
macro MOSCALL arg1
	ld a, arg1
	rst.lil $08
endmacro

; Wait for Vertical Blanking Interval (VSYNC)
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

; Set Screen Mode
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
macro SET_TXT_COL col
	ld a, 17					; set text colour
	rst.lil $10
    ld a, col                   ; text colour to use		    
	rst.lil $10
endmacro

; Set Text Background Colour
macro SET_TXT_BG_COL col
	ld a, 17					; set text colour
	rst.lil $10
    ld a, col                   ; background colour to use
    add a, 128					    
	rst.lil $10
endmacro

; Set Graphics Foreground Colour (GCOL)
macro SET_GCOL_COL col
	ld a, 18					; set graphics colour
	rst.lil $10
    ld a, 0                     ; mode 0
    rst.lil $10
	ld a, col					; colour to use
	rst.lil $10
endmacro

; Set Graphics Background Colour (GCOL)
macro SET_GCOL_BG_COL col
	ld a, 18					; set graphics colour
	rst.lil $10
    ld a, 0                     ; mode 0
    rst.lil $10
	ld a, col		            ; colour to use
    add a, 128
	rst.lil $10
endmacro

; Disable Graphics Scaling
macro SET_NONSCALED_GRAPHICS
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
macro HIDE_CURSOR
	ld a, 23					
	rst.lil $10
	ld a, 1		
	rst.lil $10
    ld a, 0		
	rst.lil $10
endmacro

; Show the flashing text cursor
macro SHOW_CURSOR
	ld a, 23
    rst.lil $10
    ld a, 1
    rst.lil $10
    ld a, 1
    rst.lil $10
endmacro

macro TABTO x, y
    ld a, 31
    rst.lil $10
    ld a, x
    rst.lil $10
    ld a, y
    rst.lil $10
endmacro

macro CLEAR_ALL_BUFFERS
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