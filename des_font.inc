; --- des_font.inc ---
; Fully reformatted for Agon: 8 bytes per char, no headers.
; Starting at ASCII 32 (Space)

FontProto:
    ; 32-43: Space to + (Placeholders/Originals)
    db $00,$00,$00,$00,$00,$00,$00,$00 ; Space
    db $18,$3C,$3C,$18,$18,$00,$18,$00 ; !
    db $6C,$6C,$6C,$00,$00,$00,$00,$00 ; "
    db $6C,$6C,$FE,$6C,$FE,$6C,$6C,$00 ; #
    db $10,$3C,$50,$38,$14,$3C,$10,$00 ; $
    db $60,$66,$0C,$18,$30,$66,$06,$00 ; %
    db $38,$6C,$38,$76,$DC,$CC,$76,$00 ; &
    db $30,$30,$60,$00,$00,$00,$00,$00 ; '
    db $0C,$18,$30,$30,$30,$30,$18,$0C ; (
    db $30,$18,$0C,$0C,$0C,$0C,$18,$30 ; )
    db $00,$66,$3C,$FF,$3C,$66,$00,$00 ; *
    db $00,$18,$18,$7E,$18,$18,$00,$00 ; +

    ; 44-47: Comma, Hyphen, Dot, Slash
    db $00,$00,$00,$00,$00,$60,$60,$C0 ; ,
    db $00,$00,$00,$00,$F8,$F8,$00,$00 ; -
    db $00,$00,$00,$00,$00,$00,$00,$C0 ; .
    db $0C,$18,$30,$30,$60,$60,$C0,$C0 ; /

    ; 48-57: Numbers 0-9
    db $78,$FC,$CC,$CC,$CC,$CC,$FC,$78 ; 0
    db $10,$30,$70,$F0,$30,$30,$30,$30 ; 1
    db $F0,$F8,$0C,$0C,$18,$30,$60,$C0 ; 2
    db $F0,$F8,$0C,$0C,$38,$38,$0C,$0C ; 3
    db $C0,$C0,$CC,$CC,$FC,$FC,$0C,$0C ; 4
    db $FC,$FC,$C0,$C0,$F8,$FC,$0C,$0C ; 5
    db $38,$78,$C0,$C0,$F8,$FC,$CC,$CC ; 6
    db $FC,$FC,$0C,$0C,$18,$30,$60,$60 ; 7
    db $78,$FC,$CC,$CC,$78,$FC,$CC,$CC ; 8
    db $78,$FC,$CC,$CC,$7C,$3C,$0C,$0C ; 9

    ; 58-64: Punctuation
    db $00,$18,$18,$00,$00,$18,$18,$00 ; :
    db $00,$18,$18,$00,$00,$18,$18,$30 ; ;
    db $0C,$18,$30,$60,$30,$18,$0C,$00 ; <
    db $00,$00,$7E,$00,$7E,$00,$00,$00 ; =
    db $30,$18,$0C,$06,$0C,$18,$30,$00 ; >
    db $78,$FC,$0C,$18,$30,$00,$30,$00 ; ?
    db $78,$CC,$DE,$DE,$DE,$C0,$78,$00 ; @

    ; 65-90: Uppercase A-Z
    db $30,$78,$CC,$CC,$FC,$FC,$CC,$CC ; A
    db $FC,$FC,$CC,$FC,$FC,$CC,$FC,$FC ; B
    db $78,$FC,$C0,$C0,$C0,$C0,$FC,$78 ; C
    db $F8,$FC,$CC,$CC,$CC,$CC,$FC,$F8 ; D
    db $FC,$FC,$C0,$F8,$F8,$C0,$FC,$FC ; E
    db $FC,$FC,$C0,$F8,$F8,$C0,$C0,$C0 ; F
    db $78,$FC,$C0,$CC,$CC,$CC,$FC,$7C ; G
    db $CC,$CC,$CC,$FC,$FC,$CC,$CC,$CC ; H
    db $78,$78,$30,$30,$30,$30,$78,$78 ; I
    db $1E,$1E,$0C,$0C,$0C,$CC,$FC,$78 ; J
    db $CC,$CC,$D8,$F0,$F0,$D8,$CC,$CC ; K
    db $C0,$C0,$C0,$C0,$C0,$C0,$FC,$FC ; L
    db $82,$C6,$EE,$FE,$D6,$D6,$C6,$C6 ; M
    db $CC,$CC,$EC,$EC,$FC,$DC,$DC,$CC ; N
    db $78,$FC,$CC,$CC,$CC,$CC,$FC,$78 ; O
    db $FC,$FC,$CC,$FC,$F0,$C0,$C0,$C0 ; P
    db $78,$FC,$CC,$CC,$CC,$DC,$FC,$7E ; Q
    db $FC,$FC,$CC,$FC,$F8,$DC,$CC,$CC ; R
    db $78,$FC,$C0,$78,$3C,$0C,$FC,$78 ; S
    db $FC,$FC,$30,$30,$30,$30,$30,$30 ; T
    db $CC,$CC,$CC,$CC,$CC,$CC,$FC,$78 ; U
    db $CC,$CC,$CC,$CC,$CC,$78,$30,$30 ; V

    db $C6,$C6,$C6,$D6,$D6,$FE,$EE,$C6 ; W

    db $CC,$CC,$78,$30,$30,$78,$CC,$CC ; X
    db $CC,$CC,$CC,$78,$30,$30,$30,$30 ; Y
    db $FC,$FC,$0C,$18,$30,$60,$FC,$FC ; Z

    ; 91-96: Brackets etc
    db $78,$60,$60,$60,$60,$60,$60,$78 ; [
    db $C0,$60,$30,$18,$0C,$06,$03,$00 ; \
    db $1E,$06,$06,$06,$06,$06,$06,$1E ; ]
    db $30,$78,$CC,$00,$00,$00,$00,$00 ; ^
    db $00,$00,$00,$00,$00,$00,$00,$FF ; _
    db $60,$60,$30,$00,$00,$00,$00,$00 ; `

    ; 97-122: Lowercase a-z
    db $00,$00,$7C,$FC,$CC,$CC,$FC,$7C ; a
    db $C0,$C0,$F8,$FC,$CC,$CC,$FC,$F8 ; b
    db $00,$00,$7C,$FC,$C0,$C0,$FC,$7C ; c
    db $0C,$0C,$7C,$FC,$CC,$CC,$FC,$7C ; d
    db $00,$00,$7C,$FC,$FC,$C0,$FC,$7C ; e
    db $30,$70,$60,$F8,$F8,$60,$60,$60 ; f
    db $00,$7C,$FC,$CC,$7C,$0C,$FC,$F0 ; g
    db $C0,$C0,$F0,$F8,$CC,$CC,$CC,$CC ; h
    db $30,$30,$00,$30,$30,$30,$30,$30 ; i
    db $0C,$0C,$00,$0C,$0C,$0C,$0C,$F8 ; j
    db $C0,$C0,$CC,$D8,$F0,$D8,$CC,$CC ; k
    db $30,$30,$30,$30,$30,$30,$30,$30 ; l
    db $00,$00,$FC,$FE,$DB,$DB,$DB,$DB ; m
    db $00,$00,$F0,$F8,$CC,$CC,$CC,$CC ; n
    db $00,$00,$78,$FC,$CC,$CC,$FC,$78 ; o
    db $00,$00,$FC,$FC,$CC,$FC,$C0,$C0 ; p
    db $00,$00,$7C,$FC,$CC,$7C,$0C,$0C ; q
    db $00,$00,$D8,$FC,$C0,$C0,$C0,$C0 ; r
    db $00,$00,$7C,$FC,$E0,$7C,$1C,$FC ; s
    db $30,$30,$FC,$FC,$30,$30,$30,$30 ; t
    db $00,$00,$CC,$CC,$CC,$CC,$FC,$7C ; u
    db $00,$00,$CC,$CC,$CC,$78,$30,$30 ; v

    db $00,$00,$DB,$DB,$DB,$DB,$7F,$3F ; w

    db $00,$00,$CC,$78,$30,$78,$CC,$CC ; x
    db $00,$00,$CC,$CC,$7C,$0C,$FC,$F0 ; y
    db $00,$00,$FC,$18,$30,$60,$FC,$FC ; z