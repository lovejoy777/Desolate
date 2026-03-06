; =============================================================================
; FILE: Desolate's des_main.asm
; TARGET: Agon Light 2 & ez80asm Assembler.
; =============================================================================

    .include "initial_macros.inc"
    .include "des_sound_macros.inc"
    .include "des_vdp_macros.inc"
    

    assume adl=1             ; use 24-bit addressing mode
    .org $40000              ; set origin to $40000
    jp start_here            ; jump to start_here
    .align 64                ; align to 64 bytes
    .db "MOS", 0, 1          ; MOS header

;--------------------
; Includes
;--------------------
    .include "des_input.inc"
    .include "des_load_font.inc"
    .include "des_font.inc"
    .include "des_strings.inc"
    .include "des_main_subs.inc"
    .include "strings24.asm"

    .include "des_load_assets.inc"
    .include "des_draw_banner.inc"
    .include "des_metadata_logic.inc"
    .include "des_draw_room.inc"
    .include "des_ui_logic.inc"
    .include "des_player_control.inc"
    .include "des_update_aliens.inc"
    .include "des_draw_aliens.inc"
    .include "des_draw_popups.inc"
    
    .include "des_room_data.inc"
    .include "des_tiles.inc"
    
;-------------------------
; --- Global Variables ---
;-------------------------
debug_delay_counter:   db   60      ; delay counter for debugging

; Rooms vars
current_room_ptr:      dl room_00
current_room_id:       db  0

;---------------
; Player Vars
;---------------
player_sprite_id:      equ  0       ; Sprite ID number
player_x_mid:          db   16      ; Middle of player X
player_y_mid:          db   16      ; Middle of player Y
player_x:              dw   79      ; start x 
player_y:              dw   67      ; start y
player_dir:            db   0       ; 0 = down/south
is_moving:             db   0       ; 0 = not moving
move_timeout:          db   0       ; timer for movement
anim_frame:            db   0       ; frame 0 = looking down
anim_delay:            db   0       ; delay between frames
player_health:         db   100     ; Start health (100)
last_health:           db   0       ; Used to track changes
damage_cooldown:       db   0       ; cooldown after taking damage
damage_taken:          db   2       ; amount of damage to take
cooldown_time:         db   20      ; time to wait before taking damage again
knock_back_px:         db   2       ; amount of knock back in pixels

;----------------
; Aliens Vars
;----------------
; --- Small_alien ---
small_alien_sprite_id: equ 1    ; Sprite ID number
alien:                 equ 63   ; used for bitmap ID number
small_alien_x_mid:     db  8    ; Middle of alien X
small_alien_y_mid:     db  8    ; Middle of alien Y
small_alien_x:         dw  0   ; Current X
small_alien_y:         dw  0   ; Current Y
small_alien_dir:       db  0    ; 0 = down/south, 1 = up/north, 2 = left, 3 = right
small_alien_spawn_x:   dw  0   ; Default Spawn X
small_alien_spawn_y:   dw  0   ; Default Spawn Y
small_alien_room:      db  1    ; The ID of the room where he lives
sm_alien_is_active:    db  0    ; 1 = Visible, 0 = Hidden
alien_anim_timer:      db  0    ; Animation timer
alien_move_timer:      db  0    ; Movement timer
alien_state:           db  0    ; 0=Choosing, 1=Moving, 2=Waiting
alien_move_count:      db  0    ; How many steps left to take
alien_wait_timer:      db  0    ; How long to stay still

; --- Big Alien ---
big_alien_sprite_id:   equ 2    ; Sprite ID number
big_alien:             equ 38   ; used for bitmap ID number
big_alien_x_mid:       db  8    ; Middle of alien X
big_alien_y_mid:       db  16   ; Middle of alien Y
big_alien_x:           dw  0   ; Current X
big_alien_y:           dw  0   ; Current Y
big_alien_dir:         db  0    ; 0 = down/south, 1 = up/north, 2 = left, 3 = right
big_alien_spawn_x:     dw  0   ; Default Spawn X
big_alien_spawn_y:     dw  0   ; Default Spawn Y
big_alien_is_active:   db  0    ; 1 = Visible, 0 = Hidden
big_alien_anim_timer:  db  0    ; Animation timer
big_alien_move_timer:  db  0    ; Movement timer
big_alien_state:       db  0    ; 0=Choosing, 1=Moving, 2=Waiting
big_alien_move_count:  db  0    ; How many steps left to take
big_alien_wait_timer:  db  0    ; How long to stay still

alien_random_seed:     db  $A5  ; A starting seed for our random numbers
   
; Popup Window Vars
ui_win_active:         db  0
space_lock:            db  0
inv_lock:              db  0

