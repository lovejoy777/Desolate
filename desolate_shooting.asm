; Fire key pressed in Shoot mode
LB758:
  LD A,(LDB8C)            ; get shooting flag
  CP $01                  ; still in prev shooting?
  jr Z,LB768              ; yes => jump
  LD A,$01
  LD (LDB8D),A            ; set shooting process flag
  LD (LDD55),A            ; set shooting flag for player's animation
LB768:
  JP L9E2E                ; Show the screen, continue the game main loop
;
; Process shoot within the game main loop
;
LB76B:
  LD A,(LDB8D)            ; get shooting process flag
  OR A                    ; in the process?
  JP Z,LB84A              ; no => jump
  LD A,(LDB8C)
  CP $01
  jr Z,LB797
  LD A,$01
  LD (LDB8D),A            ; set shooting process flag
  LD A,(LDB75)            ; get player Direction/orientation
  LD (LDB8B),A            ; set bullet Direction/orientation
  LD A,(LDB76)            ; get player X coord in tiles
  LD (LDB88),A            ; set bullet X coord in tiles
  LD A,(LDB77)            ; get player Y coord/line on the screen
  LD (LDB89),A            ; set bullet Y coord/line on the screen
  LD A,(LDB78)            ; get player Y coord in tiles
  LD (LDB8A),A            ; set bullet Y coord in tiles
LB797:
  LD A,(LDB8B)            ; get Bullet Direction/orientation
  OR A                    ; down?
  jr Z,LB7AD
  CP $01                  ; up?
  jr Z,LB7C7
  CP $02                  ; left?
  jr Z,LB7E1
  CP $03                  ; right?
  jr Z,LB7F3
; Bullet down
LB7AD:
  CALL LB87C              ; Move the Bullet
  CP $01                  ; Empty cell?
  JP NZ,LB8D6
  LD A,(LDB89)            ; get Bullet Y coord/line on the screen
  ADD A,16    ; was: $08  ; down 16 rows
  LD (LDB89),A            ; set Bullet Y coord/line on the screen
  LD A,(LDB8A)            ; get Bullet Y coord in tiles
  INC A                   ; down one tile
  LD (LDB8A),A            ; set Bullet Y coord in tiles
  jr LB805
; Bullet up
LB7C7:
  CALL LB87C              ; Move the Bullet
  CP $01                  ; Empty cell?
  JP NZ,LB8D6
  LD A,(LDB89)            ; get Bullet Y coord/line on the screen
  ADD A,-16   ; was: $F8  ; up 16 rows
  LD (LDB89),A            ; set Bullet Y coord/line on the screen
  LD A,(LDB8A)            ; get Bullet Y coord in tiles
  DEC A                   ; up one tile
  LD (LDB8A),A            ; set Bullet Y coord in tiles
  jr LB805
; Bullet left
LB7E1:
  CALL LB87C              ; Move the Bullet
  CP $01                  ; Empty cell?
  JP NZ,LB8D6
  LD A,(LDB88)            ; get bullet X coord in tiles
  DEC A                   ; left one tile
  LD (LDB88),A            ; set bullet X coord in tiles
  jr LB805
; Bullet right
LB7F3:
  CALL LB87C              ; Move the Bullet
  CP $01                  ; Empty cell?
  JP NZ,LB8D6
  LD A,(LDB88)            ; get bullet X coord in tiles
  INC A                   ; right one tile
  LD (LDB88),A            ; set bullet X coord in tiles
; Bullet moving
LB805:
  LD A,(LDB8D)            ; get shooting process flag
  OR A                    ; in the process?
  jr Z,LB84A              ; no => jump
  LD A,(LDB88)            ; get bullet X coord in tiles
  add a,a                 ; tile coord -> column number
  LD H,A
  LD A,(LDB89)            ; get Bullet Y coord/line on the screen
  LD L,A
  CALL LB84F              ; Get Bullet sprite address in DE
  xor a                   ; draw flags
  CALL L9EDE              ; Draw sprite DE at column H row L
  LD A,$01
  LD (LDB8C),A
  LD A,(LDB81)            ; get Alien type
  CP $02                  ; the big one?
  jr Z,LB82B              ; yes => jump to Check if the Bullet hit the Alien
  jp LB64B                ; Check Is the Bullet hit the Alien, process the hit
;
LB82B:
  CALL LB8CA              ; Is the Bullet hit the Alien?
  OR A
  ret nz                  ; no => return
