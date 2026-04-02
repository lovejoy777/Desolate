;---------------------------------
; File: des_shooting.inc
; Logic: shooting helper
;----------------------------------

Shoot:
    ; Check if player has the gun
    ld a, (shoot_is_active) 
    cp $01
    ret nz
    
    ; Don't allow multiple bullets if one is already flying
    ld a, (bullet_is_active)   
    cp $01
    ret z

    ; laser sfx
    call playLaser
    ; laser sfx
    ;call playZap
    ; Scifi Door sfx
    ;call playDoor
    ; Footsteps sfx
    ;call playFootsteps

    ld a, (player_dir)
    ld (bullet_direction), a
    
    cp $00
    jp z, .bullet_south
    cp $01
    jp z, .bullet_north
    cp $02
    jp z, .bullet_east
    cp $03
    jp z, .bullet_west

    ret
;-------------------
; Spawn Bullet
;-------------------
.bullet_south:
    ld hl, (player_x)
    ld bc, 0
    ld bc, 8
    add hl, bc
    ld (bullet_x), hl

    ld hl, (player_y)
    ld bc, 0
    ld bc, 22
    add hl, bc 
    ld (bullet_y), hl 

    ld a, $01
    ld (bullet_is_active), a   ; Start the bullet update loop

    ; Buffer ID for frame
    ld a, 40
    ld (bullet_frame), a

    ret

.bullet_north:
    ld hl, (player_x)
    ld bc, 8
    add hl, bc
    ld (bullet_x), hl

    ld hl, (player_y)
    ld bc, 10
    sbc hl, bc              ; Subtract from X
    ld (bullet_y), hl

    ld a, $01
    ld (bullet_is_active), a   ; Start the bullet update 
    
    ; Buffer ID for frame
    ld a, 40
    ld (bullet_frame), a
    
    ret

.bullet_east:
    ld hl, (player_x)
    ld bc, 0
    ld bc, 16       
    add hl, bc
    ld (bullet_x), hl 

    ld hl, (player_y)
    ld bc, 12
    add hl, bc
    ld (bullet_y), hl

    ld a, $01
    ld (bullet_is_active), a   ; Start the bullet update loop

    ; Buffer ID for frame
    ld a, 41
    ld (bullet_frame), a

    ret

.bullet_west:
    ld hl, (player_x)
    ;ld bc, 0
    ;ld bc, 2     
    ;sbc hl, bc 
    ld (bullet_x), hl

    ld hl, (player_y)
    ld bc, 12
    add hl, bc
    ld (bullet_y), hl

    ld a, $01
    ld (bullet_is_active), a   ; Start the bullet update 

    ; Buffer ID for frame
    ld a, 41
    ld (bullet_frame), a

    ret

Update_Bullet:
    ld a, (bullet_is_active)
    or a
    ; If Bullet is not Active exit
    ret z

    ; 1. MOVE THE BULLET
    ld a, (bullet_direction)
    cp 0
    jr z, .move_bullet_s
    cp 1
    jr z, .move_bullet_n
    cp 2
    jr z, .move_bullet_e
    cp 3
    jr z, .move_bullet_w
    ret

.move_bullet_s:
    ld hl, (bullet_y)
    ld bc, (bullet_speed)
    add hl, bc
    ld (bullet_y), hl
    jr .check_collisions
.move_bullet_n:
    ld hl, (bullet_y)
    ld bc, (bullet_speed)     
    sbc hl, bc
    ld (bullet_y), hl
    jr .check_collisions
.move_bullet_e:
    ld hl, (bullet_x)
    ld bc, (bullet_speed)
    add hl, bc
    ld (bullet_x), hl
    jr .check_collisions
.move_bullet_w:
    ld hl, (bullet_x)
    ld bc, (bullet_speed)     
    sbc hl, bc
    ld (bullet_x), hl
    jr .check_collisions

.check_collisions:
    ld a, (bullet_direction)
    cp $00
    jp z, .hit_south
    cp $01
    jp z, .hit_north
    cp $02
    jp z, .hit_east
    cp $03
    jp z, .hit_west
    ret

.hit_south:
    ld hl, (bullet_x)
    ld de, 0
    ld de, 8              ; Center X
    add hl, de

    push hl               ; Save X

    ld hl, (bullet_y)
    ld de, 0
    ld de, 12             ; Front tip Y
    add hl, de
    ex de, hl             ; DE = Y tip

    pop hl                ; HL = X center
    jp .bullet_collision_check

.hit_north:
    ld hl, (bullet_x)
    ld de, 0
    ld de, 8              ; Center X
    add hl, de

    push hl

    ld hl, (bullet_y)
    ld de, 0
    ld de, 4              ; Front tip Y
    add hl, de
    ex de, hl

    pop hl
    jp .bullet_collision_check

.hit_east:
    ld hl, (bullet_x)
    ld de, 0
    ld de, 12             ; Front tip X
    add hl, de

    push hl

    ld hl, (bullet_y)
    ld de, 0
    ld de, 8             ; Center Y
    add hl, de
    ex de, hl

    pop hl
    jp .bullet_collision_check

.hit_west:
    ld hl, (bullet_x)     ; Front tip X is 0
    ld de, 0
    ld de, 4              ; Center X
    add hl, de

    push hl

    ld hl, (bullet_y)
    ld de, 0
    ld de, 8             ; Center Y
    add hl, de
    ex de, hl

    pop hl
    jp .bullet_collision_check

