Check_Action_Triggers:
;---------------------------------------------
; Check Key presses of keyboard
;---------------------------------------------
;=========================
; CURSOR MOVEMENT KEYS
;=========================
    ; --- Check UP cursor key ---
    ld b, KEY_UP_BYTE
    ld c, KEY_UP_BIT
    call Is_Key_Held_Matrix
    jp nz, .up_key_is_down

    ; --- Check DOWN cursor key ---
    ld b, KEY_DOWN_BYTE
    ld c, KEY_DOWN_BIT
    call Is_Key_Held_Matrix
    jp nz, .down_key_is_down

    ; --- Check LEFT cursor key ---
    ld b, KEY_LEFT_BYTE
    ld c, KEY_LEFT_BIT
    call Is_Key_Held_Matrix
    jp nz, .left_key_is_down

    ; --- Check RIGHT cursor key ---
    ld b, KEY_RIGHT_BYTE
    ld c, KEY_RIGHT_BIT
    call Is_Key_Held_Matrix
    jp nz, .right_key_is_down

;=========================
; ACTION KEYS
;=========================
    ; --- Check Spacebar for Look or Shoot ---
    ld b, KEY_SPACE_BYTE
    ld c, KEY_SPACE_BIT
    call Is_Key_Held_Matrix
    jp nz, .space_is_down

    ; --- Check "Z" Key for Look Mode ---
    ld b, KEY_Z_BYTE
    ld c, KEY_Z_BIT
    call Is_Key_Held_Matrix
    jp nz, .z_key_is_down

    ; --- Check "X" Key for Shoot Mode ---
    ld b, KEY_X_BYTE
    ld c, KEY_X_BIT
    call Is_Key_Held_Matrix
    jp nz, .x_key_is_down

    ; --- Check "C" Key for Close All Popups ---
    ld b, KEY_C_BYTE
    ld c, KEY_C_BIT
    call Is_Key_Held_Matrix
    jp nz, .c_key_is_down

    ; --- Check "M" Key for Menu ---
    ld b, KEY_M_BYTE        
    ld c, KEY_M_BIT         
    call Is_Key_Held_Matrix
    jp nz, .m_key_is_down

    ; --- Check "I" Key for Inventory ---
    ld b, KEY_I_BYTE        
    ld c, KEY_I_BIT         
    call Is_Key_Held_Matrix
    jp nz, .i_key_is_down

    ; --- Key is NOT held (Released) ---
    xor a
    ld (up_lock), a
    ld (down_lock), a
    ld (left_lock), a
    ld (right_lock), a
    ld (space_lock), a
    ld (look_lock), a
    ld (shoot_lock), a
    ld (close_lock), a
    ld (menu_lock), a
    ld (inv_lock), a

    ret

;==============================
; KEYS PRESSED
;==============================