;----------------
; Start Here
;----------------     
start_here:
    push af
    push bc
    push de
    push ix
    push iy

    CLEAR_ALL_BUFFERS          ; from des_main_subs.inc
    ; Initailize the display.
    call Init_Display          ; from des_main_subs.inc
    call Load_All_Assets       ; from des_load_assets.inc
    ; call Upload_Desol_Font   ; need to return to this.

    ; Initialize Sound Channels
    SET_SOUND_WAVEFORM 0, 0    ; Channel 0: Square wave for hits
    SET_SOUND_WAVEFORM 1, 4    ; Channel 1: Noise for death crash

    call Draw_Banner           ; from des_draw_banner.inc
    call Set_Graphics_VP       ; Set the Graphics VP for the game area

    V_SYNC                     ; wait for vertical sync

;--------------------------
; Trigger a Room Reload.
;--------------------------
Trigger_Room_Load:
    CLG                     ; clear graphics in the game screen.
    xor a                   ; clear accumulator
    ld (is_moving), a       ; Force the player to stop moving
    ld (move_timeout), a    ; Reset the timer

    call Decode_Room_Metadata      ; from des_metadata_logic.inc
    call Check_Alien_Presence      ; Checks room metadata and sets sm_alien_is_active & big_alien_is_active
    call Draw_Room                 ; from des_draw_room.inc
    call Draw_Health_Value         ; from main_subs.inc
    call update_animation          ; from des_player_control.inc

    ; Select Player Sprite (0)
    Select_Sprite_8bit 0
    Show_Sprite
    Update_GPU 
    jp main_loop

;=====================================================================
; MAIN GAME LOOP
;======================================================================
main_loop:
    ; --- Decrement Cooldown for player damage ---
    ld a, (damage_cooldown)                    ; get damage cooldown
    or a                                       ; check if zero
    jr z, .ready                               ; if zero, skip to .ready
    dec a                                      ; decrement damage cooldown
    ld (damage_cooldown), a                    ; store damage cooldown

.ready:
    ;
    ; 1 Checks
    ; a. Check if space was pressed near a terminal
    call Check_Action_Triggers                      ; des_ui_logic.inc
    ; b. Check if gameplay should be frozen
    ld a, (ui_win_active)
    or a
    ; If window is open (1), skip movement/logic to (skip_gameplay)
    jr nz, .skip_gameplay

    ; 2. Handle Player Movement and Animation
    call Movement_anim                              ; player_control.inc
    ; 3. Move the small alien in memory
    call Update_Small_Alien                         
    ; 4. Move the big alien in memory
    call Update_Big_Alien
    ; 5. Update midpoints for player and aliens for collision detection
    call Update_Midpoints 
    ; 6. Check if Player and Alien are touching
    call Collision_Detection
    ; 7. Draw the Small Alien 
    call Draw_Small_Alien            ; from des_draw_room.inc
    ; 8. Draw the Big Alien 
    call Draw_Big_Alien              ; from des_draw_room.inc
    
    ; --- Update all active sprites in the GPU ---
    ; Now that all sprites have sent their new coordinates to the VDP,
    ; tell the GPU to refresh everything at once.
    Update_GPU
    
    ; if pop up window is open jump here
    .skip_gameplay: 

;----------------------------------------
; Debug Print outs top left of screen
;----------------------------------------
    ; --- Delayed Debug Update ---
    ld a, (debug_delay_counter) ; get debug delay counter
    dec a                       ; decrement debug delay counter
    ld (debug_delay_counter), a ; store debug delay counter
    jr nz, .skip_debug          ; if not zero, skip to .skip_debug
    
    ; Reset counter to 60 (1 second at 60Hz)
    ld a, 60                    ; reset counter to 60 (1 second at 60Hz)
    ld (debug_delay_counter), a ; store debug delay counter
    ; print debug info to screen
    call Debug_Overlay      ; des_ui_logic.inc

.skip_debug:  

    V_SYNC                  ; Wait for vertical sync

   ; --- ESC key Check ---
    MOSCALL $1E             ; Get pointer from keyboard matrix
    ld a, (ix + $0E)        ; ESC byte from keyboard matrix
    bit 0, a                ; ESC bit from keyboard matrix
    jp nz, exit_back_to_mos ; Exit if pressed
    
    jp main_loop
;===========================================
; END MAIN GAME LOOP
;===========================================

;---------------------------------------------------------
; Check_Collision
; Checks if Player hitboxes overlap with active aliens
;---------------------------------------------------------
Collision_Detection:

; --- Check Small Alien First ---
ld a, (sm_alien_is_active)
or a
jr z, .check_big

