;------------------------------------
; File: des_player_control.inc
;------------------------------------

;------------------------------
; Player Movement & Animation
;------------------------------
Movement_anim:
    ; Reset moving flag
    xor a
    ld (is_moving), a
    ld a, c

;---------------------------------------------
; Check Key presses
;---------------------------------------------
    ; Check UP
    ld b, KEY_UP_BYTE
    ld c, KEY_UP_BIT
    call Is_Key_Held_Matrix
    jp nz, set_up

    ; Check DOWN
    ld b, KEY_DOWN_BYTE
    ld c, KEY_DOWN_BIT
    call Is_Key_Held_Matrix
    jp nz, set_down

    ; Check Left
    ld b, KEY_LEFT_BYTE
    ld c, KEY_LEFT_BIT
    call Is_Key_Held_Matrix
    jp nz, set_left

    ; Check Right
    ld b, KEY_RIGHT_BYTE
    ld c, KEY_RIGHT_BIT
    call Is_Key_Held_Matrix
    jp nz, set_right

;------------------------------------------------
    

    ld hl, move_timeout
    ld a, (hl)
    or a
    jp z, logic_stopped   
    dec (hl)              
    ld a, 1
    ld (is_moving), a     
    jp apply_movement

logic_stopped:
    xor a
    ld (is_moving), a
    jp update_animation

;------------------------------
; Direction & Action Handlers
;------------------------------

set_down:
    ld a, 0
    ld (player_dir), a
    jp set_moving
set_up:
    ld a, 1
    ld (player_dir), a
    jp set_moving
set_right:
    ld a, 2
    ld (player_dir), a
    jp set_moving
set_left:
    ld a, 3
    ld (player_dir), a
    jp set_moving

set_moving:
    ld a, 1
    ld (is_moving), a
    ld a, 8
    ld (move_timeout), a

;-----------------------------------------------------------
; apply_movement (Preserved VDP/Coordinate Logic)
;-----------------------------------------------------------
apply_movement:
    ld a, (player_dir)
    or a
    jp z, check_down   
    cp 1
    jp z, check_up     
    cp 2
    jp z, check_right  
    cp 3
    jp z, check_left   

check_up:
    ld hl, 0
    ld a, (player_y)
    ld l, a
    ld a, (player_y + 1)
    ld h, a
    ld bc, 14 
    add hl, bc
    push hl
    pop de                 ; DE = Y + 20
    ld hl, 0
    ld a, (player_x)
    ld l, a
    ld a, (player_x + 1)
    ld h, a
    ld bc, 16
    add hl, bc             ; HL = X + 16

    call get_tile_at_coords
    cp $29                
    jp z, Found_North_Door
    cp $2A                
    jp z, Found_North_Door
    cp $01                
    jp nz, update_animation

    ld hl, (player_y)
    dec hl
    ld (player_y), hl
    jp update_animation

check_down:
    ld hl, 0
    ld a, (player_y)
    ld l, a
    ld a, (player_y + 1)
    ld h, a
    ld bc, 30             
    add hl, bc
    push hl
    pop de                 ; DE = Y + 30
    ld hl, 0
    ld a, (player_x)
    ld l, a
    ld a, (player_x + 1)
    ld h, a
    ld bc, 16             
    add hl, bc             ; HL = X + 16

    call get_tile_at_coords
    cp $18                
    jp z, Found_South_Door
    cp $19                
    jp z, Found_South_Door
    cp $01                
    jp nz, update_animation
    

    ld hl, (player_y)
    inc hl
    ld (player_y), hl
    jp update_animation

check_right:
    ld hl, 0
    ld a, (player_x)
    ld l, a
    ld a, (player_x + 1)
    ld h, a
    ld bc, 24
    add hl, bc
    push hl                ; Save New X
    ld hl, 0
    ld a, (player_y)
    ld l, a
    ld a, (player_y + 1)
    ld h, a
    ld bc, 24
    add hl, bc
    push hl
    pop de                 ; DE = Y + 24
    pop hl                 ; HL = X + 18

    call get_tile_at_coords
    cp $2C
    jp z, Found_East_Door
    cp $2D
    jp z, Found_East_Door
    cp $01                   ; floor tile
    jp nz, update_animation


    ld hl, (player_x)
    inc hl
    ld (player_x), hl
    jp update_animation

