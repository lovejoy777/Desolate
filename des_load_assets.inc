; =============================================================================
; FILE: des_load_assets.inc
; Purpose: Load Desolate's Assets.
; =============================================================================
Load_All_Assets:

    ; Load Sprites
    call Load_Player_Assets
    call Load_Small_Alien_Assets
    call Load_Big_Alien_Assets
    call Load_Bullet_Assets

    ; Load banner from pbm image
    call Load_Banner_Asset
    ; Load Room Tiles from Tileset1/des_data.inc
    call Load_Tileset1_Tiles
    ; Load Menu Tiles from Tileset2/des_data.inc
    call Load_Tileset2_Tiles
    ; Load Sprites Tiles from Tileset3/des_data.inc
    call Load_Tileset3_Tiles
    ; Load UI Tiles from Tileset4/des_data.inc
    call Load_Tileset4_Tiles

    ; 0=player, 1=small alien, 2=big alien, 3=bullet 
    Activate_amount_sprites 4         

    ; Load SFX:
    call loadMenu_nav
    ;call loadNav_tick
    call loadSelect
    call loadLaser
    call loadBump
    call loadDoor
    call loadError
    call loadGlitch
    
    ret

;---------------------------------------------------
; Room Tile Assets, (122) 12x8 Grid of 16x16 tiles.
; Load Rooms from des_data Tileset1 (no mask)
; Buffers: 100-221
;---------------------------------------------------
; Tileset1 Loader, buffers 100-221
Load_Tileset1_Tiles:
    ld ix, Tileset1            ; Point to the original data blocks
    ld de, 100                 ; Start Buffer 100 - 221
    ld b, 122                  ; Total tiles (Matches des_tiles.inc)
    jp tile_loop_nomask

    ret 

;------------------------------------------------------------------------------
; Menu Tile Assets, (126) 12x8 Grid of 16x16 tiles.
; Load Menu_ :  from des_data Tileset2 (masked)
; Buffers ID 300-425
;------------------------------------------------------------------------------
; Tileset2 Loader, buffers 300-425
Load_Tileset2_Tiles:
    ld ix, Tileset2            ; Point to the original data blocks
    ld de, 300                 ; Start Buffer 300 - 425
    ld bc, 126                 ; Total tiles
    jp tile_loop_masked

    ret

;------------------------------------------------------------------------------
; Sprite Tile Assets, (36) of 16x16 pixels.
; Load Sprites :  from des_data Tileset3 (masked)
; Buffers ID 430-465
;------------------------------------------------------------------------------
; Tileset3 Loader, buffers 430-465
Load_Tileset3_Tiles:
    ld ix, Tileset3            ; Point to the data blocks
    ld de, 430                 ; Start Buffer 430 - 465
    ld bc, 36                  ; Total tiles
    jp tile_loop_masked

    ret

;------------------------------------------------------------------------------
; Inventory/UI Assets, (21) of 16x16 pixels.
; Load Sprites :  from des_data Tileset3 (no masked)
; Buffers ID 470-499
;------------------------------------------------------------------------------
; Tileset 4 Loader, buffers 470-489
Load_Tileset4_Tiles:
    ld ix, Tileset4            ; Point to the original data blocks
    ld de, 470                 ; Start at Buffer 470 - 490
    ld b, 21                   ; Total tiles ( from des_tiles.inc)
    jp tile_loop_nomask

    ret

;------------------------------------------------------------------------------
; Loop through the pixels 
; Load Bitmaps to Buffers:  
; from des_data Tilesets (no mask)
;------------------------------------------------------------------------------
tile_loop_nomask:
    push bc
    push de 
    SELECT_BUFFER_DE
    ; --- 1. Upload Data (VDU 23, 0, $A0) ---
    Write_To_Buffer_16bit d, e
    push ix
    pop hl              
    ld bc, 32           ; Original 2bpp tile size
    rst.lil $18         ; Stream pixels
    ; --- 2. Create Bitmap (Type 2 = Paletted 2bpp) ---
    Create_Bitmap 16, 16, 2
    pop de
    inc de
    dec b              
    ld bc, 32           
    add ix, bc          ; Move IX to the next 32-byte tile
    pop bc
    
    djnz tile_loop_nomask

    ret

