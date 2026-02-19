; =============================================================================
; FILE: des_draw_popups.inc
; Purpose: Draw the already-loaded Buffers 80,81 to the screen
; =============================================================================

    ;------------------------------------------------
    ; Draw the Small Message Popup Window
    ;------------------------------------------------
Draw_Sm_Popup:
    ; Select Buffer 80.
    Select_Buffer_8bit 80

    ; Draw Buffer 80 at X:64, Y:164.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Buffer ID 80 (High)
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
    ; Draw the Top half
    ;--------------------------
    ; Select Buffer 81.
    Select_Buffer_8bit 81

    ; Draw Buffer 81 at X:64, Y:99.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Buffer ID 81 (High)
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
    ; Draw the Bottom half
    ;----------------------------
    ; Select Buffer 82.
    Select_Buffer_8bit 82

    ; Draw Buffer 81 at X:64, Y:155.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Buffer ID 82 (High)
    rst.lil $10
    ld a, 64            ; X Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 155            ; Y Coordinate
    rst.lil $10
    ld a, 0
    rst.lil $10
    
    ret

    ;------------------------------------------------
    ; Draw the Terminal Screen Popup Window
    ;------------------------------------------------
Draw_Terminal_Popup:

    ;-------------------------
    ; Draw the Top half
    ;--------------------------
    ; Select Buffer 83.
    Select_Buffer_8bit 83

    ; Draw Buffer 81 at X:64, Y:99.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Buffer ID 81 (High)
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
    ; Draw the Bottom half
    ;----------------------------
    ; Select Buffer 84.
    Select_Buffer_8bit 84

    ; Draw Buffer 4 at X:64, Y:155.
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3             ; Buffer ID 82 (High)
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
    