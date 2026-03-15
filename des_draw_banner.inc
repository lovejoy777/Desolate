; =============================================================================
; FILE: des_draw_banner.inc
; Purpose: Draw the already-loaded Buffer 30 to the screen
; =============================================================================

Draw_Banner:
    ; Select Buffer 30.
    Select_Buffer_8bit 30

    ; Draw Buffer 30 at X:64, Y:30.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             
    rst.lil $10
    ld a, 64           ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 20           ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ret