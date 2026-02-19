; Keyboard Matrix Offsets (IX + Offset)
; Byte, Bit
KEY_UP_BYTE:    EQU $07
KEY_UP_BIT:     EQU 1
KEY_DOWN_BYTE:  EQU $05
KEY_DOWN_BIT:   EQU 1
KEY_LEFT_BYTE:  EQU $03
KEY_LEFT_BIT:   EQU 1
KEY_RIGHT_BYTE: EQU $0F
KEY_RIGHT_BIT:  EQU 1
KEY_SPACE_BYTE: EQU $0C
KEY_SPACE_BIT:  EQU 2
KEY_I_BYTE:     EQU $04
KEY_I_BIT:      EQU 5
KEY_M_BYTE:     EQU $0C
KEY_M_BIT:      EQU 5
KEY_ENTER_BYTE: EQU $09
KEY_ENTER_BIT:  EQU 1
KEY_ESC_BYTE:   EQU $0E
KEY_ESC_BIT:    EQU 0
KEY_0_BYTE:     EQU $04
KEY_0_BIT:      EQU 7
KEY_1_BYTE:     EQU $6
KEY_1_BIT:      EQU 0
KEY_2_BYTE:     EQU $06
KEY_2_BIT:      EQU 1
KEY_3_BYTE:     EQU $02
KEY_3_BIT:      EQU 1
KEY_4_BYTE:     EQU $02
KEY_4_BIT:      EQU 2
KEY_5_BYTE:     EQU $02
KEY_5_BIT:      EQU 3
KEY_6_BYTE:     EQU $06
KEY_6_BIT:      EQU 4
KEY_7_BYTE:     EQU $04
KEY_7_BIT:      EQU 4
KEY_8_BYTE:     EQU $02
KEY_8_BIT:      EQU 5
KEY_9_BYTE:     EQU $04
KEY_9_BIT:      EQU 6



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
    
    ; Workaround for 'add ix, hl'
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