.bullet_collision_check:
    ; --- Oject Collision check ---
    call get_tile_at_coords
    cp $01
    jr nz, .hit_something

    ; --- Bullet & alien collision (Using your Midpoint Logic) ---
    call Bullet_Alien_Collision_Logic
    ; If Carry is set, we hit an alien and damaged it
    jr c, .hit_something  
    jp .no_hits 

.no_hits:
    ; Draw bullet to the screen
    call send_bul_vdu 
    ret

.hit_something:
    xor a
    ld (bullet_is_active), a

    Select_Sprite_8bit bulletSprite
    Hide_Sprite            
    ret

Bullet_Alien_Collision_Logic:
;----------------------------
; Check Bullet & Small Alien
;----------------------------
     ;----------------------------
     ; Check Bullet & Small Alien
     ;----------------------------
.check_small_bullet:
    ld a, (small_alien_is_active)
    or a
    jp z, .check_big_bullet

    ; X-Axis Distance (Bullet Mid X vs Small Alien Mid X)
    ld a, (bullet_x_mid)        ; Use 8-bit low byte for comparison
    ld b, a
    ld a, (small_alien_x_mid)
    sub b
    jp p, .skip_neg_bx1
    neg
.skip_neg_bx1:
    cp 8                   ; Hitbox tolerance
    jp nc, .check_big_bullet
    ; Y-Axis Distance
    ld a, (bullet_y_mid)
    ld b, a
    ld a, (small_alien_y_mid)
    sub b
    jp p, .skip_neg_by1
    neg
.skip_neg_by1:
    cp 8                     
    jp nc, .check_big_bullet

    ;--------------------
    ; HIT SMALL Alien
    ;--------------------
    ld a, (small_alien_hp)
    dec a                    ; Decrease HP
    ld (small_alien_hp), a
    
    push af                 ; Save the new HP (e.g., 2)

    ld a, (current_room_id)
    ld hl, Alien_Kill_Table
    ld de, 0
    ld e, a
    add hl, de
    pop af                  ; Restore the new HP
    ld (hl), a              ; Write 2 (or 1, or 0) to the table
    jp z, On_Small_Alien_Death ; If it reached 0, hide him

    ret
 
 ;--------------------------
 ; Check Bullet & Big Alien
 ;--------------------------
.check_big_bullet:
    ld a, (big_alien_is_active)
    or a
    ret z

    ; X-Axis Distance (Bullet MidX vs Big Alien Mid X)
    ld a, (bullet_x_mid)
    ld b, a
    ld a, (big_alien_x_mid)
    sub b
    jp p, .skip_neg_bx2
    neg
.skip_neg_bx2:
    cp 8
    ret nc
    ; Y-Axis Distance
    ld a, (bullet_y_mid)
    ld b, a
    ld a, (big_alien_y_mid)
    sub b 
    jp p, .skip_neg_by2
    neg
.skip_neg_by2:
    cp 16
    ret nc

    ;-------------------
    ; HIT BIG Alien
    ;-------------------
    ld a, (big_alien_hp)
    dec a
    ld (big_alien_hp), a

    push af             ; Save the new HP (e.g., 2)

    ld a, (current_room_id)
    ld hl, Alien_Kill_Table
    ld de, 0
    ld e, a
    add hl, de
    pop af                  ; Restore the new HP
    ld (hl), a              ; Write 2 (or 1, or 0) to the table
    jp z, On_Big_Alien_Death ; If it reached 0, hide him, he's dead

    ret



;-----------------------------------------------------------
; Alien Death Handlers
; These are called when HP reaches 0
;-----------------------------------------------------------
On_Small_Alien_Death:
    xor a
    ld (small_alien_is_active), a ; 1. Stop it from moving/drawing
    
    ; Mark this room as "Cleared" in the Global Persistence Table
    ld a, (current_room_id)
    ld hl, Alien_Kill_Table
    ld d, 0
    ld e, a
    add hl, de
    ld (hl), 0                   ; Store 0 (Dead) at this Room ID index
    
    ld a, (alien_amount_killed)
    inc a
    ld (alien_amount_killed), a

    ; Hide the sprite immediately
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite

    ret

On_Big_Alien_Death:
    xor a
    ld (big_alien_is_active), a   ; 1. Stop it from moving/drawing
    
    ; Mark this room as "Cleared" in the Global Persistence Table
    ld a, (current_room_id)
    ld hl, Alien_Kill_Table
    ld d, 0
    ld e, a
    add hl, de
    ld (hl), 0                   ; Store 0 (Dead) at this Room ID index
    
    ld a, (alien_amount_killed)
    inc a
    ld (alien_amount_killed), a

    ; Hide the sprite immediately
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    ret

;----------------------------------------------------
; Swap Bullet Frame and Move Bullet on the Screen
;----------------------------------------------------
send_bul_vdu:
    Select_Sprite_8bit bulletSprite
    
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 21            ; Command: swap frame
    rst.lil $10
    ld a, (bullet_frame)
    rst.lil $10
    ; Move sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13            ; Command: Move sprite
    rst.lil $10
    ; load X for Sprite Position
    ld hl, (bullet_x)
    ld bc, (game_screen_offset_x)
    add hl, bc
    ld a, l
    rst.lil $10   
    ld a, h
    rst.lil $10 
    ; load Y for Sprite Position
    ld hl, (bullet_y)
    ld bc, (game_screen_offset_y)
    add hl, bc
    ld a, l
    rst.lil $10 
    ld a, h
    rst.lil $10   

    Show_Sprite

    ret

