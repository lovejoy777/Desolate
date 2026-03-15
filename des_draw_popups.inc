; =============================================================================
; FILE: des_draw_popups.inc
; Purpose: Draw the already-loaded Buffers 80,81,82,83,84 to the screen
; =============================================================================

    ;------------------------------------------------
    ; Draw the Small Message Popup Window
    ;------------------------------------------------
Draw_Sm_Popup:
    ;Select_Buffer_8bit 80
    push de
    ld de, 80
    SELECT_BUFFER_DE
    pop de
    ; Position cursor inside the window for row 1
    TABTO 9, 22
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
    ;Select_Buffer_8bit 81
    push de
    ld de, 81
    SELECT_BUFFER_DE
    pop de

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
    push de
    ld de, 82
    SELECT_BUFFER_DE
    pop de
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
    ; Position cursor inside the window for row 1
    ;TABTO 11, 13
    ; Draw Buffer 80 at X:64, Y:164.
    ;-------------------------
    ; Top half
    ;--------------------------
    push de
    ld de, 83
    SELECT_BUFFER_DE
    pop de
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
    push de
    ld de, 84
    SELECT_BUFFER_DE
    pop de
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
    