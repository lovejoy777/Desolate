Upload_Desol_Font:
    ld hl, FontProto
    ld b, 91               ; Load up to 'z'
    ld c, 32               ; Starting at Space
.char_loop:
    push bc
    push hl
    
    ld a, 23               ; VDU 23
    rst.lil $10
    ld a, c                ; Character code
    rst.lil $10
    
    ; Send 8 bytes of data
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    ld a, (hl)
    rst.lil $10
    inc hl
    
    pop hl
    ld de, 8               ; Move to next 8-byte chunk
    add hl, de
    
    pop bc
    inc c
    djnz .char_loop
    ret