;------------------------------------------------------------------------------
; Loop through the pixels 
; Load Bitmaps to Buffers:  
; from des_data Tilesets (masked)
;------------------------------------------------------------------------------
tile_loop_masked:
    push bc
    push de 
    SELECT_BUFFER_DE
    ; 1. Upload Data (VDU 23, 0, $A0) 
    Write_To_Buffer_16bit d, e 
    ld b, 16                   ; 16 rows
.row_loop_masked:
    ld a, (ix+1)               ; Get Pixel Data Byte 1 (Skip first mask)
    rst.lil $10
    ld a, (ix+3)               ; Get Pixel Data Byte 2 (Skip second mask)
    rst.lil $10
    lea ix, ix+4               ; Move to next row in source (4 bytes total)
    djnz .row_loop_masked
    ; 2. Create Bitmap (Type 2 = Paletted 2bpp)
    Create_Bitmap 16, 16, 2
    pop de
    inc de                     ; Next Buffer ID
    pop bc
    dec bc
    ld a, b
    or c
    jr nz, tile_loop_masked

    ret

;------------------------
; Banner Asset
;------------------------
; Banner Loader. using mono pbm file, buffer 30
Load_Banner_Asset:
    ld hl, loadGraphics				            ; start of data to send
	ld bc, endLoadGraphics - loadGraphics		; length of data to send
	rst.lil $18					             

	; take a break to catch up in 2 batches

	ld hl, loadGraphics				            ; start of data to send
	ld bc, endLoadGraphics - loadGraphics		; length of data to send
	rst.lil $18					                
    SET_GCOL_COL black

    ret

loadGraphics:	
	;                  id, width, height,    data,        colourCode
	MAKEBUFFEREDBITMAP 30, 192, 48, "menus/banner_02.pbm", 0 
    
endLoadGraphics:

;------------------------------------------
; Player Assets, Sprite 0, RGBA8888 format
;------------------------------------------
; Player Loader, buffers 0-11
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

;-----------------------------------------------
; Small Alien Assets, Sprite 1, RGBA8888 format
;-----------------------------------------------
; Small Alien Loader, buffers 50 & 51
Load_Small_Alien_Assets:
    ld ix, Small_Alien_Frame_List
    ld d, 50
.sa_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    
    Select_Buffer_8bit d
    Write_To_Buffer_8bit 16, 16

    ld hl, (ix+0)
    ld bc, 1024     ; size 16x16x4, rgba
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
; Big Alien Assets, Sprite 2, 
;---------------------------------
; Big Alien Loader, buffers 52 & 53
Load_Big_Alien_Assets:
    ld ix, big_Alien_Frame_List
    ld d, 52
.ba_loop:
    ld a, (ix+0)
    cp $FF
    ret z              

    Select_Buffer_8bit d
    Write_To_Buffer_8bit 16, 32

    ld hl, (ix+0)
    ld bc, 2048     ; size 16x32x4, rgba
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

;---------------------------------
; Bullet Assets, Sprite 3
;---------------------------------
; Bullet Loader, using mono pbm file, buffers 40-41
Load_Bullet_Assets:
    Select_Sprite_8bit bulletSprite
    ; North/South Bullet
    ld hl, loadBullet1				            ; start of data to send
	ld bc, endLoadBullet1 - loadBullet1		    ; length of data to send
	rst.lil $18	
    Add_Frame_To_Sprite	40	
    ; East/West Bullet
    ld hl, loadBullet2				            ; start of data to send
	ld bc, endLoadBullet2 - loadBullet2		    ; length of data to send
	rst.lil $18
    Add_Frame_To_Sprite 41
    					              
    Hide_Sprite

    ret

; ---- Bullet .pbm mono images ----
                     ; id,  w,  h,  data,                 colourCode
loadBullet1:	
	MAKEBUFFEREDBITMAP 40, 16, 16, "sprites_mono/029.pbm", black 
endLoadBullet1:

loadBullet2:	
	MAKEBUFFEREDBITMAP 41, 16, 16, "sprites_mono/030.pbm", black 
