; =============================================================================
; FILE: des_draw_banner.inc
; Purpose: Draw the already-loaded Buffer 76 to the screen
; =============================================================================

Draw_Banner:
    ; Select Buffer 76.
    Select_Buffer_8bit 76

    ; Draw Buffer 76 at X:64, Y:30.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Buffer ID 76 (High)
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