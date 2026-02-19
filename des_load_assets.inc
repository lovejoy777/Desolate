; =============================================================================
; FILE: des_load_assets.inc
; Purpose: Load Desolate's Assets.
; =============================================================================
Load_All_Assets:

    call Load_Banner_Asset
    
    call Load_Room_Tiles
    
    call Load_Sm_Alien_Asset
    call Load_Big_Alien_Asset
    call Load_Player_Assets
    
    Activate_amount_sprites 3

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
    

;------------------------------------------------------------
; Banner Asset
;--------------------------------------------------------------
Load_Banner_Asset:
    ; 1. Select buffer id 76
    Select_Buffer_8bit 76

    ; 2. Write data to buffer , width & height for size
    Write_To_Buffer_8bit 192, 48

    ; 2a. Stream the Pixels
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

;------------------------------------------------------------
; Room Assets 12x8 Grid of 16x16 tiles.
;--------------------------------------------------------------

Load_Room_Tiles:
    ld ix, Tileset1            ; Point to your original data blocks
    ld de, 100                 ; Start Buffer 100
    ld b, 122                  ; Total tiles (Matches your des_load_assets.inc)
.tile_loop:
    push bc
    push de 

    ; --- 1. Select Buffer with 16 bit id ---
    Select_Buffer_16bit d, e

    ; --- 2. Upload Data (VDU 23, 0, $A0) ---
    Write_To_Buffer_16bit d, e

    ; Transfer IX to HL via stack to fix "Operand not matching"
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
; Player Assets, Sprite 0
;--------------------------------------------------------------
Load_Player_Assets:
    ld ix, Player_Frame_List
    ld d, 0
.p_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    
    ; Select Buffer
    Select_Buffer_8bit d
    ; Load colour bitmap to current buffer. VDU 23, 27, 1, w; h; b1, b2 ... bn
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

    ; Select Sprite 0 = player
    Select_Sprite_8bit 0
    ; Add Frames to current Sprites with default buffer id
    Add_Frame_To_Sprite d

    ; Hide the current sprite until the room is ready and he has his coords.
    Hide_Sprite

    inc d
    inc ix
    inc ix
    inc ix
    jp .p_loop
    
    ret

;------------------------------------------------------------
; Small Alien Assets, Sprite 1
;--------------------------------------------------------------
; Alien Loader Sprite 1
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

    ; Select Sprite 1 = small alien
    Select_Sprite_8bit 1
    ; Add Frame to current Sprites with default buffer id
    Add_Frame_To_Sprite d
    ; Hide the current sprite
    Hide_Sprite

    inc d
    inc ix
    inc ix
    inc ix
    jp .sa_loop

    ret

; Big Alien Loader Sprite 2
Load_Big_Alien_Asset:
    ld ix, big_Alien_Frame_List
    ld d, 38
.ba_loop:
    ld a, (ix+0)
    cp $FF
    ret z              
    
    ; Select Buffer id
    Select_Buffer_8bit d

    ; Write Block to buffer width & height
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

    ; Select Sprite 2 = big alien
    Select_Sprite_8bit 2
    ; Add Frame to current Sprites with default buffer id
    Add_Frame_To_Sprite d
    ; Hide the current sprite
    Hide_Sprite

    inc d
    inc ix
    inc ix
    inc ix
    jp .ba_loop

    ret

;------------------------------------------------------------
; Load 16x16 tiles from Tileset2: into Buffers 300-425.
;--------------------------------------------------------------

Load_UI_Assets:
    ld ix, Tileset2            ; Point to data block Tileset2 in desoldata.asm.
    ld de, 300                 ; Start Buffer id = 300
    ld b, 126                  ; Total tiles (Matches your des_load_assets.inc)
.wp_loop:
    push de                    ; save current buffer id to stack

    ; --- 1. Select Buffer with 16 bit id ---
    Select_Buffer_16bit d, e
    ; Load colour bitmap data into current bitmap
    ;Load_Colour_Bitmap 16, 16
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 1            ; was 33
    rst.lil $10
    ld a, 16            ; Width 16
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 16            ; Height 16
    rst.lil $10
    ld a, 0
    rst.lil $10
    ; Send Data (Masked logic)
    call send_sprite_pixels      
    
    pop de     ; get the current buffer id from the stack
    inc de     ; loop up id's
    dec b      ; loop down amount of total tiles
    
    djnz .wp_loop
    ret

; Expansion logic for getting rid of the mask and forming 16x16 bitmaps.
send_sprite_pixels:
    ld c, 16
.spr_row:
    inc ix
    ld a, (ix+0)
    inc ix
    call expand_byte
    inc ix
    ld a, (ix+0)
    inc ix
    call expand_byte
    dec c
    jr nz, .spr_row
    ret

expand_byte:
    push bc
    ld b, 8
.bit_loop:
    rlca
    jr c, .is_set
    push af
    ld a, 0
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 0
    rst.lil $10
    pop af
    jr .next_bit
