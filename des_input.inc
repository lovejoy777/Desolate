;============================================
; Keyboard Matrix Offsets (IX + Offset)
;============================================
KEY_UP_BYTE:    EQU $07    ; Up Cursor
KEY_UP_BIT:     EQU 1
KEY_DOWN_BYTE:  EQU $05    ; Down Cursor
KEY_DOWN_BIT:   EQU 1
KEY_LEFT_BYTE:  EQU $03    ; Left Cursor
KEY_LEFT_BIT:   EQU 1
KEY_RIGHT_BYTE: EQU $0F    ; Right Cursor
KEY_RIGHT_BIT:  EQU 1
KEY_SPACE_BYTE: EQU $0C    ; Look or Shoot
KEY_SPACE_BIT:  EQU 2
KEY_Z_BYTE:     EQU $0C    ; Look Mode
KEY_Z_BIT:      EQU 1
KEY_X_BYTE:     EQU $08    ; Shoot Mode
KEY_X_BIT:      EQU 2
KEY_C_BYTE:     EQU $0A    ; Close Popup Windows
KEY_C_BIT:      EQU 2
KEY_M_BYTE:     EQU $0C    ; Menu
KEY_M_BIT:      EQU 5
KEY_I_BYTE:     EQU $04    ; Inventory
KEY_I_BIT:      EQU 5
KEY_ENTER_BYTE: EQU $09    ; Enter
KEY_ENTER_BIT:  EQU 1
KEY_ESC_BYTE:   EQU $0E    ; Quit to Mos
KEY_ESC_BIT:    EQU 0
  
;----------------------------------------------------------
; Is_Key_Held_Matrix
; Input: B = Byte Offset, C = Bit Number
; Output: NZ if pressed (1), Z if released (0)
;----------------------------------------------------------
Is_Key_Held_Matrix:
    push ix
    push bc
    
    ld a, $1E
    rst.lil $08             ; IX = Pointer to keyboard matrix
    
    push ix
    pop hl                  ; HL = Start of matrix
    ld de, 0
    ld e, b                 ; DE = Offset (the byte index)
    add hl, de              ; HL = Start + Offset
    push hl
    pop ix                  ; IX = Correct Byte Address
    
    ld a, (ix + 0)          ; Load the byte from the matrix
    pop bc                  ; Restore Bit number into C
    
    ld b, a                 ; Store matrix byte into B
    ld a, 1
    inc c                   ; Increment for loop logic
_mask_loop:
    dec c
    jr z, _do_test
    add a, a                ; Shift bit left to create mask
    jr _mask_loop
_do_test:
    and b                   ; Isolate the bit (1 if pressed)
    pop ix
    ret