; =============================================================================
; FILE: Desolate's des_main.asm
; TARGET: Agon Light 2 & ez80asm Assembler.
; =============================================================================
    
    .include "initial_macros.inc"
    .include "des_vdp_macros.inc"
    .include "des_sound_macros.inc"
    
    assume adl=1             ; use 24-bit addressing mode
    .org $40000              ; set origin to $40000
    jp start_here            ; jump to start_here
    .align 64                ; align to 64 bytes
    .db "MOS", 0, 1          ; MOS header

;--------------------
; Includes
;--------------------
    ;.include "strings24.asm"
    .include "des_main.asm"
    .include "des_load_font.inc"
    .include "des_font.inc"
    .include "des_input.inc"
    .include "des_main_subs.inc"
    .include "des_data.inc"
    .include "des_tiles.inc"
    .include "des_strings.inc"
    
    .include "des_load_assets.inc"
    .include "des_draw_banner.inc"
    .include "des_metadata_logic.inc"
    .include "des_draw_room.inc"
    .include "des_draw_menu.inc"
    .include "des_story.inc"
    .include "des_ui_logic.inc"
    .include "des_player_control.inc"
    .include "des_update_aliens.inc"
    .include "des_draw_aliens.inc"
    .include "des_draw_popups.inc"

    .include "des_shooting.inc"
    .include "des_game_over.inc"
    
;-------------------------
; --- Global Variables ---
;-------------------------

debug_delay_counter:    db    60      ; delay counter for debugging
debug_is_active:        db    1       ; 1 = Active (Default)
banner_colour:          db    7
story_mode_is_active:   db    0       ; 1 = Active (Default)
story_page:             db    1       ; 1 = Default (page 1)
info_mode_is_active:    db    0
credits_mode_is_active: db    0
game_screen_offset_x:   dw    63
game_screen_offset_x16: dw    63
game_screen_offset_y:   dw    83
game_screen_offset_y16: dw    83

last_char_spacing:      db    1
start_char_story_x:     db    56
start_char_popup_x:     db    86

move_menu_bg_x:         db    8
move_menu_bg_y:         db    8


;--------
;UI vars
;--------
look_shoot_mode:        db    0       ; Look/Shoot Mode (0 = Look, 1 = Shoot)
main_menu_mode:         db    0       ; Look/Shoot Mode (0 = New Game, 1 = Continue Game, 2 = info, 3 = credits, 4 = quit)

main_menu_is_active:    db    1       ; 1 = Active (Default)
start_win_active:       db    0       ; start Window Flag
ui_win_active:          db    0       ; Popup Window Flag
restart_win_active:     db    0       ; Restart Window Flag

;-------------------
; Room/Screen vars
;-------------------

; pointers for tilesets data blocks
current_room_ptr:       dl    room_00          ; data block in des_data.inc
main_menu_ptr:          dl    main_menu        ; data block in des_data.inc
main_menu_bg_ptr:       dl    main_menu_bg     ; data block in des_data.inc
inventory_popup_ptr:    dl    inventory_popup  ; data block in des_data.inc
data_cart_popup_ptr:    dl    data_cart_popup  ; data block in des_data.inc
door_lock_popup_ptr:    dl    door_lock_popup  ; data block in des_data.inc
small_popup_ptr:        dl    small_popup      ; data block in des_data.inc Decode_Room_Metadata

current_room_id:        db    0
current_room_metadata:  ds    49             ; 49 byte buffer
Alien_Kill_Table:       ds    80      ; Allocate 80 bytes

;---------
; sprites
;---------
playerSprite:           equ   0
smAlienSprite:          equ   1
bigAlienSprite:         equ   2
bulletSprite:           equ   3

;---------------
; Player Vars
;---------------
player_is_alive:        db    1

player_x_mid:           db    16      ; Middle of player X
player_y_mid:           db    16      ; Middle of player Y
player_x:               dw    79      ; start x 
player_y:               dw    67      ; start y
player_dir:             db    0       ; 0 = down/south
is_moving:              db    0       ; 0 = not moving
move_timeout:           db    0       ; timer for movement
anim_frame:             db    0       ; frame 0 = looking down
anim_delay:             db    0       ; delay between frames
player_health:          db    0       ; current health
player_start_hp:        equ   20      ; start health
last_health:            db    0       ; Used to track changes
damage_cooldown:        db    0       ; cooldown after taking damage
damage_taken:           db    2       ; amount of damage to take
cooldown_time:          db    20      ; time to wait before taking damage again
knock_back_px:          db    8       ; amount of knock back in pixels

