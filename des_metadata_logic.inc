;-----------------------------------------------------------
; File: des_metadata_logic.inc
; Logic: Decode Room Metadata (49 bytes)
;----------------------------------------------------------- 
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
    cp $FF                  ; Repeater Flag
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
    call Initialize_Alien_Health
    ret

Initialize_Alien_Health:
    ; --- 1. Safe Address Math ---
    ld a, (current_room_id)
    ld hl, Alien_Kill_Table
    ld de, 0                ; CLEAR FULL 24-BIT REGISTER
    ld e, a
    add hl, de              ; HL points to this room's HP in the table
    
    ld a, (hl)              ; Get the HP from the table
    cp 0                    ; IS IT DEAD?
    jr z, .hide_and_exit    ; If 0, skip everything

    ; --- 2. It's not 0, so the alien is alive. Store that HP locally ---
    ; (We don't know yet if it's a big or small alien, so we'll 
    ;  save 'A' for a moment or just check metadata first)
    push af                 ; Save the Table HP

    ; --- 3. Check Metadata for Type ---
    ld hl, current_room_metadata
    ld de, $2E              
    add hl, de
    ld a, (hl)               
    
    cp alien_type_small
    jr z, .init_small
    cp alien_type_big
    jr z, .init_big
    
    pop af                  ; Clean up stack if no alien found
    jr .hide_and_exit

.init_small:
    pop af                  ; Get Table HP back (3, 2, or 1)
    ld (small_alien_hp), a
    ld a, 1
    ld (small_alien_is_active), a
    ret

.init_big:
    pop af                  ; Get Table HP back (10, 5, etc)
    ld (big_alien_hp), a
    ld a, 1
    ld (big_alien_is_active), a
    ret

.hide_and_exit:
    ; Hide Alien Sprites
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite
    ret