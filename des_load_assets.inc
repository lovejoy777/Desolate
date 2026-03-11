; =============================================================================
; FILE: des_load_assets.inc
; Purpose: Load Desolate's Assets.
; =============================================================================
Load_All_Assets:

    call Load_Banner_Asset
    call Load_Room_Tiles
    call Load_Tileset4_Tiles
    call Load_Player_Assets
    call Load_Sm_Alien_Asset
    call Load_Big_Alien_Asset
    call loadLaser                    ; Shoot sfx
    
    Activate_amount_sprites 3         ; 0=player, 1=small alien, 2=big alien

    call Load_Inventoy_window
    call Load_Terminal_window
    call Load_Sm_Popup_Asset

    ret

Load_Inventoy_window:
    call Load_Invtop_Popup_Asset
    call Load_Invbot_Popup_Asset
    ret

Load_Terminal_window:
    call Load_Termtop_Popup_Asset
    call Load_Termbot_Popup_Asset
    ret
    
;------------------------
; Banner Asset
;------------------------
; Desolate Banner Loader
Load_Banner_Asset:
    Select_Buffer_8bit 76
    Write_To_Buffer_8bit 192, 48
    ; Stream the Pixels
    ld hl, Banner_Pixels
    ld bc, 36864        ; 192 * 48 * 4 bytes [cite: 127]
.stream_b:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_b
    ret

;----------------------------------------------
; Room Assets (122) 12x8 Grid of 16x16 tiles.
;----------------------------------------------
; Tileset 1 Loader
Load_Room_Tiles:
    ld ix, Tileset1            ; Point to the original data blocks
    ld de, 100                 ; Start Buffer 100
    ld b, 122                  ; Total tiles (Matches des_tiles.inc)
.tile_loop:
    push bc
    push de 
    ; --- 1. Select Buffer with 16 bit id ---
    SELECT_BUFFER_DE
    ; --- 2. Upload Data (VDU 23, 0, $A0) ---
    Write_To_Buffer_16bit d, e
    push ix
    pop hl              
    ld bc, 32           ; Original 2bpp tile size
    rst.lil $18         ; Stream pixels
    ; --- 3. Create Bitmap (Type 2 = Paletted 2bpp) ---
    Create_Bitmap 16, 16, 2

    pop de
    inc de
    dec b              
    ld bc, 32           
    add ix, bc          ; Move IX to the next 32-byte tile
    pop bc
    
    djnz .tile_loop
    ret

;------------------------------------------------------------
; Tileset 4 Assets (14) 16x16 tiles.
;--------------------------------------------------------------
; Tileset 4 Loader
Load_Tileset4_Tiles:
    ld ix, Tileset4           ; Point to the original data blocks
    ld de, 50                 ; Start at Buffer 50
    ld b, 14                  ; Total tiles (Matches des_tiles.inc)
.tileset4_loop:
    push bc
    push de 
    ; --- 1. Select Buffer with 16 bit id ---
    SELECT_BUFFER_DE
    ; --- 2. Upload Data (VDU 23, 0, $A0) ---
    Write_To_Buffer_16bit d, e
    push ix
    pop hl              
    ld bc, 32           ; Original 2bpp tile size
    rst.lil $18         ; Stream pixels
    ; --- 3. Create Bitmap (Type 2 = Paletted 2bpp) ---
    Create_Bitmap 16, 16, 2

    pop de
    inc de
    dec b              
    ld bc, 32           
    add ix, bc          ; Move IX to the next 32-byte tile
    pop bc
    
    djnz .tileset4_loop
    ret

;--------------------------------
; Player Assets, Sprite 0
;--------------------------------
; Player Loader
Load_Player_Assets:
    ld ix, Player_Frame_List
    ld d, 0
.p_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    Select_Buffer_8bit d
    Write_To_Buffer_8bit 32, 32
    ; Bitmap data stream into buffer.
    ld hl, (ix+0)
    ld bc, 4096        ; size 32x32x4, rgba
.stream_p:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jp nz, .stream_p
    Select_Sprite_8bit playerSprite
    Add_Frame_To_Sprite d
    Hide_Sprite

    inc d
    inc ix
    inc ix
    inc ix
    jp .p_loop
    ret

;---------------------------------
; Small Alien Assets, Sprite 1
;---------------------------------
; Alien Loader
Load_Sm_Alien_Asset:
    ld ix, Sm_Alien_Frame_List
    ld d, 63
.sa_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    ; Select Buffer id
    Select_Buffer_8bit d
    ; Write Block to buffer width & height
    Write_To_Buffer_8bit 16, 16
    ld hl, (ix+0)
    ld bc, 1024
.stream_sa:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jp nz, .stream_sa
    Select_Sprite_8bit smAlienSprite
    Add_Frame_To_Sprite d
    Hide_Sprite

    inc d
    inc ix
    inc ix
    inc ix
    jp .sa_loop
    ret

;---------------------------------
; Big Alien Assets, Sprite 2
;---------------------------------
; Big Alien Loader
Load_Big_Alien_Asset:
    ld ix, big_Alien_Frame_List
    ld d, 38
.ba_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    Select_Buffer_8bit d
    Write_To_Buffer_8bit 16, 32
    ld hl, (ix+0)
    ld bc, 2048
