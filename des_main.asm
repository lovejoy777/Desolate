; =============================================================================
; FILE: Desolate's des_main.asm
; TARGET: Agon Light 2 & ez80asm Assembler.
; =============================================================================

    .include "initial_macros.inc"
    .include "des_sound_macros.inc"
    .include "des_vdp_macros.inc"
    

    assume adl=1
    .org $40000
    jp start_here
    .align 64
    .db "MOS", 0, 1

;--------------------
; Includes
;--------------------
    
    ; Main Includes
    .include "des_input.inc"
    .include "des_load_font.inc"
    .include "des_font.inc"
    .include "des_strings.inc"
    .include "des_main_subs.inc"

    ; Game Logic Includes
    .include "des_load_assets.inc"
    .include "des_draw_banner.inc"
    .include "des_draw_room.inc"
    .include "des_ui_logic.inc"
    .include "des_player_control.inc"
    .include "des_update_aliens.inc"
    .include "des_draw_aliens.inc"
    .include "des_draw_popups.inc"
    .include "des_room_map.inc"

    ; Game Data Includes
    .include "des_room_data.inc"
    .include "des_tiles.inc"
    

; --- Variables ---
debug_delay_counter:   db 60

; Player.
player_sprite_id:      equ 0
player_x:              dw 79      ; start x 
player_y:              dw 67      ; start y
player_dir:            db 0       ; 0 = down/south
is_moving:             db 0       ; 0 = not moving
move_timeout:          db 0          
anim_frame:            db 0       ; frame 0 = looking down
anim_delay:            db 0
player_health:         db 100     ; Start health (100)
last_health:           db 0       ; Used to track changes
damage_cooldown:       db 0
damage_taken:          db 2
cooldown_time:         db 20
knock_back_px:         db 2

; Rooms
current_room_ptr:      .dl room_00
current_room_id:       db 0

; Aliens
; --- Small_alien) ---
small_alien_sprite_id: equ 1
small_alien_x:         dw 79       ; Current X
small_alien_y:         dw 80       ; Current Y
small_alien_dir:       db 0
small_alien_spawn_x:   dw 79       ; Default Spawn X
small_alien_spawn_y:   dw 80       ; Default Spawn Y
small_alien_room:      db 1        ; The ID of the room where he lives
sm_alien_is_active:    db 0        ; 1 = Visible, 0 = Hidden
alien_anim_timer:      db 0
alien_move_timer:      db 0
; alien tracking
alien_state:        db 0    ; 0=Choosing, 1=Moving, 2=Waiting
alien_move_count:   db 0    ; How many steps left to take
alien_wait_timer:   db 0    ; How long to stay still
alien_random_seed:  db $A5  ; A starting seed for our random numbers

; --- Alien Room List ---
; A list of all rooms containing a small alien. End with 255 as a marker.
alien_rooms_list: 
    .db 3, 18, 26, 29, 32, 33, 35, 39, 43, 45, 48, 49, 51, 54, 67, 68, 70, 255

; --- Big Alien ---
big_alien_sprite_id:   equ 2
big_alien_x:           dw 90       ; Current X
big_alien_y:           dw 50       ; Current Y
big_alien_dir:         db 0
big_alien_spawn_x:     dw 90       ; Default Spawn X
big_alien_spawn_y:     dw 50       ; Default Spawn Y
big_alien_is_active:      db 0         ; 1 = Visible, 0 = Hidden
big_alien_anim_timer:  db 0
big_alien_move_timer:  db 0
; big alien tracking
big_alien_state:       db 0         ; 0=Choosing, 1=Moving, 2=Waiting
big_alien_move_count:  db 0         ; How many steps left to take
big_alien_wait_timer:  db 0         ; How long to stay still

big_alien_rooms_list:  
    .db 4, 22, 37, 38, 40, 41, 47, 52, 53, 56, 57, 61, 64, 65, 66, 69, 71, 255

alien:                 equ     63   ; used for bitmap ID number

