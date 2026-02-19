;=============================================
; SMALL ALIEN
;=============================================
Draw_Small_Alien:
    ; 1. ACTIVE CHECK: Only draw if alien_is_active is 1
    ld a, (sm_alien_is_active) ; You may need to rename 'small_alien_active' in des_main.asm to match this
    or a
    jr z, .hide_small_alien       ; If 0, hide it

    jr .init_spawn

.hide_small_alien:
    ; If not in the room, hide the sprite and exit
    Select_Sprite_8bit 1
    Hide_Sprite

    ret
    
.init_spawn:

    ; Select Sprite 1.
    Select_Sprite_8bit 1
    
    ; --- Slow down logic ---
    ld hl, alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                ; <--- number to adjust speed (Higher = Slower)
    jr nz, .skip_frame_swap
    
    ld (hl), 0           ; Reset alien_anim_timer timer

    ; Next frame
    Load_Next_Frame

.skip_frame_swap:

    ; Move sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13            ; Command 3: move sprite
    rst.lil $10

    ; --- X Coordinate (MUST SEND 2 BYTES) ---
    ld hl, (small_alien_x)
    ld bc, 64           ; Match the Player's X offset if applicable
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 180)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    ; --- Y Coordinate (MUST SEND 2 BYTES) ---
    ld hl, (small_alien_y)
    ld bc, 83           ; THE VIEWPORT OFFSET
    add hl, bc
    ld a, l             ; Send Low Byte
    rst.lil $10
    ld a, h             ; Send High Byte
    rst.lil $10

    ; Show sprite
    Show_Sprite
    ; Update gpu
    Update_GPU
    ret

;=============================================
; BIG ALIEN
;=============================================
Draw_Big_Alien:
    ; 1. ACTIVE CHECK: Only draw if alien_is_active is 1
    ld a, (big_alien_is_active) ; You may need to rename 'big_alien_active' in des_main.asm to match this
    or a
    jr z, .hide_big_alien       ; If 0, hide it

    jr .init_big_spawn

.hide_big_alien:
    ; If not in the room, hide the sprite and exit
    Select_Sprite_8bit 2
    Hide_Sprite

    ret
    
.init_big_spawn:

    ; Select Sprite 2.
    Select_Sprite_8bit 2
    
    ; --- Slow down logic ---
    ld hl, big_alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                ; <--- number to adjust speed (Higher = Slower)
    jr nz, .skip_big_frame_swap
    
    ld (hl), 0           ; Reset big_alien_anim_timer timer

    ; Next frame
    Load_Next_Frame

.skip_big_frame_swap:

    ; Move sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13            ; Command 3: move sprite
    rst.lil $10

    ; --- X Coordinate (MUST SEND 2 BYTES) ---
    ld hl, (big_alien_x)
    ld bc, 64           ; Match the Player's X offset if applicable
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 180)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    ; --- Y Coordinate (MUST SEND 2 BYTES) ---
    ld hl, (big_alien_y)
    ld bc, 83           ; THE VIEWPORT OFFSET
    add hl, bc
    ld a, l             ; Send Low Byte
    rst.lil $10
    ld a, h             ; Send High Byte
    rst.lil $10

    ; Show sprite
    Show_Sprite
    ; Update gpu
    Update_GPU
    ret
    
    
    
    