.is_set:
    push af
    ; fg colour for tiles, 0 = black, 255 = full colour, alpha = 255 solid
    ld a, 0       ; red
    rst.lil $10
    ld a, 0       ; green
    rst.lil $10
    ld a, 0       ; blue
    rst.lil $10
    ld a, 255     ; alpha
    rst.lil $10
    pop af
.next_bit:
    djnz .bit_loop
    pop bc
    ret

;------------------------------------------------------------
; Small Pop up window 192x48 (12x3 tiles)
;--------------------------------------------------------------
Load_Sm_Popup_Asset:
    ; 1. Select buffer id 80
    Select_Buffer_8bit 80

    ; 2. Write data to buffer , width & height for size
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
Load_Invtop_Popup_Asset:
    ; 1. Select buffer id 81
    Select_Buffer_8bit 81

    ; 2. Write data to buffer , width & height for size
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

Load_Invbot_Popup_Asset:
    ; 1. Select buffer id 81 for bottom half
    Select_Buffer_8bit 82

    ; 2. Write data to buffer , width & height for size
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
Load_Termtop_Popup_Asset:
    ; 1. Select buffer id 81
    Select_Buffer_8bit 83

    ; 2. Write data to buffer , width & height for size
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

Load_Termbot_Popup_Asset:
    ; 1. Select buffer id 81 for bottom half
    Select_Buffer_8bit 84

    ; 2. Write data to buffer , width & height for size
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



Tile_File_List:
    .dl t000, t001, t002, t003, t004, t005, t006, t007, t008, t009
    .dl t010, t011, t012, t013, t014, t015, t016, t017, t018, t019
    .dl t020, t021, t022, t023, t024, t025, t026, t027, t028, t029
    .dl t030, t031, t032, t033, t034, t035, t036, t037, t038, t039
    .dl t040, t041, t042, t043, t044, t045, t046, t047, t048, t049
    .dl t050, t051, t052, t053, t054, t055, t056, t057, t058, t059
    .dl t060, t061, t062, t063, t064, t065, t066, t067, t068, t069
    .dl t070, t071, t072, t073, t074, t075, t076, t077, t078, t079
    .dl t080, t081, t082, t083, t084, t085, t086, t087, t088, t089
    .dl t090, t091, t092, t093, t094, t095, t096, t097, t098, t099
    .dl t100, t101, t102, t103, t104, t105, t106, t107, t108, t109
    .dl t110, t111, t112, t113, t114, t115, t116, t117, t118, t119
    .dl t120, t121

