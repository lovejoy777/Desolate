;-----------
; Game Over
;-----------
Game_Over:
    ld a, 0
    ld (player_is_alive), a

    ; Hide Sprites
    Select_Sprite_8bit playerSprite
    Hide_Sprite
    Select_Sprite_8bit smAlienSprite
    Hide_Sprite
    Select_Sprite_8bit bigAlienSprite
    Hide_Sprite
    
    call Reset_VP
    ld a, red
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP
    
    TEXT_AT_GRAPHICS_CURSOR     ; set text at graphics cursor.
    SET_GCOL_BG_COL black
    SET_GCOL_COL red
    CLG

    ; --- Draw Text ---
    call Draw_Dead

    ; Fall through to Game Over Loop

Game_Over_Loop:
    V_SYNC                       ; Wait for vertical sync

    ; Check For Key Presses
    call Check_Action_Triggers 
    
    jp Game_Over_Loop            ; loop back to game over loop

;----------------
; Game Over Text
;----------------
Draw_Dead:
    
    ld hl, SE0BD 
    ld b, 56
    ld c, 98
    call print_frac_string

    ld hl, SE0A1
    ld b, 56
    ld c, 138
    call print_frac_string

    ;--------------------------------------------------
    ; Rect 1 (Health Value area) 
    ;--------------------------------------------------
    ; draw grey rectangle to mask old Health Value.
    ; Draw_Rect x1, y1, x2, y2, bgcol
    Draw_Filled_Rect 190, 128, 210, 190, black

    ; Plot for Text
    ;Plot_For_Text plot_x, plot_y
    Plot_For_Text 194, 148
            
    TEXT_AT_GRAPHICS_CURSOR     ; set text at graphics cursor.
    SET_GCOL_COL red

    ; Print the actual health number
    ld a, (alien_amount_killed)
    call Print_A_Number

    ret