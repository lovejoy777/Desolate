; =============================================================================
; FILE: Desolate's des_main.asm
; TARGET: Agon Light 2 & ez80asm Assembler.
; =============================================================================

    .include "initial_macros.inc"
    .include "des_vdp_macros.inc"
    
    assume adl=1             ; use 24-bit addressing mode
    .org $40000              ; set origin to $40000
    jp start_here            ; jump to start_here
    .align 64                ; align to 64 bytes
    .db "MOS", 0, 1          ; MOS header

    .include "des_input.inc"
    .include "des_player_control.inc"


;---------------
; Player Vars
;---------------
player_sprite_id:      equ  0       ; Sprite ID number
player_x_mid:          db   16      ; Middle of player X
player_y_mid:          db   16      ; Middle of player Y
player_x:              dw   79      ; start x 
player_y:              dw   67      ; start y
player_dir:            db   0       ; 0 = down/south
is_moving:             db   0       ; 0 = not moving
move_timeout:          db   0       ; timer for movement
anim_frame:            db   0       ; frame 0 = looking down
anim_delay:            db   0       ; delay between frames
player_health:         db   100     ; Start health (100)
last_health:           db   0       ; Used to track changes
damage_cooldown:       db   0       ; cooldown after taking damage
damage_taken:          db   2       ; amount of damage to take
cooldown_time:         db   20      ; time to wait before taking damage again
knock_back_px:         db   2       ; amount of knock back in pixels


;----------------
; Start Here  
;----------------     
start_here:
    push af
    push bc
    push de
    push ix
    push iy
    

    call Init_Display          ; from des_main_subs.
    call Load_Player_Assets
    call Set_Graphics_VP       ; Set the Graphics VP for the game area
    CLG
    
Main_Loop:
    Select_Sprite_8bit 0
    Show_Sprite
    Update_GPU



    ; loop here until we hit ESC key
    ld a, (ix + $05)                    ; get ASCII code of key pressed
    cp 27                               ; check if 27 (ascii code for ESC)   
    jp z, EXIT_HERE                     ; if pressed, jump to exit

    jr Main_Loop

; ------------------
; This is where we exit the program

EXIT_HERE:

    CLS 
    pop iy                              ; Pop all registers back from the stack
    pop ix
    pop de
    pop bc
    pop af
    ld hl,0                             ; Load the MOS API return code (0) for no errors.
    ret                                 ; Return to MOS

   
    



Init_Display:
    SET_MODE 8
    SET_NONSCALED_GRAPHICS 
    SET_GCOL_BG_COL grey
    SET_GCOL_COL black           
    SET_TXT_BG_COL black
    CLS
    HIDE_CURSOR
    ret

;----------------------------------------
; Set Graphics ViewPort for game window.
;----------------------------------------
Set_Graphics_VP:
    ld a, 24           ; vdu 24,x1;y1;x2;y2; set graphics viewport
    rst.lil $10        ; send command to VDU
    ld hl, 64          ; left (X1)
    call vdu_write_word
    ld hl, 210         ; bottom (Y1)
    call vdu_write_word
    ld hl, 255         ; right (X2)
    call vdu_write_word
    ld hl, 83          ; top (Y2)
    call vdu_write_word
    ret

;------------------------------------------------------
; Helper to send 16-bit words (little endian) to VDU.
;------------------------------------------------------
vdu_write_word:
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
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