; Bullet hit the Alien, the big one
  XOR A
  LD (LDB8D),A            ; clear shooting process flag
  LD (LDB88),A            ; clear Bullet X coord in tiles
  LD (LDB89),A            ; clear Bullet Y coord/line on the screen
  LD A,(LDB85)            ; get Alien health
  DEC A                   ; Alien injured
  LD (LDB85),A            ; set Alien health
  OR A                    ; zero Health?
  CALL Z,LB71F            ; yes => Killed the alien
  ret
;
LB84A:
  XOR A
  LD (LDB8C),A            ; clear shooting flag
  RET
;
; Get Bullet tile address and draw flags
;   Returns: DE = tile address; A = draw flags (always $00)
LB84F:
  LD A,(LDB8B)            ; get Bullet Direction/orientation
  OR A                    ; down?
  jr Z,LB865
  CP $01                  ; up?
  jr Z,LB86A
  CP $02                  ; left?
  jr Z,LB870
  CP $03                  ; right?
  jr Z,LB876
LB865:                    ; Bullet goes down
  LD DE,Sprites+$1D*64    ; was: $EAC7 - Bullet vert sprite
  RET
LB86A:                    ; Bullet goes up
  LD DE,Sprites+$1E*64    ; was: $EAC7 - Bullet vert sprite
  RET
LB870:                    ; Bullet goes left
  LD DE,Sprites+$1F*64    ; was: $EAB7 - Bullet horz sprite
  RET
LB876:                    ; Bullet goes right
  LD DE,Sprites+$20*64    ; was: $EAB7 - Bullet horz sprite
  RET
;
; Move the Bullet
; Returns: A = room cell value for the new bullet position
LB87C:
  CALL LADE5              ; Decode current room to LDBF5
  LD A,(LDB88)            ; get Bullet X coord in tiles
  LD E,A
  CALL LB89B              ; For Bullet direction left: dec E, right: inc E
  LD D,$00
  ADD HL,DE
;  LD A,(LDB74)            ; $0C - line width in tiles ??
;  LD E,A
  ld e,12                 ; Line width in tiles
  LD D,$00
  LD A,(LDB8A)            ; get Bullet Y coord in tiles
  LD B,A
  CALL LB8AB              ; For Bullet direction up: dec B, down: inc B
LB896:
  ADD HL,DE
  DJNZ LB896
  LD A,(HL)               ; get value for the room cell
  RET
;
; For Bullet direction left: dec E, right: inc E
LB89B:
  LD A,(LDB8B)            ; get Bullet Direction/orientation
  OR A
  RET Z
  CP $01
  RET Z
  CP $02                  ; left?
  JR NZ,LB8A9
  DEC E                   ; one left
  RET
LB8A9:
  INC E                   ; one right
  RET
;
; For Bullet direction up: dec B, down: inc B
LB8AB:
  LD A,(LDB8B)            ; get Bullet Direction/orientation
  CP $02
  RET Z
  CP $03
  RET Z
  OR A                    ; down?
  JR NZ,LB8B9
  INC B                   ; one down
  RET
LB8B9:
  DEC B                   ; one up
  RET
;
; Get A = Bullet position within the room
LB8BB:
;  LD A,(LDB74)            ; $0C - line width in tiles ??
;  LD C,A
  ld c,12                 ; Line width in tiles
  LD A,(LDB8A)            ; get Bullet Y coord in tiles
  LD B,A
  LD A,(LDB88)            ; get Bullet X coord in tiles
LB8C6:
  ADD A,C
  DJNZ LB8C6
  RET                     ; now A = Bullet Y coord * 12 + Bullet X coord
;
; Is the Bullet hit the Alien?
LB8CA:
  CALL LB6ED              ; Get A = Alien position within the room
  CALL LB8BB              ; Get A = Bullet position within the room
  LD C,A
  LD A,(LDB87)            ; get Alien position within the room
  SUB C
  RET
;
; Bullet hit something
LB8D6:
  CALL LB8DC              ; Clear all Bullet variables
  JP LB805
;
; Clear all Bullet variables
LB8DC:
  XOR A
  LD (LDB8D),A            ; clear shooting process mark
  LD (LDB88),A            ; clear Bullet X coord in tiles
  LD (LDB89),A            ; clear Bullet Y coord/line on the screen
  LD (LDB8A),A            ; clear Bullet Y coord in tiles
  RET