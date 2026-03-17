;=============================================
; SMALL ALIEN
;=============================================
Draw_Small_Alien:
    Select_Sprite_8bit smAlienSprite
    ; 1. ACTIVE CHECK: Only draw if alien_is_active is 1
    ld a, (small_alien_is_active) 
    or a
    jr nz, .draw_small_active       ; IF ACTIVE: Jump OVER the hide logic

.hide_small_alien:
    ; IF NOT ACTIVE: Hide the sprite and exit
    Hide_Sprite
    ret                       ; Return here so we don't draw anything
    
.draw_small_active:
    ; --- Animation logic ---
    ld hl, small_alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                
    jr nz, .skip_small_frame_swap
    
    ld (hl), 0          
    Load_Next_Frame

.skip_small_frame_swap:
    ; --- Move sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13            ; Command: move sprite
    rst.lil $10

    ; Send X (2 bytes)
    ld hl, (small_alien_x)
    ld bc, (game_screen_offset_x)           ; X Screen Offset
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 64)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    ; Send Y (2 bytes)
    ld hl, (small_alien_y)
    ld bc, (game_screen_offset_y)           ; Y Screen Offset
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 83)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    Show_Sprite
    ret

;=============================================
; BIG ALIEN
;=============================================
Draw_Big_Alien:
    Select_Sprite_8bit bigAlienSprite
    ; 1. ACTIVE CHECK: Only draw if alien_is_active is 1
    ld a, (big_alien_is_active) 
    or a
    jr nz, .draw_big_active       ; If 0, hide it

.hide_big_alien:
    Hide_Sprite
    ret 

.draw_big_active:
    ; --- Animation logic ---
    ld hl, big_alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                
    jr nz, .skip_big_frame_swap
    
    ld (hl), 0          
    Load_Next_Frame

.skip_big_frame_swap:
    ; Move sprite 
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13            ; Command: move sprite
    rst.lil $10

    ; ; Send X (2 bytes)
    ld hl, (big_alien_x)
    ld bc, (game_screen_offset_x)           ; X screen offset
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 83)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    ; ; Send Y (2 bytes)
    ld hl, (big_alien_y)
    ld bc, (game_screen_offset_y)           ; Y screen offset
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 83)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    Show_Sprite
    ret
    
    
    
    
