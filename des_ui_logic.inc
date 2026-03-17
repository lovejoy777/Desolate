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
; SPACEBAR ACTION KEY
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
    ld a, (up_lock)        ; Debounce check for Z look key
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
    PLAY_SOUND 0, 127, 500, 5
    call Main_Menu_Mode

.go_to_4_end:

    ret

.go_to_4:
    ld a, $04
    ld (main_menu_mode), a
    PLAY_SOUND 0, 127, 500, 5
    call Main_Menu_Mode
    jp .go_to_4_end

.down_key_is_down:
    ld a, (down_lock)        ; Debounce check for Z look key
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
    PLAY_SOUND 0, 127, 500, 5
    call Main_Menu_Mode

.go_to_0_end:

    ret

.go_to_0:
    ld a, $00
    ld (main_menu_mode), a
    PLAY_SOUND 0, 127, 500, 5
    call Main_Menu_Mode
    jp .go_to_0_end

.left_key_is_down:
    ld a, (left_lock)        ; Debounce check for Z look key
    or a
    ret nz

    ld a, 1
    ld (left_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    ret nz
    call Draw_Small_Popup
    PLAY_SOUND 0, 127, 500, 5
    
    ret
    

.right_key_is_down:
    ld a, (right_lock)        ; Debounce check for Z look key
    or a
    ret nz

    ld a, 1
    ld (right_lock), a

    ld a, (main_menu_is_active)    ; only carry on if main menu screen is open
    cp $01
    ret nz

    PLAY_SOUND 0, 127, 500, 5
    
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
    jp nz, .end_menu 

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
    PLAY_SOUND 0, 127, 500, 50
    ld a, 0
    ld (main_menu_is_active), a
    call CleanStart
    call Trigger_Room_Load

    ret

.cont:
    PLAY_SOUND 0, 127, 500, 50
    ld a, 0
    ld (main_menu_is_active), a
    call Trigger_Room_Load

    ret

.info:
    ;call CleanStart
    ;call Draw_Door_Lock_Popup_BG
    call Draw_Door_Lock_Popup
    PLAY_SOUND 0, 127, 500, 50

    ret

.creds:
    PLAY_SOUND 0, 127, 500, 50
    call Draw_Small_Popup
    
    ret

.quit:
    PLAY_SOUND 0, 127, 500, 50
    ld a, 0
    ld (main_menu_is_active), a
    call exit_back_to_mos

    ret

.end_menu:
    ; look/shoot
    ld a, (look_shoot_mode)
    cp $00
    jp z, .look
    cp $01
    jp z, .shoot

.look:
    ld a, (ui_win_active)

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

    PLAY_SOUND 0, 127, 1000, 15

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
    ld hl, startLaser
    ld bc, endLaser - startLaser
    rst.lil $18

    ret 

startLaser: 
    .db 23,0,$85                        ; do sound
    .db 2,4,-1                          ; channel

    .db 23,0,$85                        ; do sound
    .db 2,0, 127                        ; channel, volume
    .dw 125                             ; freq (ignored for samples)
    .dw 100                            ; duration (ignored for samples)
endLaser:

    ret

;=========================
; Z Look Mode KEY
;=========================
.z_key_is_down:
    ld a, (look_lock)        ; Debounce check for Z look key
    or a
    ret nz

    ld a, 1
    ld (look_lock), a
    ld a, 0
    ld (look_shoot_mode), a

    PLAY_SOUND 0, 127, 500, 5
    call Look_Shoot_Mode
    
    ret

;=========================
; X Shoot Mode KEY
;=========================
.x_key_is_down:
    ld a, (shoot_lock)        ; Debounce check for X shoot key
    or a
    ret nz

    ld a, 1
    ld (shoot_lock), a
    ld (look_shoot_mode), a

    PLAY_SOUND 0, 127, 500, 5
    call Look_Shoot_Mode

    ret

;=================================
; "C" Close All Popup Windows KEY
;=================================
.c_key_is_down:
    ld a, (ui_win_active)
    cp $01
    ret nz

    PLAY_SOUND 0, 127, 500, 5
    ld a, 0
    ld (ui_win_active), a

    CLG
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
    ld a, (menu_lock)        ; Debounce check for M Menu key
    or a
    ret nz

    ld a, 1
    ld (menu_lock), a
    ld (main_menu_is_active), a

    PLAY_SOUND 0, 127, 500, 5

    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

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
    
    PLAY_SOUND 0, 127, 500, 5
    
    ; Hide the player
    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite

    ; Draw the UI
    call Draw_Inventory_Popup

    ; print inventory title
    SET_TXT_COL black
    SET_TXT_BG_COL grey
    TABTO 15, 13
    ld hl, Inv_Title_String
    call print_string
    TABTO 16, 24
    ld hl, Inv_Item_String
    call print_string
    ; call Display_Inv_Items; (Optional: call the item list here)

    ; --- Restore Screen colours---
    SET_TXT_BG_COL black
    SET_TXT_COL bright_white 

    ret
 
;-----------------------------------------------------------
; LOOK/SHOOT MODE
;-----------------------------------------------------------
Look_Shoot_Mode:


    ld a, (look_shoot_mode)
    cp $00
    jp z, .look_mode
    cp $01
    jp z, .shoot_mode
    ret

.look_mode:
    ; Hide Shoot Indicators
    ; left
    Draw_Filled_Rect 225, 85, 232, 95, grey
    ; right
    Draw_Filled_Rect 203, 85, 210, 95, grey

  ;  Hide_Shoot_Indicator        ; from des_vdp_macros.inc
    ld de, 486                   ; 50 = left look indicator
    SELECT_BUFFER_DE
    Draw_Tile 184, 77           ; from des_vdp_macros.inc
    ld de, 487                   ; 50 = right look indicator
    SELECT_BUFFER_DE
    Draw_Tile 202, 77           ; from des_vdp_macros.inc

    ret

.shoot_mode:
    ; Hide Look Indicators
    ; left
    Draw_Filled_Rect 182, 85, 189, 95, grey
    ; right
    Draw_Filled_Rect 203, 85, 210, 95, grey

    ld de, 486                     ; 50 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 204, 77             ; from des_vdp_macros.inc
    ld de, 487                     ; 50 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 224, 77             ; from des_vdp_macros.inc

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
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                   ; 50 = left look indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 143           ; from des_vdp_macros.inc
    ld de, 489                   ; 50 = right look indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 143           ; from des_vdp_macros.
    
    ret

.mm_cont_mode:
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 50 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 155            ; from des_vdp_macros.inc
    ld de, 489                     ; 50 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 155             ; from des_vdp_macros.inc

    ret

.mm_info_mode:
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 50 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 167            ; from des_vdp_macros.inc
    ld de, 489                     ; 50 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 167             ; from des_vdp_macros.inc

    ret

.mm_creds_mode:
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 50 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 179            ; from des_vdp_macros.inc
    ld de, 489                     ; 50 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 179             ; from des_vdp_macros.inc

    ret

.mm_quit_mode:
    ; Hide Indicators Left
    Draw_Filled_Rect 70, 140, 82, 202, grey
    ; Hide Indicators Right
    Draw_Filled_Rect 140, 140, 152, 202, grey

    ld de, 488                     ; 50 = left shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 73, 191            ; from des_vdp_macros.inc
    ld de, 489                     ; 50 = right shoot indicator
    SELECT_BUFFER_DE
    Draw_Tile 143, 191             ; from des_vdp_macros.inc
    
    ret