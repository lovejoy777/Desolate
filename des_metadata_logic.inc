;-----------------------------------------------------------
; File: des_metadata_logic.inc
; Logic: Decode Room Metadata (49 bytes)
;-----------------------------------------------------------
current_room_metadata:    
    .ds 96             ; 49 byte buffer
Decode_Room_Metadata:
    ; 1. Find the 24-bit pointer from Metadata_Pointer_Table
    ld hl, 0
    ld a, (current_room_id)
    ld l, a
    ld d, h
    ld e, l
    add hl, hl              ; ID * 2
    add hl, de              ; ID * 3
    ld de, Metadata_Pointer_Table
    add hl, de
    ld hl, (hl)             ; HL = Compressed metadata address

    ; 2. Setup for Decompression
    ld de, current_room_metadata
    ld b, 49                ; Decode exactly 49 bytes

meta_decode_main:
    ld a, (hl)            
    cp $FF                  ; Repeater?
    jr nz, meta_literal_byte
    
    ; --- Handle Repeater ---
    inc hl
    ld c, (hl)              ; C = Count
    inc hl
    ld a, (hl)              ; A = Value
meta_repeat_loop:
    ld (de), a            
    inc de
    dec b                   ; Decrease Master Counter
    jr z, meta_finished     ; Stop if we reach 49
    dec c                   ; Decrease Repeat Counter
    jr nz, meta_repeat_loop
    
    inc hl
    jr meta_decode_main

meta_literal_byte:
    ld (de), a
    inc de
    inc hl
    djnz meta_decode_main   ; standard 8-bit djnz for the 49 bytes

meta_finished:
    ret