;--------------------
; Popup Window Vars
;--------------------
ui_win_active:         db 0
space_lock:            db 0
inv_lock:              db 0
          
start_here:
    push af
    push bc
    push de
    push ix
    push iy

    ; Initailize the display.
    call Init_Display      ; from des_main_subs.inc
    CLEAR_ALL_BUFFERS      ; from des_main_subs.inc
    call Load_All_Assets

    ; call Upload_Desol_Font ; need to return to this.

    ; Initialize Sound Channels
    SET_SOUND_WAVEFORM 0, 0    ; Channel 0: Square wave for hits
    SET_SOUND_WAVEFORM 1, 4    ; Channel 1: Noise for death crash

    call Draw_Banner
    
    call Set_Graphics_VP     ; Set the Graphics VP for the game area

    V_SYNC

;--------------------------
; Trigger a Room Reload.
;--------------------------
Trigger_Room_Load:
    CLG                     ; clear graphics in the game screen.
    xor a
    ld (is_moving), a       ; Force the player to stop moving
    ld (move_timeout), a    ; Reset the timer

    ; --- Reset small Alien to Spawn Point ---
    ld hl, (small_alien_spawn_x)
    ld (small_alien_x), hl
    ld hl, (small_alien_spawn_y)
    ld (small_alien_y), hl

    ; --- Reset Big Alien to Spawn Point ---
    ld hl, (big_alien_spawn_x)
    ld (big_alien_x), hl
    ld hl, (big_alien_spawn_y)
    ld (big_alien_y), hl

    call Check_Alien_Presence      ; Checks list and sets alien_is_active

    call Draw_Room          ; from des_draw_room.inc
    call Draw_Health_Value  ; from main_subs.inc
    call update_animation   ; from player_control.inc

    Select_Sprite_8bit 0
    Show_Sprite
    Update_GPU
    

    jp main_loop

;=====================================================================
; MAIN LOOP
;======================================================================
main_loop:

    ; --- Decrement Cooldown ---
    ld a, (damage_cooldown)
    or a
    jr z, .ready
    dec a
    ld (damage_cooldown), a

.ready:
    ; 1 Check if space was pressed near a terminal
    call Check_Action_Triggers                      ; des_ui_logic.inc
    ; Check if gameplay should be frozen
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
    ; 5. Check if Player and Alien are touching
    call Check_Collision

    ;----------------------------------------------
    ; Draw routines 
    ;----------------------------------------------
    ; 1. Draw the Small Alien 
    call Draw_Small_Alien
    ; 2. Draw the Big Alien 
    call Draw_Big_Alien
    
    ; --- Update all active sprites in the GPU ---
    ; Now that all sprites have sent their new coordinates to the VDP,
    ; we tell the GPU to refresh everything at once.
    Update_GPU
    
    .skip_gameplay: ; if window is open jump here

    ;----------------------------------------
    ; Debug Print outs top right of screen
    ;----------------------------------------
    ; --- Delayed Debug Update ---
    ld a, (debug_delay_counter)
    dec a
    ld (debug_delay_counter), a
    jr nz, .skip_debug
    
    ; Reset counter to 60 (1 second at 60Hz)
    ld a, 60
    ld (debug_delay_counter), a
    
    call Debug_Overlay

.skip_debug:  

    V_SYNC

   ; --- Quick ESC Check ---
    MOSCALL $1E             ; Get pointer
    ld a, (ix + $0E)        ; ESC byte
    bit 0, a                ; ESC bit
    jp nz, exit_back_to_mos ; Exit if pressed
    

    jp main_loop
;===========================================
; END MAIN OF LOOP
;===========================================

;---------------------------------------
; CHECK COLLISION
;---------------------------------------
Check_Collision:
    ; --- 1. Small Alien Check ---
    ld a, (sm_alien_is_active)
    or a
    jr z, .check_big_alien      ; If no small alien, go check the big one
    
    ; Small Alien logic
    ld a, (player_x)
    ld b, a
    ld a, (small_alien_x)
    sub b
    jp p, .pos_x_sm
    neg
