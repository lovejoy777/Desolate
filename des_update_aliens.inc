;======================
; SMALL ALIEN
;======================
Update_Small_Alien:
    ; 1. Only move if in the same room
    ld a, (sm_alien_is_active)
    or a
    ret z                   ; Exit if no alien in this room

    ld a, (alien_state)
    cp 0
    jp z, .state_choosing
    cp 1
    jp z, .state_moving
    cp 2
    jp z, .state_waiting
    ret

.state_choosing:
    ld a, r             ; Use refresh register for randomness
    and 3               ; Mask 0-3 for direction
    ld (small_alien_dir), a
    ld a, r
    and 31              ; Mask 0-31
    add a, 10           ; Move at least 10 pixels
    ld (alien_move_count), a
    ld a, 1
    ld (alien_state), a ; Change to Moving state
    ret

.state_moving:
    ld hl, alien_move_timer
    inc (hl)
    ld a, (hl)
    cp 4                ; Adjust speed here
    ret nz
    ld (hl), 0
    
    ld a, (alien_move_count)
    or a
    jp z, .finished_moving
    dec a
    ld (alien_move_count), a

    ld a, (small_alien_dir)
    cp 0
    jp z, .move_s
    cp 1
    jp z, .move_n
    cp 2
    jp z, .move_e
    cp 3
    jp z, .move_w
    ret