; X-Axis Distance Check (Small Alien)
ld a, (player_x_mid)
ld b, a
ld a, (small_alien_x_mid)
sub b
jp p, .skip_neg_x1
neg
.skip_neg_x1:
cp 14
jr nc, .check_big

; Y-Axis Distance Check (Small Alien)
ld a, (player_y_mid)
ld b, a
ld a, (small_alien_y_mid)
sub b
jp p, .skip_neg_y1
neg
.skip_neg_y1:
cp 22
jr nc, .check_big
jp Take_Damage

.check_big:
; --- Check Big Alien ---
ld a, (big_alien_is_active)
or a
ret z

; X-Axis Distance Check (Big Alien)
ld a, (player_x_mid)
ld b, a
ld a, (big_alien_x_mid)
sub b
jp p, .skip_neg_x2
neg
.skip_neg_x2:
cp 14
ret nc

; Y-Axis Distance Check (Big Alien)
ld a, (player_y_mid)
ld b, a
ld a, (big_alien_y_mid)
sub b
jp p, .skip_neg_y2
neg
.skip_neg_y2:
cp 30
ret nc

jp Take_Damage

;---------------------------------------
; TAKE DAMAGE
;---------------------------------------
Take_Damage:
    ; --- 1. Cooldown Check ---
    ld a, (damage_cooldown)         ; get damage cooldown
    or a                            ; if a is not 0, exit
    ret nz                          ; Exit if we are still in cooldown

    ; --- Play Damage Sound ---
    ; Channel 0, Volume 127, 150Hz, 100ms
    PLAY_SOUND 0, 127, 150, 100
    ; Update health value
    call Draw_Health_Value  ; from main_subs.inc

    ; --- 2. Check for Death ---
    ld a, (player_health)           ; get player health
    sub 2                           ; Subtract the damage first
    jr z, .is_dead                  ; If result is 0, player is dead
    jr c, .is_dead                  ; If result is negative (Carry), they are dead
    ld (player_health), a           ; Otherwise, save the new health

    ; --- 3. Apply Physical Knockback --- Dir S(0)N(1)E(2)W(3),
    ld a, (player_dir)              ; Get current facing direction
    cp 0                            ; compare with 0
    jr z, .kb_up                    ; if equal, go to .kb_up
    cp 1                            ; compare with 1
    jr z, .kb_down                  ; if equal, go to .kb_down
    cp 2                            ; compare with 2
    jr z, .kb_left                  ; if equal, go to .kb_left
    cp 3                            ; compare with 3
    jr z, .kb_right                 ; if equal, go to .kb_right
    jr .apply_damage                ; Apply damage

.kb_up:
    ld hl, (player_y)               ; Get current Y
    ld bc, 4                        ; knock back pixels
    or a                            ; or a
    sbc hl, bc                      ; Subtract from Y
    ld (player_y), hl               ; Save new Y
    jr .apply_damage                ; Apply damage

.kb_down:
    ld hl, (player_y)               ; Get current Y
    ld bc, 4                        ; knock back pixels
    add hl, bc                      ; Add to Y
    ld (player_y), hl               ; Save new Y
    jr .apply_damage                ; Apply damage

.kb_right:
    ld hl, (player_x)               ; Get current X
    ld bc, 4                        ; knock back pixels
    add hl, bc                      ; Add to X
    ld (player_x), hl               ; Save new X
    jr .apply_damage                ; Apply damage

.kb_left:
    ld hl, (player_x)               ; Get current X
    ld bc, 4                        ; knock back pixels
    or a                            ; or a
    sbc hl, bc                      ; Subtract from X
    ld (player_x), hl               ; Save new X

.apply_damage:
    ; --- 4. Process Health Reduction ---
    ld a, (player_health)           ; Get current health
    sub 2                           ; Reduce health
    ld (player_health), a           ; Save new health
    ld a, 20                        ; Reset cooldown (0.5s at 30fps)
    ld (damage_cooldown), a         ; Save new cooldown
    call Draw_Health_Value          ; Update HUD
    ret

.is_dead:
    ; --- Trigger Death Sound Sequence ---
    PLAY_SOUND 0, 127, 400, 150     ; Mid note
    PLAY_SOUND 0, 127, 300, 150     ; Lower note
    PLAY_SOUND 0, 127, 200, 400     ; Long low note
    PLAY_SOUND 1, 100, 50, 500      ; Add a bit of white noise "crash"
    
    CLS                     
    ; Reset Viewports to full screen
    ld a, 26                     ; reset viewports
    rst.lil $10                  ; Send to vdp

    call Draw_Banner             ; Draw banner

    ; --- Draw Text ---
    call Draw_Dead               ; Draw dead text