.pos_x_sm:
    cp 14
    jr nc, .check_big_alien     ; No X collision with small, check big

    ld a, (player_y)
    add a, 16                   ; Player foot offset
    ld b, a
    ld a, (small_alien_y)
    sub b
    jp p, .pos_y_sm
    neg
.pos_y_sm:
    cp 14
    jr nc, .check_big_alien     ; No Y collision with small, check big
    
    call Take_Damage            ; Small Alien Hit!
    ret                         ; Exit so we don't trigger damage twice per frame

    ; --- 2. Big Alien Check ---
.check_big_alien:
    ld a, (big_alien_is_active)    ;
    or a
    ret z                       ; Neither alien is active, exit

    ; Check X (Same for Head and Body)
    ld a, (player_x)
    ld b, a
    ld a, (big_alien_x)         ;
    sub b
    jp p, .pos_x_ba
    neg
.pos_x_ba:
    cp 14
    ret nc                      ; No X collision with big alien

    ; --- Check Y (Big Alien Head) ---
    ld a, (player_y)
    add a, 16
    ld b, a
    ld a, (big_alien_y)         ;
    sub b
    jp p, .pos_y_ba
    neg
.pos_y_ba:
    cp 14
    jr c, .ba_hit               ; Big Alien Hit!

                      ; No collision with head or body

.ba_hit:
    call Take_Damage            ; Big Alien Hit!
    ret

;---------------------------------------
; TAKE DAMAGE
;---------------------------------------

Take_Damage:
    ; --- 1. Cooldown Check ---
    ld a, (damage_cooldown)
    or a
    ret nz                          ; Exit if we are still in cooldown

    ; --- Play Damage Sound ---
    ; Channel 0, Volume 127, 150Hz, 100ms
    PLAY_SOUND 0, 127, 150, 100
     call Draw_Health_Value  ; from main_subs.inc

    ; --- 2. Check for Death ---
    ld a, (player_health)           ; 
    sub 2                           ; Subtract the damage first
    jr z, .is_dead                  ; If result is 0, they are dead
    jr c, .is_dead                  ; If result is negative (Carry), they are dead
    ld (player_health), a           ; Otherwise, save the new health

    ; --- 3. Apply Physical Knockback --- Dir S(0)N(1)E(2)W(3),
    ld a, (player_dir)              ; Get current facing direction
    cp 0
    jr z, .kb_up                    ; Facing Down? Bounce Up
    cp 1
    jr z, .kb_down                  ; Facing Up? Bounce Down
    cp 2
    jr z, .kb_left                  ; Facing Right? Bounce Left
    cp 3
    jr z, .kb_right                 ; Facing Left? Bounce Right
    jr .apply_damage

.kb_up:
    ld hl, (player_y)               ; Get current Y
    ld bc, 4                        ; 4 pixels
    or a
    sbc hl, bc                      ; Subtract from Y
    ld (player_y), hl               ; Save new Y
    jr .apply_damage

.kb_down:
    ld hl, (player_y)               ;
    ld bc, 4
    add hl, bc                      ; Add to Y
    ld (player_y), hl               ;
    jr .apply_damage

.kb_right:
    ld hl, (player_x)               ; Get current X
    ld bc, 4
    add hl, bc                      ; Add to X
    ld (player_x), hl               ;
    jr .apply_damage

.kb_left:
    ld hl, (player_x)               ;
    ld bc, 4
    or a
    sbc hl, bc                      ; Subtract from X
    ld (player_x), hl               ;

.apply_damage:
    ; --- 4. Process Health Reduction ---
    ld a, (player_health)           ;
    sub 2                           ; Reduce health
    ld (player_health), a           ;
    ld a, 20                        ; Reset cooldown (0.5s at 30fps)
    ld (damage_cooldown), a         ;
    call Draw_Health_Value          ; Update HUD
    ret

