; =============================================================================
; FILE: des_draw_popups.inc
; Purpose: Draw the already-loaded Buffers 80,81,82,83,84 to the screen
; =============================================================================

    ;------------------------------------------------
    ; Draw the Small Message Popup Window
    ;------------------------------------------------
Draw_Sm_Popup:
    Select_Buffer_8bit 80
    ; Draw Buffer 80 at X:64, Y:164.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Command: Draw Buffer
    rst.lil $10
    ld a, 64            ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 164            ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ret

    ;------------------------------------------------
    ; Draw the Inventory and Infomation Popup Window
    ;------------------------------------------------
Draw_Inv_Popup:
    ;-------------------------
    ; Top half
    ;--------------------------
    Select_Buffer_8bit 81
    ; Draw Buffer 81 at X:64, Y:99.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Command: Draw Buffer
    rst.lil $10
    ld a, 64            ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 99            ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ;----------------------------
    ; Bottom half
    ;----------------------------
    Select_Buffer_8bit 82
    ; Draw Buffer 82 at X:64, Y:155.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Command: Draw Buffer
    rst.lil $10
    ld a, 64            ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 155           ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ret

    ;------------------------------------------------
    ; Draw the Terminal Screen Popup Window
    ;------------------------------------------------
Draw_Terminal_Popup:
    ;-------------------------
    ; Top half
    ;--------------------------
    Select_Buffer_8bit 83
    ; Draw Buffer 83 at X:64, Y:83.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Command: Draw Buffer
    rst.lil $10
    ld a, 64            ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 83            ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ;----------------------------
    ; Bottom half
    ;----------------------------
    Select_Buffer_8bit 84
    ; Draw Buffer 84 at X:64, Y:147.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Command: Draw Buffer
    rst.lil $10
    ld a, 64            ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 147           ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ret
    