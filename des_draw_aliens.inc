;=============================================
; SMALL ALIEN
;=============================================
Draw_Small_Alien:
    ; 1. ACTIVE CHECK: Only draw if alien_is_active is 1
    ld a, (sm_alien_is_active) 
    or a
    jr nz, .draw_active       ; IF ACTIVE: Jump OVER the hide logic

.hide_small_alien:
    ; IF NOT ACTIVE: Hide the sprite and exit
    Select_Sprite_8bit 1
    Hide_Sprite
    ret                       ; Return here so we don't draw anything
    
.draw_active:                 
    Select_Sprite_8bit 1
    
    ; --- Animation logic ---
    ld hl, alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                
    jr nz, .skip_frame_swap
    
    ld (hl), 0          
    Load_Next_Frame

.skip_frame_swap:
    ; --- Move sprite (VDU 23, 27, 13) ---
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13            
    rst.lil $10

    ; Send X (2 bytes)
    ld hl, (small_alien_x)
    ld bc, 64           ; Screen Offset
    add hl, bc
    ld a, l             
    rst.lil $10
    ld a, h             
    rst.lil $10

    ; Send Y (2 bytes)
    ld hl, (small_alien_y)
    ld bc, 83           ; Screen Offset
    add hl, bc
    ld a, l             
    rst.lil $10
    ld a, h             
    rst.lil $10

    Select_Sprite_8bit 1
    Show_Sprite
    ret

;=============================================
; BIG ALIEN
;=============================================
Draw_Big_Alien:
    ; 1. ACTIVE CHECK: Only draw if alien_is_active is 1
    ld a, (big_alien_is_active) 
    or a
    jr nz, .draw_big_active       ; If 0, hide it

.hide_big_alien:
    Select_Sprite_8bit 2
    Hide_Sprite
    ret 

.draw_big_active:                 
    Select_Sprite_8bit 2
    
    ; --- Animation logic ---
    ld hl, big_alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                
    jr nz, .skip_big_frame_swap
    
    ld (hl), 0          
    Load_Next_Frame
    
.init_big_spawn:

    Select_Sprite_8bit 2
    
    ; --- Slow down logic ---
    ld hl, big_alien_anim_timer
    inc (hl)
    ld a, (hl)
    cp 5                ; <--- number to adjust speed (Higher = Slower)
    jr nz, .skip_big_frame_swap
    
    ld (hl), 0           ; Reset big_alien_anim_timer timer

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
    ld bc, 64           ; X screen offset if applicable
    add hl, bc
    ld a, l             ; Send Low Byte (e.g., 180)
    rst.lil $10
    ld a, h             ; Send High Byte (e.g., 0)
    rst.lil $10

    ; --- Y Coordinate (MUST SEND 2 BYTES) ---
    ld hl, (big_alien_y)
    ld bc, 83           ; Y screen offset if applicable
    add hl, bc
    ld a, l             ; Send Low Byte
    rst.lil $10
    ld a, h             ; Send High Byte
    rst.lil $10

    Select_Sprite_8bit 2
    Show_Sprite
    ret
    
    
    
    
