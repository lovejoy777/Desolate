;-----------------------------------------------------------
; Check_Action_Triggers
;-----------------------------------------------------------
Check_Action_Triggers:

; --- "I" Key for Inventory ---
    ld b, KEY_I_BYTE        ; Ensure KEY_I_BYTE is defined in your key_defs
    ld c, KEY_I_BIT         ; Ensure KEY_I_BIT is defined
    call Is_Key_Held_Matrix
    jp nz, .i_key_is_down

    ; --- Check Spacebar ---
    ld b, KEY_SPACE_BYTE
    ld c, KEY_SPACE_BIT
    call Is_Key_Held_Matrix
    jp nz, .space_is_down

    ; --- Key is NOT held (Released) ---
    xor a
    ld (inv_lock), a
    ld (space_lock), a
    ret

.i_key_is_down:
    ld a, (inv_lock)        ; Debounce check for inventory key [cite: 117]
    or a
    ret nz
    ld a, 1
    ld (inv_lock), a

    ld a, (ui_win_active)   ; Check if a window is already open 
    or a
    jp nz, .close_window    ; If open, close it
    jp .open_inventory      ; Otherwise, open the inventory

    ; --- SUCCESS: OPEN INVENTORY ---
.open_inventory:
    ld a, 1
    ld (ui_win_active), a   ; This freezes the main_loop logic
    
    PLAY_SOUND 0, 127, 500, 10
    
    ; Hide the player
    Select_Sprite_8bit 0
    Hide_Sprite
    Update_GPU

    ; Draw the UI
    call Draw_Inv_Popup     ; Your routine to draw the inventory box

    ; print inventory title
    SET_TXT_COL black
    SET_TXT_BG_COL dark_grey
    TABTO 15, 13
    ld hl, Inv_Title_String
    call print_string
    TABTO 16, 24
    ld hl, Inv_Item_String
    call print_string
    ; call Display_Inv_Items; (Optional: call your item list here)
    ret

;=========================
; SPACEBAR ACTION KEY
;=========================

.space_is_down:
    ; --- Debounce Check ---
    ld a, (space_lock)
    or a
    ret nz

    ; Lock it now so we only trigger once per press
    ld a, 1
    ld (space_lock), a

    ; --- Toggle Logic ---
    ld a, (ui_win_active)
    or a
    jp nz, .close_window
    

.open_window_check:
    ;PLAY_SOUND 0, 127, 1000, 10
    ; --- Check Room ID ---
    ; If this is not 0, it currently exits. 
    ; Ensure you are actually in Room 0 when testing!
    ;ld a, (current_room_id)
    ;or a 
    ;ret nz                       

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
    ;PLAY_SOUND 0, 127, 1000, 30
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
    
    ; If not 26 or 28, exit
    ret

.terminal_found:
    ; --- SUCCESS: OPEN WINDOW ---
    ld a, 1
    ld (ui_win_active), a        
    ld hl, SE131  
    PLAY_SOUND 0, 127, 1000, 15
    PLAY_SOUND 0, 127, 1000, 15
    Select_Sprite_8bit 0
    Hide_Sprite
    Select_Sprite_8bit 1
    Hide_Sprite
    Select_Sprite_8bit 2
    Hide_Sprite
    call Draw_Terminal_Popup
    SET_TXT_BG_COL 8
    SET_TXT_COL black
    ;call Draw_Sm_Popup      ; Draw the 192x48 border
    ;call Display_Message         
    ret

.close_window:
    xor a
    ld (ui_win_active), a
    PLAY_SOUND 0, 127, 1000, 30
    ; --- Restore Screen ---
    SET_TXT_BG_COL 0
    SET_TXT_COL bright_white
    Select_Sprite_8bit 0
    Show_Sprite
    
    
    CLG
    call Draw_Room

    ret