Game_Over_Loop:

    V_SYNC                       ; Wait for vertical sync

    ; --- ESC key Check ---
    MOSCALL $1E                  ; Get pointer from keyboard matrix
    ld a, (ix + $0E)             ; ESC byte from keyboard matrix
    bit 0, a                     ; ESC bit from keyboard matrix
    jp nz, exit_back_to_mos      ; Exit if pressed
    
    jp Game_Over_Loop            ; loop back to game over loop

;--------------------------------------
; Game Over Banner
;--------------------------------------
Draw_Dead:
    SET_TXT_COL bright_red       ; set text colour to bright red

    ; line 1
    TABTO 8, 15                  ; tab to column 8, row 15
    ld hl, Dead_String_1         ; load dead string 1
    call print_string            ; print dead string 1

    ; line 2
    TABTO 9, 17                  ; tab to column 9, row 17
    ld hl, Dead_String_2         ; load dead string 2
    call print_string            ; print dead string 2

    SET_TXT_COL bright_white ; reset text colour to white
    ret

;---------------------------------------------
; Check Alien Presence (Using Metadata 43-47)
; 43: X Tile, 44: Y Tile, 46: Type (1=Sm, 2=Bg)
;---------------------------------------------
Check_Alien_Presence:
    ; Default both to inactive
    xor a
    ld (sm_alien_is_active), a
    ld (big_alien_is_active), a

    ld hl, current_room_metadata
    push hl
    ; Check Alien Type (Byte 46)
    ld de, $2E
    add hl, de
    ld a, (hl)       ; Offset $2E
    cp $01                ; Small Alien?
    jr z, .init_small
    cp $02                ; Big Alien?
    jr z, .init_big
    pop hl
    ret                   ; No alien or $61

.init_small:
    ld a, 1
    ld (sm_alien_is_active), a
    pop hl
    push hl
    ; Get X Tile (Byte 43-2B), multiply by 16 for Pixels
    ld de, $2B
    add hl, de
    ld a, (hl)
    ld l, a
    ld h, 0
    add hl, hl    ; *2
    add hl, hl    ; *4
    add hl, hl    ; *8
    add hl, hl    ; *16
    ld (small_alien_x), hl
    pop hl
    ; Get Y Tile (Byte 45-2D), multiply by 16 for Pixels
    ld de, $2D
    add hl, de
    ld a, (hl)
    ld l, a
    ld h, 0
    add hl, hl    ; *2
    add hl, hl    ; *4
    add hl, hl    ; *8
    add hl, hl    ; *16
    ld (small_alien_y), hl
    ret

.init_big:
    ld a, 1
    ld (big_alien_is_active), a
    pop hl
    push hl
    ; Get X Tile (Byte 43-2B), multiply by 16 for Pixels
    ld de, $2B
    add hl, de
    ld a, (hl)
    ld l, a
    ld h, 0
    add hl, hl    ; *2
    add hl, hl    ; *4
    add hl, hl    ; *8
    add hl, hl    ; *16
    ld (big_alien_x), hl
    pop hl
    ; Get Y Tile (Byte 45-2D), multiply by 16 for Pixels
    ld de, $2D
    add hl, de
    ld a, (hl)
    ld l, a
    ld h, 0
    add hl, hl    ; *2
    add hl, hl    ; *4
    add hl, hl    ; *8
    add hl, hl    ; *16
    ld (big_alien_y), hl
    ret

;------------------------------------
; Update Player and Aliens MidPoints
;------------------------------------
Update_Midpoints:
; --- Player ---
ld a, (player_x)
add a, 16
ld (player_x_mid), a
ld a, (player_y)
add a, 16
ld (player_y_mid), a

; --- Small Alien ---
ld a, (small_alien_x)
add a, 8
ld (small_alien_x_mid), a
ld a, (small_alien_y)
add a, 8
ld (small_alien_y_mid), a
; --- Big Alien ---
ld a, (big_alien_x)
add a, 8
ld (big_alien_x_mid), a
ld a, (big_alien_y)
add a, 16
ld (big_alien_y_mid), a

ret

;----------------
; Exit the Game.
;----------------
exit_back_to_mos:                 ;  exit back to mos
    SET_MODE 0                    ; set mode 0
    SHOW_CURSOR                   ; show cursor
    SET_TXT_BG_COL black          ; set text background colour to black
    SET_TXT_COL bright_white      ; set text colour to bright white
    CLS                           ; clear screen
    ; reset registers
    pop iy                        
    pop ix
    pop de
    pop bc
    pop af
    ld hl, 0
    ret

ram_buffer:                      ; ram buffer
    .ds 4096                     ; 4096 bytes