endLoadBullet2:



; --- Player Frame List ---
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

; --- Small Alien Frame List ---
Small_Alien_Frame_List:
    .dl f50, f51
    .db $FF
f50: incbin "sprites/023.data"    ; Frame 1
f51: incbin "sprites/033.data"    ; Frame 2

; --- Big Alien Frane List ---
big_Alien_Frame_List:
    .dl ba52, ba53
    .db $FF
ba52: incbin "sprites/038.data"   ; Frame 1
ba53: incbin "sprites/039.data"   ; Frame 2

; Bullet Frame List
Bullet_Frame_List:
    .dl bul40, bul41
    .db $FF
bul40: incbin "sprites/029.data"    ; Frame n/s
bul41: incbin "sprites/030.data"    ; Frame e/w

;----------------
; Load Sound FX
;----------------
loadMenu_nav:                              ; for loading shoot sfx (laser)
    ld hl, menu_navStr                     ; start of data to send
    ld bc, endMenu_navStr - menu_navStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
menu_navStr:
    .db     23,0,$85                    ; audio command
    .db     -1, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     2522                        ; length in bytes (LONG)
    incbin "sounds/menu_nav.raw"        ; raw file path
endMenu_navStr:

loadNav_tick:                              ; for loading shoot sfx (laser)
    ld hl, nav_tickStr                     ; start of data to send
    ld bc, endNav_tickStr - nav_tickStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
nav_tickStr:
    .db     23,0,$85                    ; audio command
    .db     -2, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     1170                        ; length in bytes (LONG)
    incbin "sounds/new/nav_tick.raw"        ; raw file path
endNav_tickStr:

loadSelect:                              ; for loading shoot sfx (laser)
    ld hl, selectStr                     ; start of data to send
    ld bc, endSelectStr - selectStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
selectStr:
    .db     23,0,$85                    ; audio command
    .db     -3, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     2975                        ; length in bytes (LONG)
    incbin "sounds/new/select.raw"        ; raw file path
endSelectStr:

loadLaser:                              ; for loading shoot sfx (laser)
    ld hl, laserStr                     ; start of data to send
    ld bc, endLaserStr - laserStr       ; length of data to send
    rst.lil $18                         ; send data
    ret 
laserStr:
    .db     23,0,$85                    ; audio command
    .db     -5, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     3848                        ; length in bytes (LONG)
    incbin "sounds/laser.raw"           ; raw file path
endLaserStr:

loadBump:                              ; for loading shoot sfx (laser)
    ld hl, bumpStr                     ; start of data to send
    ld bc, endBumpStr - bumpStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
bumpStr:
    .db     23,0,$85                    ; audio command
    .db     -6, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     9830                        ; length in bytes (LONG)
    incbin "sounds/bump.raw"        ; raw file path
endBumpStr:

loadDoor:                              ; for loading shoot sfx (laser)
    ld hl, doorStr                     ; start of data to send
    ld bc, endDoorStr - doorStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
doorStr:
    .db     23,0,$85                    ; audio command
    .db     -7, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     10243                        ; length in bytes (LONG)
    incbin "sounds/new/door_2.raw"        ; raw file path
endDoorStr:

loadError:                              ; for loading shoot sfx (laser)
    ld hl, errorStr                     ; start of data to send
    ld bc, endErrorStr - errorStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
errorStr:
    .db     23,0,$85                    ; audio command
    .db     -8, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     2399                       ; length in bytes (LONG)
    incbin "sounds/new/error.raw"        ; raw file path
endErrorStr:

loadGlitch:                              ; for loading shoot sfx (laser)
    ld hl, glitchStr                     ; start of data to send
    ld bc, endGlitchStr - glitchStr    ; length of data to send
    rst.lil $18                            ; send data
    ret 
glitchStr:
    .db     23,0,$85                    ; audio command
    .db     -9, 5                       ; sample number, sample management
    .db     0                           ; load
    .dl     6068                        ; length in bytes (LONG)
    incbin "sounds/new/glitch.raw"        ; raw file path
endGlitchStr:












