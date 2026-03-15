; =============================================================================
; FILE: des_load_assets.inc
; Purpose: Load Desolate's Assets.
; =============================================================================
Load_All_Assets:

    call Load_Banner_Asset
    call Load_Player_Assets
    call Load_Small_Alien_Asset
    call Load_Big_Alien_Asset

    ; Load Room Tiles from Tileset1/des_data.inc
    call Load_Tileset1_Tiles
    ; Load Menu Tiles from Tileset2/des_data.inc
    call Load_Tileset2_Tiles
    ; Load UI Tiles from Tileset4/des_data.inc
    call Load_Tileset4_Tiles

    
    Activate_amount_sprites 3         ; 0=player, 1=small alien, 2=big alien

    call Load_Inventoy_window
    call Load_Terminal_window
    call Load_Small_Popup_Asset

    ;-----------
    ; SFX
    ;-----------
    call loadLaser                    ; Shoot sfx
    
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
; Main Menu Asset
;------------------------
; Main Menu Screen Loader
Load_Main_Menu:
    Select_Buffer_8bit 40
    Write_To_Buffer_8bit 192, 128
    ; Stream the Pixels
    ld hl, Main_Menu_Pixels
    ld bc, 3072 ;3072       ; 192px * 128px / 8 = 3072 bytes
.stream_m:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_m
    ret

;------------------------
; Banner Asset buffer 30
;------------------------
; Desolate Banner Loader
Load_Banner_Asset:
    Select_Buffer_8bit 30
    Write_To_Buffer_8bit 192, 48
    ; Stream the Pixels
    ld hl, Banner_Pixels
    ld bc, 36864        ; 192 * 48 * 4 bytes (rgba)
.stream_b:
    ld a, (hl)
    rst.lil $10
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .stream_b
    ret

;---------------------------------------------------
; Room Tile Assets, (122) 12x8 Grid of 16x16 tiles.
; Buffers: 100-221
;---------------------------------------------------
; Tileset1 Loader
Load_Tileset1_Tiles:
    ld ix, Tileset1            ; Point to the original data blocks
    ld de, 100                 ; Start Buffer 100 - 221
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

;------------------------------------------------------------------------------
; Room Tile Assets, (122) 12x8 Grid of 16x16 tiles.
; Load Menu_ :  (126 tiles)(Masked) as VDU Bitmaps
; Buffers ID 300-425
;------------------------------------------------------------------------------
Load_Tileset2_Tiles:
    ld ix, Tileset2            ; Point to the original data blocks
    ld de, 300                 ; Start Buffer 300 - 425
    ld bc, 126                 ; Total tiles
.tile_loop1:
    push bc
    push de 
    
    SELECT_BUFFER_DE
    Write_To_Buffer_16bit d, e 

    ld b, 16                   ; 16 rows
.row_loop1:
    ld a, (ix+1)               ; Get Pixel Data Byte 1 (Skip first mask)
    rst.lil $10
    ld a, (ix+3)               ; Get Pixel Data Byte 2 (Skip second mask)
    rst.lil $10
    lea ix, ix+4               ; Move to next row in source (4 bytes total)
    djnz .row_loop1

    ; Create the bitmap as 16x16 Paletted (Type 2)
    Create_Bitmap 16, 16, 2

    pop de
    inc de                     ; Next Buffer ID
    pop bc
    dec bc
    ld a, b
    or c
    jr nz, .tile_loop1
    ret

;--------------------------------------------------------------------------------------
; Tileset 4 Assets (14) 16x16 tiles. same as Tileset3 but shoot/look indicators edited
;--------------------------------------------------------------------------------------
; Tileset 4 Loader
Load_Tileset4_Tiles:
    ld ix, Tileset4            ; Point to the original data blocks
    ld de, 470                 ; Start at Buffer 470 - 483
    ld b, 14                   ; Total tiles ( from des_tiles.inc)
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
; Player Loader buffers 0 - 11
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
; Alien Loader buffer
Load_Small_Alien_Asset:
    ld ix, Small_Alien_Frame_List
    ld d, 50
.sa_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    
    Select_Buffer_8bit d
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
    ld d, 52
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
Load_Small_Popup_Asset:
    push de
    ld de, 80
    SELECT_BUFFER_DE
    pop de

    Write_To_Buffer_8bit 192, 48
    ld hl, Small_Popup_Pixels
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
    ;Select_Buffer_8bit 81
    push de
    ld de, 81
    SELECT_BUFFER_DE
    pop de

    Write_To_Buffer_8bit 192, 56
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
    push de
    ld de, 82
    SELECT_BUFFER_DE
    pop de

    Write_To_Buffer_8bit 192, 56
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
    push de
    ld de, 83
    SELECT_BUFFER_DE
    pop de

    Write_To_Buffer_8bit 192, 64  
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
    push de
    ld de, 84
    SELECT_BUFFER_DE
    pop de

    Write_To_Buffer_8bit 192, 64
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

; --- Main Menu Screen Data ---
Main_Menu_Pixels: incbin "popups/main_menu.data"
; --- Banner Data ---
Banner_Pixels: incbin "sprites/banner_cyan.data"
;---- Small Message Popup Window
Small_Popup_Pixels: incbin "popups/sm_popup_gs.data"
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
Small_Alien_Frame_List:
    .dl f50, f51
    .db $FF
f50: incbin "sprites/023.data"    ; Frame 1
f51: incbin "sprites/033.data"    ; Frame 2

; --- Big Alien Data List ---
; --- Update the Big Alien animation List and data ---
big_Alien_Frame_List:
    .dl ba52, ba53
    .db $FF
ba52: incbin "sprites/038.data"   ; Frame 1
ba53: incbin "sprites/039.data"   ; Frame 2

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