;=========================
;  Main Menu Mode KEY
;=========================
.up_key_is_down:
    ld a, (up_lock)        ; Debounce check for up cursor key
    or a
    ret nz

    ld a, 1
    ld (up_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    ret nz

    ld a, (main_menu_mode)
    cp $00
    jp z, .go_to_4

    sub 1
    ld (main_menu_mode), a
    call playNav_tick
    call Main_Menu_Mode

.go_to_4_end:

    ret

.go_to_4:
    ld a, $04
    ld (main_menu_mode), a
    call playNav_tick
    call Main_Menu_Mode
    jp .go_to_4_end

.down_key_is_down:
    ld a, (down_lock)        ; Debounce check for down cursor key
    or a
    ret nz

    ld a, 1
    ld (down_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    ret nz

    ld a, (main_menu_mode)
    cp $04
    jp z, .go_to_0

    add 1
    ld (main_menu_mode), a
    call playNav_tick
    call Main_Menu_Mode

.go_to_0_end:

    ret

.go_to_0:
    ld a, $00
    ld (main_menu_mode), a
    call playNav_tick
    call Main_Menu_Mode
    jp .go_to_0_end

.left_key_is_down:
    ld a, (left_lock)        ; Debounce check for left cursor key
    or a
    ret nz

    ld a, 1
    ld (left_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    ret nz
    ;call Draw_Small_Popup
    call playNav_tick
    
    ret
    

.right_key_is_down:
    ld a, (right_lock)        ; Debounce check for right cursor key
    or a
    ret nz

    ld a, 1
    ld (right_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    ret nz

    call playNav_tick
    
    ret

.space_is_down:
    ; --- Debounce Check ---
    ld a, (space_lock)
    or a
    ret nz

    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (space_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    jp nz, .skip_menu 

    ld a, 1
    ld (ui_win_active), a

    ; main menu
    ld a, (main_menu_mode)
    cp $00
    jp z, .new
    cp $01
    jp z, .cont
    cp $02
    jp z, .info
    cp $03
    jp z, .creds
    cp $04
    jp z, .quit

.new:
    call playSelect
   
    call story_mode

    ld a, (story_page)
    ld b, a
    inc b
    ld a, b
    ld (story_page), a

    ret

.cont:
    call playSelect
    ld a, 0
    ld (main_menu_is_active), a
    ld (ui_win_active), a
    call Trigger_Room_Load

    ret

.info:
    call playGlitch

    call Draw_Data_Cart_Popup
    TEXT_AT_GRAPHICS_CURSOR

    ld a, $01
    ld (info_mode_is_active), a

    ; -Controls- (title)
    ld hl, SE0A5 
    ld b, 112    ; x
    ld c, 106    ; y
    call print_frac_string
    ; keyboard controls
    ld hl, SE0A7 
    ld b, 78
    ld c, 118
    call print_frac_string

    SET_GCOL_BG_COL grey
    SET_GCOL_COL black

    ret


.creds:
    call playSelect

    ld a, $01
    ld (credits_mode_is_active), a

    call Reset_VP
    ld a, bright_cyan
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP

    TEXT_AT_GRAPHICS_CURSOR     ; set text at graphics cursor.
    SET_GCOL_BG_COL black
    SET_GCOL_COL bright_cyan

    CLG
    ; MaxCoderz Presents
    ld hl, SE09D 
    ld b, 92
    ld c, 106
    call print_frac_string
    ; Original author 
    ld hl, SE09F 
    ld b, 104
    ld c, 120
    call print_frac_string
    ; ported to the
    ld hl, SCR40 
    ld b, 108
    ld c, 148
    call print_frac_string
    ; Agon Light 2
    ld hl, SCR41 
    ld b, 112
    ld c, 160
    call print_frac_string
    ; by lovejoy777
    ld hl, SCR42 
    ld b, 108
    ld c, 174
    call print_frac_string
    ; 2026 +
    ld hl, SCR43 
    ld b, 136
    ld c, 188
    call print_frac_string
    
    SET_GCOL_BG_COL grey
    SET_GCOL_COL black


    ret

.quit:
    PLAY_SOUND 0, 20, 500, 50
    ld a, 0
    ld (main_menu_is_active), a
    call exit_back_to_mos

    ret

.skip_menu:

    ; look/shoot
    ld a, (look_shoot_mode)
    cp $00
    jp z, .look
    cp $01
    jp z, .shoot

.look:
    ld a, (ui_win_active)
    cp $01
    ret z


    ; --- Get Current Position ---
    ld hl, (player_x)
    ld de, (player_y)

    ; --- Normalize to Center-Mass (Exact Debug Match) ---
    ld a, l
    add a, 16
    ld l, a
    ld a, e
    add a, 24
    ld e, a

    ; --- Branching for Look-Ahead ---
    ld a, (player_dir)
    cp 0                ; UP
    jr z, .look_down
    cp 1                ; DOWN
    jr z, .look_up
    cp 2                ; RIGHT
    jr z, .look_right
    cp 3                ; LEFT
    jr z, .look_left

    ret

.look_down:
    ld a, e
    add a, 10
    ld e, a
    jr .do_tile_check
.look_up:
    ld a, e
    sub 26
    ld e, a
    jr .do_tile_check
.look_right:
    ld a, l
    add a, 16
    ld l, a
    jr .do_tile_check
.look_left:
    ld a, l
    sub 16
    ld l, a
    
.do_tile_check:

    call get_tile_at_coords
    
    ; Compare against your Terminal Tile IDs (26 decimal)
    cp 26
    jr z, .terminal_found
    cp 27
    jr z, .terminal_found
    cp 28
    jr z, .terminal_found
    cp 29
    jr z, .terminal_found
    
    ; If not 26,27,28 or 29 exit
    ret

.terminal_found:
    ; --- SUCCESS: OPEN WINDOW ---
    ld a, 1
    ld (ui_win_active), a        
   ; ld hl, SE131  ; message from des_strings

    call playGlitch
    

    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    call Draw_Data_Cart_Popup
    ;call Display_Message 

    ; --- Restore Screen colours---
    SET_TXT_BG_COL black
    SET_TXT_COL bright_white

    ret

.shoot:
    ld a, (ui_win_active)
    cp $01
    ret z

    call Shoot

    ret 


;=========================
; Z Look Mode KEY
;=========================
.z_key_is_down:
    ld a, (ui_win_active)
    cp $01
    ret z

    ld a, (look_lock)        ; Debounce check for Z look key
    or a
    ret nz

    ld a, 1
    ld (look_lock), a
    ld a, 0
    ld (look_shoot_mode), a

    call playSelect
    call Look_Shoot_Mode
    
    ret

;=========================
; X Shoot Mode KEY
;=========================
.x_key_is_down:
    ld a, (ui_win_active)
    cp $01
    ret z
    
    ld a, (shoot_lock)        ; Debounce check for X shoot key
    or a
    ret nz

    ld a, 1
    ld (shoot_lock), a
    ld (look_shoot_mode), a

    call playSelect
    call Look_Shoot_Mode

    ret

;=================================
; "C" Close All Popup Windows KEY
;=================================
.c_key_is_down:
    ld a, (ui_win_active)
    cp $01
    ret nz

    ld a, (close_lock)        ; Debounce check for X shoot key
    or a
    ret nz

    call playSelect

    ld a, 0
    ld (ui_win_active), a

    ld a, (main_menu_is_active)
    cp $01
    jp z, back_to_main_menu

.continue_game:

    CLG
    call Draw_Room
    call Draw_Health_Value         ; from main_subs.inc
    call Look_Shoot_Mode           ; from des_ui_logic.inc

    Select_Sprite_8bit playerSprite
    Show_Sprite

    ret

back_to_main_menu:
    ld a, $00
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a
    call Menu_Screen
    ret

;=========================
; "M" Menu KEY
;=========================
.m_key_is_down:
    ld a, (menu_lock)        ; Debounce check for M Menu key
    or a
    ret nz

    ; Check if player is alive
    ld a, (player_is_alive) 
    cp $01
    jp nz, .player_is_dead

    ld a, 1
    ld (menu_lock), a
    ld (main_menu_is_active), a

    call playSelect

    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    ld a, $00
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a

    jp Menu_Screen
    
    ret

.player_is_dead:
    ; "m" key while player is dead.
    ld a, 1
    ld (main_menu_is_active), a
    ld (ui_win_active), a
    ld (player_is_alive), a

    ld a, (menu_lock)        ; Debounce check for M Menu key
    or a
    ret nz

    call Reset_VP
    ld a, grey
    ld (banner_colour), a
    call Draw_Banner_Image      ; from des_draw_banner.inc
    call Set_Game_VP

    SET_GCOL_BG_COL grey        ; set the gfx bg 
    SET_GCOL_COL black          ; set gfx col
    
    CLG

    call CleanStart
    jp Menu_Screen

    ret

;=========================
; I INVENTORY KEY
;=========================
.i_key_is_down:
    ld a, (inv_lock)        ; Debounce check for inventory key
    or a
    ret nz

    ld a, 1
    ld (inv_lock), a

    ld a, (ui_win_active)    ; if already open then return
    cp $01
    ret z

    ld a, 1
    ld (ui_win_active), a   ; This freezes the main_loop logic
    
    call playGlitch
    
    ; Hide the player and aliens
    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    ; Draw the UI
    call Draw_Inventory_Popup

    TEXT_AT_GRAPHICS_CURSOR     ; set text at graphics cursor.
   ; SET_GCOL_BG_COL black
   ; SET_GCOL_COL bright_cyan

    ; - Inventory -
    ld hl, SE0BB 
    ld bc, 0
    ld b, 106
    ld c, 106
    call print_frac_string

    ; call Display_Inv_Items; (Optional: call the item list here)
    ; --- No Item ---
    ld hl, SE0DB 
    ld bc, 0
    ld b, 88
    ld c, 192
    call print_frac_string

    TEXT_AT_TEXT_CURSOR


    ret
 
;-----------------------------------------------------------
; LOOK/SHOOT MODE
;-----------------------------------------------------------
Look_Shoot_Mode:

    ld a, (look_shoot_mode)
    cp 0
    jp z, .look_mode
    cp 1
    jp z, .shoot_mode
    ret

.look_mode:
    ; buffers 486-487, Tileset4
    ; Hide Shoot Indicators
    ; left
    Draw_Filled_Rect 225, 85, 232, 95, grey
    ; right
    Draw_Filled_Rect 203, 85, 210, 95, grey

  ;  Hide_Shoot_Indicator        ; from des_vdp_macros.inc
    ld de, 486                   ; 486 = left look indicator
    SELECT_BUFFER_DE
    Draw_Tile 184, 77            ; from des_vdp_macros.inc
    ld de, 487                   ; 487 = right look indicator
    SELECT_BUFFER_DE
    Draw_Tile 202, 77            ; from des_vdp_macros.inc

    ret

.shoot_mode:
    ; buffers 486-487, Tileset4
    ; Hide Look Indicators
    ; left
    Draw_Filled_Rect 182, 85, 189, 95, grey
    ; right
    Draw_Filled_Rect 203, 85, 210, 95, grey

    ld de, 486                     ; 486 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 204, 77              ; from des_vdp_macros.inc
    ld de, 487                     ; 487 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 224, 77              ; from des_vdp_macros.inc

    ret

;-----------------------------------------------------------
; Main Menu MODE
;-----------------------------------------------------------
Main_Menu_Mode:

    ld a, (main_menu_mode)
    cp $00
    jp z, .mm_new_mode
    cp $01
    jp z, .mm_cont_mode
    cp $02
    jp z, .mm_info_mode
    cp $03
    jp z, .mm_creds_mode
    cp $04
    jp z, .mm_quit_mode
   
    ret

.mm_new_mode:
    ; buffers 488-489, Tileset4
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                   ; 488 = left look indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 143            ; from des_vdp_macros.inc
    ld de, 489                   ; 489 = right look indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 143           ; from des_vdp_macros.
    
    ret

.mm_cont_mode:
    ; buffers 488-489, Tileset4
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 488 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 155              ; from des_vdp_macros.inc
    ld de, 489                     ; 489 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 155             ; from des_vdp_macros.inc

    ret

.mm_info_mode:
    ; buffers 488-489, Tileset4
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 488 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 167              ; from des_vdp_macros.inc
    ld de, 489                     ; 489 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 167             ; from des_vdp_macros.inc

    ret

.mm_creds_mode:
    ; buffers 488-489, Tileset4
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 488 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 179              ; from des_vdp_macros.inc
    ld de, 489                     ; 489 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 179             ; from des_vdp_macros.inc

    ret

.mm_quit_mode:
    ; buffers 488-489, Tileset4
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 488 = left shoot indicator 
    SELECT_BUFFER_DE
    Draw_Tile 73, 191              ; from des_vdp_macros.inc
    ld de, 489                     ; 489 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 191             ; from des_vdp_macros.inc
    
    ret