.move_s:
    ; --- Step 1: Check_down) ---
    ld hl, 0
    ld a, (small_alien_x)
    ld l, a
    ld bc, 8              ; Alien is 16x16, so +8 is the middle
    add hl, bc            ; HL = X + 8
    push hl               ; Save X for the call

    ld hl, 0
    ld a, (small_alien_y)
    ld l, a
    ld bc, 16             ; Check 0 pixel below the feet (16)
    add hl, bc
    push hl
    pop de                ; DE = Y + 17
    pop hl                ; HL = X + 8

    call get_tile_at_coords
    cp $01                ; Is it floor?
    jp nz, .hit_wall      ; If NOT floor, it's a wall

    ; --- Step 2: Apply (Exactly like Player's apply_movement) ---
    ld a, (small_alien_y)
    inc a
    ld (small_alien_y), a
    ret

.move_n:
    ; --- Step 1: Check (Exactly like Player's check_up) ---
    ld hl, 0
    ld a, (small_alien_x)
    ld l, a
    ld bc, 8
    add hl, bc
    push hl

    ld hl, 0
    ld a, (small_alien_y)
    ld l, a
    ld bc, 0             ; Check 0 pixel above the head
    add hl, bc
    push hl
    pop de                ; DE = Y - 1
    pop hl                ; HL = X + 8

    call get_tile_at_coords
    cp $01
    jp nz, .hit_wall

    ; --- Step 2: Apply ---
    ld a, (small_alien_y)
    dec a
    ld (small_alien_y), a
    ret

.move_e:
    ; --- Step 1: Check (Exactly like Player's check_right) ---
    ld hl, 0
    ld a, (small_alien_x)
    ld l, a
    ld bc, 16             ; Check 0 pixel past right side (16)
    add hl, bc
    push hl               ; Save New X

    ld hl, 0
    ld a, (small_alien_y)
    ld l, a
    ld bc, 8              ; Middle of alien Y
    add hl, bc
    push hl
    pop de                ; DE = Y + 8
    pop hl                ; HL = X + 17

    call get_tile_at_coords
    cp $01
    jp nz, .hit_wall

    ; --- Step 2: Apply ---
    ld a, (small_alien_x)
    inc a
    ld (small_alien_x), a
    ret

.move_w:
    ; --- Step 1: Check (Exactly like Player's check_left) ---
    ld hl, 0
    ld a, (small_alien_x)
    ld l, a
    ld bc, 0             ; Check 0 pixel past left side
    add hl, bc
    push hl               ; Save New X

    ld hl, 0
    ld a, (small_alien_y)
    ld l, a
    ld bc, 8              ; Middle of alien Y
    add hl, bc
    push hl
    pop de                ; DE = Y + 8
    pop hl                ; HL = X - 1

    call get_tile_at_coords
    cp $01
    jp nz, .hit_wall

    ; --- Step 2: Apply ---
    ld a, (small_alien_x)
    dec a
    ld (small_alien_x), a
    ret

.hit_wall:
    ; If any "check" fails, we come here to reset the alien's brain
    xor a
    ld (alien_state), a   ; Reset to state_choosing
    ret

.finished_moving:
    ; Alien reached its move_count, now it waits
    ld a, 2               ; Change to state_waiting
    ld (alien_state), a
    ld a, 60              ; Wait for 60 frames (approx 1 second)
    ld (alien_wait_timer), a
    ret

.state_waiting:
    ld hl, alien_wait_timer
    dec (hl)
    ret nz                ; Keep waiting until timer is 0
    xor a
    ld (alien_state), a   ; Back to choosing a new direction
    ret

;======================
; BIG ALIEN
;======================
Update_Big_Alien:
    ; 1. Only move if in the same room
    ld a, (big_alien_is_active)
    or a
    ret z                   ; Exit if no big alien in this room

    ld a, (big_alien_state)
    cp 0
    jp z, .big_state_choosing
    cp 1
    jp z, .big_state_moving
    cp 2
    jp z, .big_state_waiting
    ret

.big_state_choosing:
    ld a, r             ; Use refresh register for randomness
    and 3               ; Mask 0-3 for direction
    ld (big_alien_dir), a
    ld a, r
    and 31              ; Mask 0-31
    add a, 10           ; Move at least 10 pixels
    ld (big_alien_move_count), a
    ld a, 1
    ld (big_alien_state), a ; Change to Moving state
    ret

.big_state_moving:
    ld hl, big_alien_move_timer
    inc (hl)
    ld a, (hl)
    cp 4                ; Adjust speed here
    ret nz
    ld (hl), 0
    
    ld a, (big_alien_move_count)
    or a
    jp z, .big_finished_moving
    dec a
    ld (big_alien_move_count), a

    ld a, (big_alien_dir)
    cp 0
    jp z, .big_move_s
    cp 1
    jp z, .big_move_n
    cp 2
    jp z, .big_move_e
    cp 3
    jp z, .big_move_w
    ret

.big_move_s:
    ; --- Step 1: Check (Exactly like Player's check_down) ---
    ld hl, 0
    ld a, (big_alien_x)
    ld l, a
    ld bc, 8              ; Alien is 16x32, so +8 is the x middle
    add hl, bc            ; HL = X + 8
    push hl               ; Save X for the call

    ld hl, 0
    ld a, (big_alien_y)
    ld l, a
    ld bc, 32             ; Check 0 pixel below the feet (32)
    add hl, bc
    push hl
    pop de                ; DE = Y + 32
    pop hl                ; HL = X + 8

    call get_tile_at_coords
    cp $01                ; Is it floor?
    jp nz, .big_hit_wall      ; If NOT floor, it's a wall

    ; --- Step 2: Apply (Exactly like Player's apply_movement) ---
    ld a, (big_alien_y)
    inc a
    ld (big_alien_y), a
    ret

.big_move_n:
    ; --- Step 1: Check (Exactly like Player's check_up) ---
    ld hl, 0
    ld a, (big_alien_x)
    ld l, a
    ld bc, 8
    add hl, bc
    push hl

    ld hl, 0
    ld a, (big_alien_y)
    ld l, a
    ld bc, 0             ; Check 0 pixel above the head
    add hl, bc
    push hl
    pop de                ; DE = Y - 1
    pop hl                ; HL = X + 8

    call get_tile_at_coords
    cp $01
    jp nz, .big_hit_wall

    ; --- Step 2: Apply ---
    ld a, (big_alien_y)
    dec a
    ld (big_alien_y), a
    ret

.big_move_e:
    ; --- Step 1: Check (Exactly like Player's check_right) ---
    ld hl, 0
    ld a, (big_alien_x)
    ld l, a
    ld bc, 16             ; Check right side (16)
    add hl, bc
    push hl               ; Save New X

    ld hl, 0
    ld a, (big_alien_y)
    ld l, a
    ld bc, 16              ; Middle of big alien Y
    add hl, bc
    push hl
    pop de                ; DE = Y + 8
    pop hl                ; HL = X + 17

    call get_tile_at_coords
    cp $01
    jp nz, .big_hit_wall

    ; --- Step 2: Apply ---
    ld a, (big_alien_x)
    inc a
    ld (big_alien_x), a
    ret

.big_move_w:
    ; --- Step 1: Check (Exactly like Player's check_left) ---
    ld hl, 0
    ld a, (big_alien_x)
    ld l, a
    ld bc, 0             ; Check 0 pixel past left side
    add hl, bc
    push hl               ; Save New X

    ld hl, 0
    ld a, (big_alien_y)
    ld l, a
    ld bc, 16
    add hl, bc
    push hl
    pop de                ; DE = Y + 8
    pop hl                ; HL = X - 1

    call get_tile_at_coords
    cp $01
    jp nz, .big_hit_wall

    ; --- Step 2: Apply ---
    ld a, (big_alien_x)
    dec a
    ld (big_alien_x), a
    ret

.big_hit_wall:
    ; If any "check" fails, we come here to reset the big alien's brain
    xor a
    ld (big_alien_state), a   ; Reset to state_choosing
    ret

.big_finished_moving:
    ; Big Alien reached its move_count, now it waits
    ld a, 2               ; Change to state_waiting
    ld (big_alien_state), a
    ld a, 60              ; Wait for 60 frames (approx 1 second)
    ld (big_alien_wait_timer), a
    ret

.big_state_waiting:
    ld hl, big_alien_wait_timer
    dec (hl)
    ret nz                ; Keep waiting until timer is 0
    xor a
    ld (big_alien_state), a   ; Back to choosing a new direction
    ret

;-----------------------------------------------------------
; Example Usage: Loading Alien from Metadata
;-----------------------------------------------------------
Load_Alien_From_Meta:
    ld ix, current_room_metadata
    ld a, (ix + 40)       ; Byte 40 = Alien Type
    cp 0
    jr z, .no_alien
    
    ld (sm_alien_is_active), a
    ld a, (ix + 41)       ; Byte 41 = Spawn X
    ld (small_alien_x), a
    ld a, (ix + 42)       ; Byte 42 = Spawn Y
    ld (small_alien_y), a
    ret
.no_alien:
    xor a
    ld (sm_alien_is_active), a
    ret