t000: incbin "tileset1/000.data"
t001: incbin "tileset1/001.data"
t002: incbin "tileset1/002.data"
t003: incbin "tileset1/003.data"
t004: incbin "tileset1/004.data"
t005: incbin "tileset1/005.data"
t006: incbin "tileset1/006.data"
t007: incbin "tileset1/007.data"
t008: incbin "tileset1/008.data"
t009: incbin "tileset1/009.data"
t010: incbin "tileset1/010.data"
t011: incbin "tileset1/011.data"
t012: incbin "tileset1/012.data"
t013: incbin "tileset1/013.data"
t014: incbin "tileset1/014.data"
t015: incbin "tileset1/015.data"
t016: incbin "tileset1/016.data"
t017: incbin "tileset1/017.data"
t018: incbin "tileset1/018.data"
t019: incbin "tileset1/019.data"
t020: incbin "tileset1/020.data"
t021: incbin "tileset1/021.data"
t022: incbin "tileset1/022.data"
t023: incbin "tileset1/023.data"
t024: incbin "tileset1/024.data"
t025: incbin "tileset1/025.data"
t026: incbin "tileset1/026.data"
t027: incbin "tileset1/027.data"
t028: incbin "tileset1/028.data"
t029: incbin "tileset1/029.data"
t030: incbin "tileset1/030.data"
t031: incbin "tileset1/031.data"
t032: incbin "tileset1/032.data"
t033: incbin "tileset1/033.data"
t034: incbin "tileset1/034.data"
t035: incbin "tileset1/035.data"
t036: incbin "tileset1/036.data"
t037: incbin "tileset1/037.data"
t038: incbin "tileset1/038.data"
t039: incbin "tileset1/039.data"
t040: incbin "tileset1/040.data"
t041: incbin "tileset1/041.data"
t042: incbin "tileset1/042.data"
t043: incbin "tileset1/043.data"
t044: incbin "tileset1/044.data"
t045: incbin "tileset1/045.data"
t046: incbin "tileset1/046.data"
t047: incbin "tileset1/047.data"
t048: incbin "tileset1/048.data"
t049: incbin "tileset1/049.data"
t050: incbin "tileset1/050.data"
t051: incbin "tileset1/051.data"
t052: incbin "tileset1/052.data"
t053: incbin "tileset1/053.data"
t054: incbin "tileset1/054.data"
t055: incbin "tileset1/055.data"
t056: incbin "tileset1/056.data"
t057: incbin "tileset1/057.data"
t058: incbin "tileset1/058.data"
t059: incbin "tileset1/059.data"
t060: incbin "tileset1/060.data"
t061: incbin "tileset1/061.data"
t062: incbin "tileset1/062.data"
t063: incbin "tileset1/063.data"
t064: incbin "tileset1/064.data"
t065: incbin "tileset1/065.data"
t066: incbin "tileset1/066.data"
t067: incbin "tileset1/067.data"
t068: incbin "tileset1/068.data"
t069: incbin "tileset1/069.data"
t070: incbin "tileset1/070.data"
t071: incbin "tileset1/071.data"
t072: incbin "tileset1/072.data"
t073: incbin "tileset1/073.data"
t074: incbin "tileset1/074.data"
t075: incbin "tileset1/075.data"
t076: incbin "tileset1/076.data"
t077: incbin "tileset1/077.data"
t078: incbin "tileset1/078.data"
t079: incbin "tileset1/079.data"
t080: incbin "tileset1/080.data"
t081: incbin "tileset1/081.data"
t082: incbin "tileset1/082.data"
t083: incbin "tileset1/083.data"
t084: incbin "tileset1/084.data"
t085: incbin "tileset1/085.data"
t086: incbin "tileset1/086.data"
t087: incbin "tileset1/087.data"
t088: incbin "tileset1/088.data"
t089: incbin "tileset1/089.data"
t090: incbin "tileset1/090.data"
t091: incbin "tileset1/091.data"
t092: incbin "tileset1/092.data"
t093: incbin "tileset1/093.data"
t094: incbin "tileset1/094.data"
t095: incbin "tileset1/095.data"
t096: incbin "tileset1/096.data"
t097: incbin "tileset1/097.data"
t098: incbin "tileset1/098.data"
t099: incbin "tileset1/099.data"
t100: incbin "tileset1/100.data"
t101: incbin "tileset1/101.data"
t102: incbin "tileset1/102.data"
t103: incbin "tileset1/103.data"
t104: incbin "tileset1/104.data"
t105: incbin "tileset1/105.data"
t106: incbin "tileset1/106.data"
t107: incbin "tileset1/107.data"
t108: incbin "tileset1/108.data"
t109: incbin "tileset1/109.data"
t110: incbin "tileset1/110.data"
t111: incbin "tileset1/111.data"
t112: incbin "tileset1/112.data"
t113: incbin "tileset1/113.data"
t114: incbin "tileset1/114.data"
t115: incbin "tileset1/115.data"
t116: incbin "tileset1/116.data"
t117: incbin "tileset1/117.data"
t118: incbin "tileset1/118.data"
t119: incbin "tileset1/119.data"
t120: incbin "tileset1/120.data"
t121: incbin "tileset1/121.data"



; --- Player Data List ---
Player_Frame_List:
    .dl f36, f37, f38, f39, f40, f41, f42, f43, f44, f45, f46, f47
    .db $FF

f36: incbin "32x32_gs_sprites/036.data"
f37: incbin "32x32_gs_sprites/037.data"
f38: incbin "32x32_gs_sprites/038.data"
f39: incbin "32x32_gs_sprites/039.data"
f40: incbin "32x32_gs_sprites/040.data"
f41: incbin "32x32_gs_sprites/041.data"
f42: incbin "32x32_gs_sprites/042.data"
f43: incbin "32x32_gs_sprites/043.data"
f44: incbin "32x32_gs_sprites/044.data"
f45: incbin "32x32_gs_sprites/045.data"
f46: incbin "32x32_gs_sprites/046.data"
f47: incbin "32x32_gs_sprites/047.data"

; --- Small Alien Data List ---
Sm_Alien_Frame_List:
    .dl f63, f64
    .db $FF

f63: incbin "sprites/023.data"
f64: incbin "sprites/033.data"

; --- Big Alien Data List ---
; --- Update the Big Alien Data List at the bottom ---
big_Alien_Frame_List:
    .dl ba38, ba39
    .db $FF

ba38: incbin "sprites/038.data"   ; Frame 1
ba39: incbin "sprites/039.data"   ; Frame 2


;-----------------------------------------------------------
; UI Tile Data (From tileset2 folder)
;-----------------------------------------------------------

ut17: incbin "tileset2/017.data"   ; ID 200: Top Left
ut18: incbin "tileset2/018.data"   ; ID 201: Top Right
ut19: incbin "tileset2/019.data"   ; ID 202: Bottom Left
ut20: incbin "tileset2/020.data"   ; ID 203: Bottom Right
ut13: incbin "tileset2/013.data"   ; ID 204: Left Edge
ut14: incbin "tileset2/014.data"   ; ID 205: Right Edge
ut15: incbin "tileset2/015.data"   ; ID 206: Top Edge
ut16: incbin "tileset2/016.data"   ; ID 207: Bottom Edge
ut00: incbin "tileset2/000.data"   ; ID 208: Center Fill