.is_dead:
    ; --- Trigger Death Sound Sequence ---
    PLAY_SOUND 0, 127, 400, 150  ; Mid note
    PLAY_SOUND 0, 127, 300, 150  ; Lower note
    PLAY_SOUND 0, 127, 200, 400  ; Long low note
    PLAY_SOUND 1, 100, 50, 500   ; Add a bit of white noise "crash"
    
    CLS                     
    ; Reset Viewports to full screen
    ld a, 26
    rst.lil $10

    call Draw_Banner

    ; --- Draw Text ---
    call Draw_Dead

Game_Over_Loop:
    V_SYNC
    ; Check ESC to exit
    MOSCALL $1E             ; Get pointer
    ld a, (ix + $0E)        ; ESC byte
    bit 0, a                ; ESC bit
    jp nz, exit_back_to_mos ; Exit if pressed
    
    jp Game_Over_Loop

;--------------------------------------
; Game Over Banner
;--------------------------------------
Draw_Dead:
    SET_TXT_COL bright_red

    ; line 1
    TABTO 8, 15
    ld hl, Dead_String_1
    call print_string

    ; line 2
    TABTO 9, 17
    ld hl, Dead_String_2
    call print_string

    SET_TXT_COL bright_white
    ret

;---------------------------------------------
; Check Alien Presence (Combined Small & Big)
;---------------------------------------------
Check_Alien_Presence:

    ;--------------------------------------
    ; --- STEP 1: Check for Small Alien ---
    ;--------------------------------------
    ld hl, alien_rooms_list
    ld a, (current_room_id)
    ld b, a

.sm_check_loop:
    ld a, (hl)
    cp 255
    jr z, .sm_not_found     ; If not in small list, go to 'not found'
    cp b
    jr z, .sm_found         ; If in list, go to 'found'
    inc hl
    jr .sm_check_loop

.sm_found:
    ld a, 1
    ld (sm_alien_is_active), a
    ; Reset small alien to spawn point
    ld a, (small_alien_spawn_x)
    ld (small_alien_x), a
    xor a
    ld (small_alien_x + 1), a
    ld a, (small_alien_spawn_y)
    ld (small_alien_y), a
    xor a
    ld (small_alien_y + 1), a
    jr .big_alien_start     ; Done with small, move to big check

.sm_not_found:
    xor a
    ld (sm_alien_is_active), a
    ; Fall through to check the big alien list

    ;------------------------------------
    ; --- STEP 2: Check for Big Alien ---
    ;-------------------------------------
.big_alien_start:
    ld hl, big_alien_rooms_list
    ld a, (current_room_id)
    ld b, a

.big_check_loop:
    ld a, (hl)
    cp 255
    jr z, .big_not_found    ; Not in big list
    cp b
    jr z, .big_found        ; Found in big list
    inc hl
    jr .big_check_loop

.big_found:
    ld a, 1
    ld (big_alien_is_active), a
    ; Reset big alien to spawn point
    ld a, (big_alien_spawn_x)
    ld (big_alien_x), a
    xor a
    ld (big_alien_x + 1), a
    ld a, (big_alien_spawn_y)
    ld (big_alien_y), a
    xor a
    ld (big_alien_y + 1), a
    ret                     ; END OF ROUTINE

.big_not_found:
    xor a
    ld (big_alien_is_active), a
    ret

;----------------
; Exit the Game.
;----------------
exit_back_to_mos:
    SET_MODE 0
    SHOW_CURSOR
    SET_TXT_BG_COL black
    SET_TXT_COL bright_white
    CLS


    pop iy
    pop ix
    pop de
    pop bc
    pop af
    ld hl, 0
    ret

; Ram Buffer.
ram_buffer:
    .ds 4096

;==============================================
; Strings
;==============================================

Dead_String_1:
    .db "The Desolate has claimed", 0
Dead_String_2:
    .db "your life too . . .", 0

Inv_Title_String:
    .db "-INVENTORY-", 0
Inv_Item_String:
    .db "No Items", 0

