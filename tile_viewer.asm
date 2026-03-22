; =============================================================================
; FILE: tile_viewer.asm
; DESCRIPTION: Renders Desolate Room Tiles.
;
; CRITICAL LOGIC:
; Renders all 122 tiles using desolutil.asm/ Tileset1: data.
; Complete with paging, 70 bitmaps per page, use space-bar to go to the next page.
; =============================================================================

    .include "initial_macros.inc"

    assume adl=1
    .org $40000
    jp start_here
    .align 64
    .db "MOS", 0, 1

; Variables
cursor_x:      .dl 8
cursor_y:      .dl 8
current_id:    .dl 0
items_on_page: .db 0

start_here:
    push af
    push bc
    push de
    push ix
    push iy

    ; 1. INITIALIZE DISPLAY
    SET_MODE 8
    SET_TXT_BG_COL grey
    SET_TXT_COL bright_white
    SET_NONSCALED_GRAPHICS
    HIDE_CURSOR
    TEXT_AT_GRAPHICS_CURSOR
    CLS

main_loop:
    CLS ; for after 1st loop of pages, restart at page 0.
    ; Reset Tracking Variables
    ld hl, 0
    ld (current_id), hl
    ld a, 0
    ld (items_on_page), a
    ld hl, 8
    ld (cursor_x), hl
    ld (cursor_y), hl

    ; -------------------------------------------------------------------------
    ; PART 1: TILESET1 (122 items)
    ; -------------------------------------------------------------------------
    ld ix, Tileset1
    ld b, 122                     ; 122 tiles from desoltils/Tileset1:
loop_tiles:
    push bc
    call define_tile_bitmap      ; Setup Header
    call send_tile_pixels        ; Send Data (Unmasked logic)
    call handle_item_display     ; Draw and Page Check
    pop bc
    djnz loop_tiles

    ; Wait for Key 32 (space), after all tiles have been rendered before looping back to page 0.
    call wait_for_down_key
    jp main_loop


; =============================================================================
; SUBROUTINES
; =============================================================================

handle_item_display:
    call draw_next_item          ; PLOT 4 and VDU 23,27,3 logic
    
    ; Increment Global ID
    ld hl, (current_id)
    inc hl
    ld (current_id), hl

    ; Page Check
    ld a, (items_on_page)
    inc a
    ld (items_on_page), a
    cp 70
    ret nz

    ; If 70, Wait for Key 32 (space) before going to the next page.
    call wait_for_down_key

    ; Reset Page
    ld a, 0
    ld (items_on_page), a
    CLS
    ld hl, 8
    ld (cursor_x), hl
    ld (cursor_y), hl
    ret
    
wait_for_down_key:
    MOSCALL $00
    cp 32
    jr nz, wait_for_down_key
    ret
    
define_tile_bitmap:
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, (current_id)
    rst.lil $10
    
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 1
    rst.lil $10
    ld a, 16
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, 16
    rst.lil $10
    ld a, 0
    rst.lil $10
    ret

draw_next_item:
    ; 1. Print ID
    ld a, 25
    rst.lil $10
    ld a, 4
    rst.lil $10 
    ld hl, (cursor_x)
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
    ld hl, (cursor_y)
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10

    ld a, (current_id)
    call print_hex_byte

    ; 2. Draw Bitmap
    ld a, 23
    rst.lil $10
    ld a, 27
    rst.lil $10
    ld a, 3
    rst.lil $10
    ld hl, (cursor_x)
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
    ld hl, (cursor_y)
    ld de, 10
    add hl, de
    ld a, l
    rst.lil $10
    ld a, h
    rst.lil $10
    
    ; Advance Grid
    ld hl, (cursor_x)
    ld de, 32
    add hl, de
    ld (cursor_x), hl
    ld de, 300
    or a
    sbc hl, de
    ret c
    
    ld hl, 8
    ld (cursor_x), hl
    ld hl, (cursor_y)
    ld de, 32
    add hl, de
    ld (cursor_y), hl
    ret

send_tile_pixels:
    ld c, 16
.tile_row:
    ld a, (ix+0)
    inc ix
    call expand_byte
    ld a, (ix+0)
    inc ix
    call expand_byte
    dec c
    jr nz, .tile_row
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
    rst.lil $10
    rst.lil $10
    ld a, 0
    rst.lil $10
    pop af
    jr .next_bit
.is_set:
    push af
    ld a, 255
    rst.lil $10
    rst.lil $10
    rst.lil $10
    ld a, 255
    rst.lil $10
    pop af
.next_bit:
    djnz .bit_loop
    pop bc
    ret

print_hex_byte:
    push af 
    rrca 
    rrca 
    rrca 
    rrca 
    call print_hex_nibble 
    pop af
print_hex_nibble:
    and $0F 
    add a, '0' 
    cp '9' + 1 
    jr c, .is_digit 
    add a, 7
.is_digit: 
    rst.lil $10 
    ret

    ; -------------------------------------------------------------------------
    ; CLEAN EXIT
    ; -------------------------------------------------------------------------

exit_to_mos:
    ; RESTORE SYSTEM STATE: Turn text cursor back on before exiting to MOS.
    SET_MODE 0           ; Return to Text Mode
    SET_TXT_BG_COL black
    SET_TXT_COL bright_white
    SET_GCOL_COL bright_white
    TEXT_AT_TEXT_CURSOR
    SHOW_CURSOR
    CLS

    pop iy
    pop ix
    pop de
    pop bc
    pop af
    ld hl, 0
    ret

include "des_tiles.inc"