check_left:
    ld hl, 0
    ld a, (player_x)
    ld l, a
    ld a, (player_x + 1)
    ld h, a
    ld bc, 8
    add hl, bc
    push hl
    ld hl, 0
    ld a, (player_y)
    ld l, a
    ld a, (player_y + 1)
    ld h, a
    ld bc, 24
    add hl, bc
    push hl
    pop de                 ; DE = Y + 24
    pop hl                 ; HL = X + 2

    call get_tile_at_coords
    cp $2E
    jp z, Found_West_Door
    cp $2F
    jp z, Found_West_Door
    cp $01
    jp nz, update_animation

    ld hl, (player_x)
    dec hl
    ld (player_x), hl
    jp update_animation

;====================================================
; Update Annimation
;=====================================================

update_animation:
    Select_Sprite_8bit 0

    ld a, (is_moving)
    or a
    jp z, force_idle
    ld hl, anim_delay
    inc (hl)
    ld a, (hl)
    cp 6
    jp c, resolve_frame
    ld (hl), 0
    ld hl, anim_frame
    inc (hl)
    ld a, (hl)
    cp 4
    jp c, resolve_frame
    ld (hl), 0
    jp resolve_frame
force_idle:
    xor a
    ld (anim_frame), a
    ld (anim_delay), a
resolve_frame:
    ld a, (player_dir)
    or a
    jp z, map_down
    cp 1
    jp z, map_up
    cp 2
    jp z, map_right
    cp 3
    jp z, map_left
    ret

map_down:
    ld a, (anim_frame)
    or a
    jp z, use_bm_0
    cp 1
    jp z, use_bm_1
    cp 2
    jp z, use_bm_0
    jp use_bm_2
map_up:
    ld a, (anim_frame)
    or a
    jp z, use_bm_3
    cp 1
    jp z, use_bm_4
    cp 2
    jp z, use_bm_3
    jp use_bm_5
map_right:
    ld a, (anim_frame)
    or a
    jp z, use_bm_6
    cp 1
    jp z, use_bm_7
    cp 2
    jp z, use_bm_6
    jp use_bm_8
map_left:
    ld a, (anim_frame)
    or a
    jp z, use_bm_9
    cp 1
    jp z, use_bm_10
    cp 2
    jp z, use_bm_9
    jp use_bm_11

use_bm_0:
    ld a, 0
    jp send_vdu
use_bm_1:
    ld a, 1
    jp send_vdu
use_bm_2:
    ld a, 2
    jp send_vdu
use_bm_3:
    ld a, 3
    jp send_vdu
use_bm_4:
    ld a, 4
    jp send_vdu
use_bm_5:
    ld a, 5
    jp send_vdu
use_bm_6:
    ld a, 6
    jp send_vdu
use_bm_7:
    ld a, 7
    jp send_vdu
use_bm_8:
    ld a, 8
    jp send_vdu
use_bm_9:
    ld a, 9
    jp send_vdu
use_bm_10:
    ld a, 10
    jp send_vdu
use_bm_11:
    ld a, 11
    jp send_vdu

send_vdu:
    ld b, a
    ; swap frame
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 21       ; swap frame
    rst.lil $10
    ld a, b
    rst.lil $10

    ; load X for VDP Sprite Position
    ld hl, 0
    ld a, (player_x)
    ld l, a
    ld a, (player_x + 1)
    ld h, a

    ld bc, 64         
    add hl, bc

    ; Move sprite
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 13          ; Move sprite
    rst.lil $10
    
    ld a, l
    rst.lil $10       
    ld a, h
    rst.lil $10

    ; load Y for VDP Sprite Position
    ld hl, 0
    ld a, (player_y)
    ld l, a
    ld a, (player_y + 1)
    ld h, a
    ld bc, 83          
    add hl, bc
    ld a, l
    rst.lil $10        
    ld a, h
    rst.lil $10

    ; Show sprite