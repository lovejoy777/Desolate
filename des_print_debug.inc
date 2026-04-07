;--------------------------------------------------------
; DEBUG TEMP CODE
;---------------------------------------------------------
; Debug_Overlay_Titles:
;---------------------------------------------------------
Debug_Overlay_Titles:
    ld a, (debug_is_active)
    cp $01
    ret nz

    ;----------------------------
    ; Current Room Number Title
    ;----------------------------
    TABTO 0, 0                  
    ld hl, Debug_CRN_String         
    call print_string     
    
    ;--------------------------
    ; Small Alien Active Title
    ;--------------------------
    TABTO 0, 1                  
    ld hl, Debug_SAA_String         
    call print_string
    
    ;--------------------------
    ; Big alien active Title
    ;--------------------------
    TABTO 0, 2              
    ld hl, Debug_BAA_String         
    call print_string

    ;--------------------------------
    ; Player current direction Title
    ;--------------------------------
    TABTO 0, 3                  
    ld hl, Debug_PCD_String         
    call print_string

    ;----------------------------
    ; Tile ID under Player Title
    ;----------------------------
    TABTO 0, 4               
    ld hl, Debug_TPO_String         
    call print_string

    ;-----------------------------------
    ; Tile ID In Front of Player Title
    ;-----------------------------------
    TABTO 0, 5                  
    ld hl, Debug_TIP_String         
    call print_string

    ;-----------------------------------
    ; Look/Shoot Mode
    ;-----------------------------------
    TABTO 0, 6                  
    ld hl, Debug_LSM_String         
    call print_string

    ;-----------------------------------
    ; Small Alien HP
    ;-----------------------------------
    TABTO 0, 7                  
    ld hl, Debug_SHP_String         
    call print_string

    ;-----------------------------------
    ; Big Alien HP
    ;-----------------------------------
    TABTO 0, 8                  
    ld hl, Debug_BHP_String         
    call print_string

    ;-----------------------------------
    ; Aliens Amount Killed
    ;-----------------------------------
    TABTO 0, 9                  
    ld hl, Debug_KIL_String         
    call print_string


    ret
;---------------------------------------------------------
; Debug_Overlay Values:
;---------------------------------------------------------
Debug_Overlay_Values:
    ld a, (debug_is_active)
    cp $01
    ret nz
    ;--------------------------
    ; Current Room Number
    ;--------------------------
    TABTO 4, 0
    SET_TXT_COL bright_white
    ld a, (current_room_id)
    call Print_A_Number
    ;--------------------------
    ; Small Alien Active
    ;--------------------------
    TABTO 4, 1
    SET_TXT_COL bright_white
    ld a, (small_alien_is_active)
    call Print_A_Number
    ;--------------------------
    ; Big alien active
    ;--------------------------
    TABTO 4, 2
    SET_TXT_COL bright_white
    ld a, (big_alien_is_active)
    call Print_A_Number
    ;--------------------------
    ; Player current direction
    ;--------------------------
    TABTO 4, 3
    SET_TXT_COL bright_white
    ld a, (player_dir)
    call Print_A_Number
    ;--------------------------
    ; Tile ID under Player
    ;--------------------------
    TABTO 4, 4
    SET_TXT_COL bright_white
    ld hl, (player_x)
    ld de, (player_y)
    ; Normalize base to Center-Mass (16, 24)
    ld a, l
    add a, 16           
    ld l, a
    ld a, e
    add a, 24          
    ld e, a
    call get_tile_at_coords 
    call Print_A_Number
    ;------------------------------
    ; Tile ID In Front of Player
    ;------------------------------
    TABTO 4, 5
    SET_TXT_COL bright_white
    ld hl, (player_x)
    ld de, (player_y)
    ; Normalize base to Center-Mass (16, 24)
    ld a, l
    add a, 16
    ld l, a
    ld a, e
    add a, 24
    ld e, a
    ld a, (player_dir)
    ; Branching for look-ahead from the new center base Decode_Room_Metadata
    cp 0                ; Down
    jr z, .dbg_down
    cp 1                ; Up
    jr z, .dbg_up
    cp 2               ; RIGHT
    jr z, .dbg_right
    cp 3                ; LEFT
    jr z, .dbg_left
    jr .dbg_print       ; Fallback
.dbg_down:
    ld a, e
    add 8              ; Look 8px Down from center
    ld e, a
    jr .dbg_print
.dbg_up:
    ld a, e
    sub a, 8           ; Look 8px Up from center
    ld e, a
    jr .dbg_print
.dbg_right:
    ld a, l
    add a, 10           ; Look 10px right from center
    ld l, a
    jr .dbg_print
.dbg_left:
    ld a, l
    sub 10              ; Look 10px left from center
    ld l, a

.dbg_print:
    call get_tile_at_coords
    call Print_A_Number

    ;--------------------------
    ; Look Mode
    ;--------------------------
    TABTO 4, 6
    SET_TXT_COL bright_white
    ld a, (look_shoot_mode)
    call Print_A_Number

    ;--------------------------
    ; Small Alien HP
    ;--------------------------
    TABTO 4, 7
    SET_TXT_COL bright_white
    ld a, (small_alien_hp)
    call Print_A_Number

    ;--------------------------
    ; Big Alien health
    ;--------------------------
    TABTO 4, 8
    SET_TXT_COL bright_white
    ld a, (big_alien_hp)
    call Print_A_Number

    ;--------------------------
    ; Aliens Killed Draw_Health_Value
    ;--------------------------
    TABTO 4, 9
    SET_TXT_COL bright_white
    ld a, (alien_amount_killed)
    call Print_A_Number

    ret