;-----------------------------------------------------------------------
; FILE: des_story.inc
; Logic: displays the Desolate story over 3 pages, "space" = next page
;------------------------------------------------------------------------

story_mode:
    ; check if story mode is active
    ; if not start new game without story.
    ld a, (story_mode_is_active)
    cp $01
    jp nz, .new_game

    call Reset_VP
    ld a, bright_cyan
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP

    TEXT_AT_GRAPHICS_CURSOR     ; set text at graphics cursor.
    SET_GCOL_BG_COL black
    SET_GCOL_COL bright_cyan

    CLG

    ld a, (story_page)
    cp $01
    jp z, .draw_page_01
    cp $02
    jp z, .draw_page_02
    cp $03
    jp z, .draw_page_03
    cp $04
    jp z, story_cleanup

.skip_story:
    ; reset window flag
    ld a, 0
    ld (story_mode_is_active), a 
    ld (main_menu_is_active), a
    ld (ui_win_active), a
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a

    ret

    ;---------------------------------------
    ; Draw Strings with Fractional spacings
    ;---------------------------------------
.draw_page_01:
    ld hl, SE115
    ld bc, 0
    ld b, 56
    ld c, 98
    call print_frac_string
    
    jp draw_arrow

    ret

.draw_page_02:
    ld hl, SE117
    ld bc, 0
    ld b, 56
    ld c, 98
    call print_frac_string

    jp draw_arrow

    ret
   
.draw_page_03:
    ld hl, SE119
    ld bc, 0
    ld b, 56
    ld c, 98
    call print_frac_string

    jp draw_arrow

    ret

draw_arrow:
    Draw_Filled_Rect 239,195, 255, 211, bright_cyan
    ld de, 490                     ; 490 = down arrow 
    SELECT_BUFFER_DE
    Draw_Tile 239, 195              ; from des_vdp_macros.inc

    ret

story_cleanup:
    TEXT_AT_TEXT_CURSOR 
    SET_GCOL_BG_COL grey
    SET_TXT_COL bright_white

    ; reset flag
    ld a, 0
    ld (story_mode_is_active), a 
    ld (main_menu_is_active), a
    ld (ui_win_active), a
    ld (info_mode_is_active), a
    ld (credits_mode_is_active), a
    
    ; reset story to page 1
    ld a, 1
    ld (story_page), a

    call Reset_VP
    ld a, grey
    ld (banner_colour), a
    call Draw_Banner_Image     ; from des_draw_banner.inc
    call Set_Game_VP           ; from des_main_subs

    CLG

    ; fall into new game

.new_game:
    call CleanStart              ;  from des_main_subs.inc
    ;call playGame_start
    call Trigger_Room_Load       ; from des_draw_room.inc
    ret

    
