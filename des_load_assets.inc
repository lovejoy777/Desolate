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

    ;-----------
    ; SFX
    ;-----------
    call loadLaser                    ; Shoot sfx
    
    ret

;------------------------
; Main Menu Asset
;------------------------
; Main Menu Screen Loader. using mono pbm file
Load_Banner_Asset:
    ld hl, loadGraphics				; start of data to send
	ld bc, endLoadGraphics - loadGraphics		; length of data to send
	rst.lil $18					; send data

	; take a break to catch up in 2 batches

	ld hl, loadGraphics				; start of data to send
	ld bc, endLoadGraphics - loadGraphics		; length of data to send
	rst.lil $18					; send data
    SET_GCOL_COL black
    ret

loadGraphics:	
	; id, width, height, data, colourCode
	MAKEBUFFEREDBITMAP 30, 192, 48, "menus/banner_02.pbm", 0 
    
endLoadGraphics:

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
    ld de, 470                 ; Start at Buffer 470 - 489
    ld b, 20                   ; Total tiles ( from des_tiles.inc)
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