.stream_ba:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jp nz, .stream_ba
    Select_Sprite_8bit bigAlienSprite
    Add_Frame_To_Sprite d
    Hide_Sprite

    inc d
    inc ix
    inc ix
    inc ix
    jp .ba_loop
    ret

;------------------------------------------------------------
; Small Pop up window 192x48 (12x3 tiles)
;--------------------------------------------------------------
Load_Sm_Popup_Asset:
    Select_Buffer_8bit 80
    Write_To_Buffer_8bit 192, 48
    ; 2a. Stream the Pixels
    ld hl, Sm_Popup_Pixels
    ld bc, 36864        ; 192 * 48 * 4 bytes = 36864 bytes
.stream_sp:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_sp
    ret

;-----------------------------------------------------------------
; Inventory and information Pop up window 192x56 x 2 (12x4 tiles)
;-----------------------------------------------------------------
; Top half
Load_Invtop_Popup_Asset:
    Select_Buffer_8bit 81
    Write_To_Buffer_8bit 192, 56
    ; 2a. Stream the Pixels
    ld hl, Invtop_Popup_Pixels
    ld bc, 43008        ; 192 * 56 * 4 bytes, top half of the window
.stream_itp:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_itp
    ret
; Bottom half
Load_Invbot_Popup_Asset:
    Select_Buffer_8bit 82
    Write_To_Buffer_8bit 192, 56
    ; 2a. Stream the Pixels
    ld hl, Invbot_Popup_Pixels
    ld bc, 43008        ; 192 * 56 * 4 bytes, bottom half of the window
.stream_ibp:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_ibp
    ret

;-----------------------------------------------------------------
; Terminal Screen and  Pop up window 192x56 x 2 (12x6 tiles)
;-----------------------------------------------------------------
; Top half
Load_Termtop_Popup_Asset:
    Select_Buffer_8bit 83
    Write_To_Buffer_8bit 192, 64              
    ; 2a. Stream the Pixels
    ld hl, Termtop_Popup_Pixels
    ld bc, 49152        ; 192 * 64 * 4 bytes, top half of the window
.stream_ttp:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_ttp
    ret
; Bottom half
Load_Termbot_Popup_Asset:
    Select_Buffer_8bit 84
    Write_To_Buffer_8bit 192, 64
    ; 2a. Stream the Pixels
    ld hl, Termbot_Popup_Pixels
    ld bc, 49152        ; 192 * 64 * 4 bytes, bottom half of the window
.stream_tbp:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_tbp
    ret

; --- Banner Data ---
Banner_Pixels: incbin "sprites/banner_cyan.data"
;---- Small Message Popup Window
Sm_Popup_Pixels: incbin "popups/sm_popup_gs.data"
;---- Inventory and Info Message Popup Window top half
Invtop_Popup_Pixels: incbin "popups/invtop_popup_gs.data"
;---- Inventory and Info Message Popup Window bottom half
Invbot_Popup_Pixels: incbin "popups/invbot_popup_gs.data"
;---- Terminal Screen  Popup Window top half
Termtop_Popup_Pixels: incbin "popups/termtop_popup_gs.data"
;---- Terminal Screen Popup Window bottom half
Termbot_Popup_Pixels: incbin "popups/termbot_popup_gs.data"

; --- Player animation List and data---
Player_Frame_List:
    .dl f36, f37, f38, f39, f40, f41, f42, f43, f44, f45, f46, f47
    .db $FF
f36: incbin "32x32_gs_sprites/036.data"    ; Frame 0  south facing
f37: incbin "32x32_gs_sprites/037.data"    ; Frame 1  south facing
f38: incbin "32x32_gs_sprites/038.data"    ; Frame 2  south facing
f39: incbin "32x32_gs_sprites/039.data"    ; Frame 3  north facing
f40: incbin "32x32_gs_sprites/040.data"    ; Frame 4  north facing
f41: incbin "32x32_gs_sprites/041.data"    ; Frame 5  north facing
f42: incbin "32x32_gs_sprites/042.data"    ; Frame 6  east  facing
f43: incbin "32x32_gs_sprites/043.data"    ; Frame 7  east  facing
f44: incbin "32x32_gs_sprites/044.data"    ; Frame 8  east  facing
f45: incbin "32x32_gs_sprites/045.data"    ; Frame 9  west  facing
f46: incbin "32x32_gs_sprites/046.data"    ; Frame 10 west  facing
f47: incbin "32x32_gs_sprites/047.data"    ; Frame 11 west  facing

; --- Small Alien animation List and data---
Sm_Alien_Frame_List:
    .dl f63, f64
    .db $FF
f63: incbin "sprites/023.data"    ; Frame 1
f64: incbin "sprites/033.data"    ; Frame 2

; --- Big Alien Data List ---
; --- Update the Big Alien animation List and data ---
big_Alien_Frame_List:
    .dl ba38, ba39
    .db $FF
ba38: incbin "sprites/038.data"   ; Frame 1
ba39: incbin "sprites/039.data"   ; Frame 2

;----------------
; Sound SFX
;----------------
loadLaser:                              ; for loading shoot sfx (laser)
    ld hl, laserStr                     ; start of data to send
    ld bc, endLaserStr - laserStr       ; length of data to send
    rst.lil $18                         ; send data
    ret 
laserStr:
    .db     23,0,85h                    ; audio command
    .db     -1, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     3848                        ; length in bytes (LONG)
    incbin "sounds/laser.raw"           ; raw file path
endLaserStr:
