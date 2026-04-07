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
;  Main Menu KEYs
;=========================
.up_key_is_down:
    ; Debounce check for up cursor key
    ld a, (up_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (up_lock), a

    ; only carry on if main menu screen is open
    ld a, (main_menu_is_active)    
    cp $01
    ret nz

    ; sfx
    call playMenu_nav

    ld a, (main_menu_mode)
    cp $00
    jp z, .loopToBottom

    sub 1
    ld (main_menu_mode), a

    call Main_Menu_Mode

.upKeyEnd:

    ret

.loopToBottom:
    ld a, $04
    ld (main_menu_mode), a
    call Main_Menu_Mode
    jp .upKeyEnd

.down_key_is_down:
    ; Debounce check for down cursor key
    ld a, (down_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (down_lock), a

    ; only carry on if main menu screen is open
    ld a, (main_menu_is_active)    
    cp $01
    ret nz

    ; sfx
    call playMenu_nav

    ld a, (main_menu_mode)
    cp $04
    jp z, .loopToTop

    add 1
    ld (main_menu_mode), a

    call Main_Menu_Mode

.downKeyEnd:

    ret

.loopToTop:
    ld a, $00
    ld (main_menu_mode), a
    call Main_Menu_Mode
    jp .downKeyEnd

.left_key_is_down:
    ; Debounce check for left cursor key
    ld a, (left_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (left_lock), a

    ; only carry on if main menu screen is open
    ld a, (main_menu_is_active)    
    cp $01
    ret nz

    ; sfx
    call playMenu_nav
    
    ret
    

.right_key_is_down:
    ; Debounce check for right cursor key
    ld a, (right_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (right_lock), a
    ; only carry on if main menu screen is open
    ld a, (main_menu_is_active)    
    cp $01
    ret nz

    ; sfx
    call playMenu_nav
    
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

    ;ld a, 1
    ;ld (ui_win_active), a

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
    ; sfx
    call playSelect
   
    call story_mode

    ld a, (story_page)
    ld b, a
    inc b
    ld a, b
    ld (story_page), a

    ret

.cont:
    ; sfx
    call playSelect

    call Reset_VP
    ld a, grey
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP

    ld a, 0
    ; reset window flags
    ld (ui_win_active), a
    ld (main_menu_is_active), a
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a
   
    call Trigger_Room_Load

    ret

.info:
    ; sfx
    call playGlitch

    call Reset_VP
    ld a, bright_cyan
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP

    call Draw_Data_Cart_Popup
    TEXT_AT_GRAPHICS_CURSOR

    ld a, 1
    ld (ui_win_active), a
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
    ; sfx
    call playSelect

    ld a, 1
    ld (ui_win_active), a
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
    ld b, 106
    ld c, 174
    call print_frac_string
    ; 2026 +
    ld hl, SCR43 
    ld b, 134
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
    ; set ui win flag
    ld a, 1
    ld (ui_win_active), a        

   ; ld hl, SE131  ; message from des_strings

    ; sfx
    call playGlitch
    
    ; hide all sprites
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
    ; check if ui window is open
    ld a, (ui_win_active)
    cp $01
    ret z

    call Shoot

    ret 


;=========================
; Z Look Mode KEY
;=========================
.z_key_is_down:
    ; Debounce check for Z look key
    ld a, (look_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (look_lock), a

    ; return if ui window is open
    ld a, (ui_win_active)
    cp $01
    ret z

    ; set look mode
    ld a, 0
    ld (look_shoot_mode), a

    ; sfx
    call playSelect

    call Look_Shoot_Mode
    
    ret

;=========================
; X Shoot Mode KEY
;=========================
.x_key_is_down:
    ; Debounce check for X shoot key
    ld a, (shoot_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (shoot_lock), a
    
    ; return if ui window is open
    ld a, (ui_win_active)
    cp $01
    ret z
    
    ; set shoot mode
    ld a, 1
    ld (look_shoot_mode), a

    ; sfx
    call playSelect

    call Look_Shoot_Mode

    ret

;=================================
; "C" Close All Popup Windows KEY
;=================================
.c_key_is_down:
    call playSelect
    ; Debounce check for C Close key
    ld a, (close_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (close_lock), a

    ; return if ui window is not open
    ld a, (ui_win_active)
    cp $01
    ret nz

    ;call playSelect

    ; if menu flag is set then return to the menu.
    ld a, (main_menu_is_active)
    cp $01
    jp z, Menu_Screen        ; from des_main.asm

    ; if not menu then fall through to .continue_game

.continue_game:

    CLG
    ; reset ui window & menu flags
    ld a, 0
    ld (ui_win_active), a
    ld (main_menu_is_active), a
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a

    call Draw_Room
    call Draw_Health_Value         ; from main_subs.inc
    call Look_Shoot_Mode           ; from des_ui_logic.inc

    Select_Sprite_8bit playerSprite
    Show_Sprite

    ret

;=========================
; "M" Menu KEY
;=========================
.m_key_is_down:
    ; Debounce check for M Menu key
    ld a, (menu_lock)        
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (menu_lock), a

    ; Check if player is alive
    ld a, (player_is_alive) 
    cp $01
    jp nz, .player_is_dead

    ; sfx
    call playSelect

    ; hide all sprites
    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    jp Menu_Screen       ; from des_main.asm
    
    ret

.player_is_dead:
    ; "m" key while player is dead.

    call Reset_VP
    ld a, grey
    ld (banner_colour), a

    call Draw_Banner_Image      ; from des_draw_banner.inc
    call Set_Game_VP

    SET_GCOL_BG_COL grey        ; set the gfx bg 
    SET_GCOL_COL black          ; set gfx col
    
    CLG

    call CleanStart         ; from des_main_subs.inc
    jp Menu_Screen          ; from des_main.asm

    ret

;=========================
; I INVENTORY KEY
;=========================
.i_key_is_down:
    ; Debounce check for inventory key
    ld a, (inv_lock)
    or a
    ret nz
    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (inv_lock), a

    ; if already open then return
    ld a, (ui_win_active)    
    cp $01
    ret z

    ; This freezes the main_loop logic
    ld a, 1
    ld (ui_win_active), a  
    
    ; sfx
    call playGlitch

    ; Hide all sprites
    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    ; Draw the Inventory Popup
    call Draw_Inventory_Popup

    ; set text at graphics cursor.
    TEXT_AT_GRAPHICS_CURSOR    

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