;----------------
; Aliens Vars
;----------------
alien_random_seed:      db    $A5      ; A starting seed for our random numbers
alien_is_alive:         db    0

; Alien Type Constants
alien_type_small:       equ   $01
small_alien_start_hp:   equ   3
alien_type_big:         equ   $02
big_alien_start_hp:     equ   10

; --- Small_alien ---
alien_amount_killed:    db    0
small_alien_hp:         db    3
small_alien_x_mid:      db    8        ; Middle of alien X
small_alien_y_mid:      db    8        ; Middle of alien Y
small_alien_x:          dw    0        ; Current X
small_alien_y:          dw    0        ; Current Y
small_alien_spawn_x:    dw    0        ; Default Spawn X
small_alien_spawn_y:    dw    0        ; Default Spawn Y
small_alien_is_active:  db    0        ; 1 = Visible, 0 = Hidden
small_alien_dir:        db    0        ; 0 = down/south, 1 = up/north, 2 = left, 3 = right
small_alien_anim_timer: db    0        ; Animation timer
small_alien_move_timer: db    0        ; Movement timer
small_alien_state:      db    0        ; 0=Choosing, 1=Moving, 2=Waiting
small_alien_move_count: db    0        ; How many steps left to take
small_alien_wait_timer: db    0        ; How long to stay still
small_alien_speed:      db    2        ; less is faster
; --- Big Alien ---
big_alien_hp:           db    0
big_alien_x_mid:        db    8        ; Middle of alien X
big_alien_y_mid:        db    16       ; Middle of alien Y
big_alien_x:            dw    0        ; Current X
big_alien_y:            dw    0        ; Current Y
big_alien_spawn_x:      dw    0        ; Default Spawn X
big_alien_spawn_y:      dw    0        ; Default Spawn Y
big_alien_is_active:    db    0        ; 1 = Visible, 0 = 
big_alien_dir:          db    0        ; 0 = down/south, 1 = up/north, 2 = left, 3 = right
big_alien_anim_timer:   db    0        ; Animation timer
big_alien_move_timer:   db    0        ; Movement timer
big_alien_state:        db    0        ; 0=Choosing, 1=Moving, 2=Waiting
big_alien_move_count:   db    0        ; How many steps left to take
big_alien_wait_timer:   db    0        ; How long to stay still
big_alien_speed:        db    2        ; less is faster

;-----------------
; keyboard locks
;----------------
up_lock:                db    0
down_lock:              db    0
left_lock:              db    0
right_lock:             db    0
space_lock:             db    0
look_lock:              db    0
shoot_lock:             db    0
close_lock:             db    0
menu_lock:              db    0
inv_lock:               db    0

;-----------------
; Shooting
;-----------------
shoot_is_active:        db    1       ; 1 = Active (have gun)                            
bullet_is_active:       db    0
bullet_frame:           db    0
bullet_direction:       db    0
bullet_spawn_x:         dw    0
bullet_spawn_y:         dw    0
bullet_x:               dw    0
bullet_y:               dw    0
bullet_x_mid:           dw    0
bullet_y_mid:           dw    0
bullet_tip_x:           dw    0
bullet_tip_y:           dw    0
bullet_speed:           db    2

; Collision Hit Boxes
; Bullet Box
box1_L: dw 0
box1_R: dw 0
box1_T: dw 0
box1_B: dw 0
; Alien Box
box2_L: dw 0
box2_R: dw 0
box2_T: dw 0
box2_B: dw 0

ram_buffer:             ds    4096    ; 4096 bytes

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

    call CleanStart            ; from des_main_subs.
    call Init_Display          ; from des_main_subs.inc
    call Load_All_Assets       ; from des_load_assets.inc
    ;call Upload_Desol_Font   ; need to return to this.

    ; Initialize Sound Channels
    SET_SOUND_WAVEFORM 0, 0    ; Channel 0: Square wave for hits
    SET_SOUND_WAVEFORM 1, 4    ; Channel 1: Noise for death crash

    call Reset_VP
    ld a, grey
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP

    call Debug_Overlay_Titles  ; Temp debug overlay, top left
           
    V_SYNC 

    call Main_Game

    ret





