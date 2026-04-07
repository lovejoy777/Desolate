Main_Game: 

Menu_Screen:
    
    CLG
    ; set flag
    ld a, 1
    ld (main_menu_is_active), a
    ld (ui_win_active), a

    call Reset_VP
    ld a, bright_cyan
    ld (banner_colour), a

    call Draw_Banner_Image           ; from des_draw_banner.inc
    call Set_Game_VP                 ; from des_main_subs.inc

    call Draw_Main_Menu_BG           ; from des_draw_menu.inc
    call Draw_Main_Menu              ; from des_draw_menu.inc
    call Main_Menu_Mode              ; from des_ui_logic.inc

    ;ld a, 1
    

    ; fall into Check_Keys:

Check_Keys:       
    
    call Check_Action_Triggers       ; from des_ui_logic.inc

    jp Check_Keys


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
    ; Check Key Presses
    call Check_Action_Triggers                      ; des_ui_logic.inc
    ; Check if gameplay should be frozen
    ld a, (ui_win_active)
    or a
    ; If window is open (1), skip movement/logic to (skip_gameplay)
    jr nz, .skip_gameplay

    ; Handle Player Movement and Animation
    call Movement_anim                ; from player_control.inc
    ;
    call Update_Bullet      ; Handle bullet movement and collisions
    ; Move the small alien
    call Update_Small_Alien                         
    ; Move the big alien
    call Update_Big_Alien
    ; Update midpoints for player and aliens for collision detection
    call Update_Midpoints 
    call Collision_Detection         ; from des_player_control.inc
    call Draw_Small_Alien            ; from des_draw_aliens.inc
    call Draw_Big_Alien              ; from des_draw_aliens.inc
    
    ; --- Update all active sprites in the GPU ---
    ; Now that all sprites have sent their new coordinates to the VDP,
    ; tell the GPU to refresh everything at once.
    Update_GPU
    
    ; if pop up window is open jump here
.skip_gameplay: 

;----------------------------------------
; Debug Print outs top left of screen
;----------------------------------------
    ; check for debug flag
    ld a, (debug_is_active)
    cp $01
    jp nz, .skip_debug

    ; --- Delayed Debug Update ---
    ld a, (debug_delay_counter) ; get debug delay counter
    dec a                       ; decrement debug delay counter
    ld (debug_delay_counter), a ; store debug delay counter
    jr nz, .skip_debug          ; if not zero, skip to .skip_debug
    ; Reset counter to 60 (1 second at 60Hz)
    ld a, 60                    ; reset counter to 60 (1 second at 60Hz)
    ld (debug_delay_counter), a ; store debug delay counter
    ; print debug info to screen
    call Debug_Overlay_Values      ; des_ui_logic.inc

.skip_debug:  

    V_SYNC                  ; Wait for vertical sync
    
    jp main_loop
;===========================================
; END MAIN GAME LOOP
;===========================================

;----------------------------------------------
; Update Player, Bullets and Aliens MidPoints
;----------------------------------------------
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

; Bullet
ld a, (bullet_x)
add a, 8
ld (bullet_x_mid), a
ld a, (bullet_y)
add a, 8
ld (bullet_y_mid), a

ret

;----------------
; Return to Mos.
;----------------
exit_back_to_mos:
    CLEAR_ALL_BUFFERS
    SET_MODE 0                    ; set mode 0
    SHOW_CURSOR                   ; show cursor
    SET_TXT_BG_COL black          ; set text background colour to black
    SET_TXT_COL bright_white      ; set text colour to bright white
    CLG
    CLS
; reset registers
    pop iy                        
    pop ix
    pop de
    pop bc
    pop af
    ld hl, 0
    RST 00h         ; Call reset 0, returns to command prompt
    ret