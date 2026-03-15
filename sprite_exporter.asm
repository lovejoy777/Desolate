; =============================================================================
; FILE: sprite_exporter.asm
; DESCRIPTION: Exports Desolate Sprites to sdcard.
;
; CRITICAL LOGIC:
; Exports all 36 sprites from desolutil.asm/ Sprites: data.
; .
; =============================================================================

    .include "initial_macros.inc"
    assume adl=1
    .org $40000
    jp start_here
    .align 64
    .db "MOS", 0, 1

; Variables
current_id:    .dl 0
buffer_ptr:    .dl 0

start_here:
    push af
    push bc
    push de
    push ix
    push iy

    SET_MODE 8
    CLS
    
    ld hl, 0
    ld (current_id), hl
    ld ix, Tileset2               ; Pointer to data in desoltils.asm

    ; We use a hard loop of 36 to ensure we don't over-run
    ld b, 126                     ; Number of sprites to copy over.
export_loop:
    push bc                      ; PROTECT LOOP COUNTER
    
    ; Reset the RAM buffer pointer for the new file
    ld hl, tile_buffer
    ld (buffer_ptr), hl

    ; Process one 16x16 sprite (Matches your viewer exactly)
    call capture_sprite_pixels   

    ; Save to SD
    call update_filename_id
    ld hl, file_path
    ld de, tile_buffer
    ld bc, 1024                   ; 16x16 pixels
    MOSCALL $02                  ; MOS_SAVE

    ; Print feedback
    ld hl, file_path
    call print_string
    ld hl, msg_ok
    call print_string

    ; Increment ID
    ld hl, (current_id)
    inc hl
    ld (current_id), hl

    pop bc                       ; RESTORE LOOP COUNTER
    djnz export_loop

    pop iy
    pop ix
    pop de
    pop bc
    pop af
    ld hl, 0
    ret

; -------------------------------------------------------------------------
; PIXEL EXTRACTION (Mirror of specky_tile_viewer.asm)
; -------------------------------------------------------------------------
capture_sprite_pixels:
    ld c, 16                     ; 16 rows high
.row:
    push bc                      ; Save row counter
    
    ; LEFT BYTE
    inc ix                       ; Skip Mask
    ld a, (ix+0)                 ; Load Data
    inc ix
    call expand_to_ram           ; Process 8 bits
    
    ; RIGHT BYTE
    inc ix                       ; Skip Mask
    ld a, (ix+0)                 ; Load Data
    inc ix
    call expand_to_ram           ; Process 8 bits
    
    pop bc                       ; Restore row counter
    dec c
    jr nz, .row
    ret

; -------------------------------------------------------------------------
; BIT EXPANSION (Modified for 1024-byte RGBA8888)
; -------------------------------------------------------------------------
; -------------------------------------------------------------------------
; BIT EXPANSION (Modified to create 1024-byte RGBA8888 files)
; -------------------------------------------------------------------------
expand_to_ram:
    push bc                      
    push af                      
    ld b, 8                      ; 8 bits per byte
    ld de, (buffer_ptr)          
.bit_loop:
    pop af                       
    rlca                         ; Shift bit to Carry
    push af                      
    
    jr c, .set_pixel
    
    ; Bit is 0: Fully Transparent (R, G, B, Alpha)
    xor a
    ld (de), a                   ; Red 0
    inc de
    ld (de), a                   ; Green 0
    inc de
    ld (de), a                   ; Blue 0
    inc de
    ld (de), a                   ; Alpha 0 (Transparent)
    inc de
    jr .next_bit
    
.set_pixel:
    ; Bit is 1: Opaque White (R, G, B, Alpha)
    ld a, 0
    ld (de), a                   ; Red 255
    inc de
    ld (de), a                   ; Green 255
    inc de
    ld (de), a                   ; Blue 255
    inc de
    ld a, 255
    ld (de), a                   ; Alpha 255 (Opaque)
    inc de

.next_bit:
    djnz .bit_loop
    
    ld (buffer_ptr), de          
    pop af                       
    pop bc                       
    ret

; -------------------------------------------------------------------------
; HELPERS & DATA
; -------------------------------------------------------------------------

update_filename_id:
    ld hl, (current_id)
    ld de, 100
    call .math
    ld (digit_h), a
    ld de, 10
    call .math
    ld (digit_t), a
    ld a, l
    add a, '0'
    ld (digit_u), a
    ret
.math:
    ld a, '0' - 1
.m_loop:
    inc a
    or a
    sbc hl, de
    jr nc, .m_loop
    add hl, de
    ret

print_string:
    ld a, (hl)
    or a
    ret z
    rst.lil $10
    inc hl
    jr print_string

msg_ok:     .db " - OK", 13, 10, 0
file_path:  .db "/Desolate/tileset21/"
digit_h:    .db "0"
digit_t:    .db "0"
digit_u:    .db "0"
            .db ".data", 0

    .align 4
tile_buffer: .ds 1024             ; Storage for one 16x16 sprite

.include "des_tiles.inc"