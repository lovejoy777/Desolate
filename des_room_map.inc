Room_Links:
    ;------------------------------------------------------------------
    ; Room 00 doors
    ;------------------------------------------------------------------

    ; --- Room 0 to Room 1 ---
    .db 0, 1, 0, 0        ; From room 0, to room 1, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; Spawn Coords, north(85,82), south (85,34), east(18,48), west(154,48) 
    .dl room_01           ; New Room name to load. 
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 01 doors
    ;------------------------------------------------------------------
    ; --- Room 1 to Room 0 ---
    .db 1, 0, 1, 1        ; From room 1, to room 0, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_00           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 1 to Room 2 ---
    .db 1, 2, 0, 0        ; From room 1, to room 2, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_02           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 2 doors
    ;------------------------------------------------------------------
    ; --- Room 2 to Room 1 ---
    .db 2, 1, 1, 1        ; From room 2, to room 1, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_01           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 2 to Room 3 ---
    .db 2, 3, 2, 2        ; From room 2, to room 3, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 10, 48            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_03           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 2 to Room 5 ---
    .db 2, 5, 0, 0        ; From room 2, to room 5, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_05           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 3 doors
    ;------------------------------------------------------------------
    ; --- Room 3 to Room 2 ---
    .db 3, 2, 3, 3        ; From room 3, to room 2, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 154, 48           ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_02           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 3 to Room 4 ---
    .db 3, 4, 1, 1        ; From room 3, to room 4, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; going north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_04           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 4 doors
    ;------------------------------------------------------------------
    ; --- Room 4 to Room 3 ---
    .db 4, 3, 0, 0        ; From room 4, to room 3, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_03           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment   

    ;------------------------------------------------------------------
    ; Room 5 doors
    ;------------------------------------------------------------------
    ; --- Room 5 to Room 2 ---
    .db 5, 2, 1, 1        ; From room 5, to room 2, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_02           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 5 to Room 8 ---
    ;.db 5, 8, 0, 0        ; From room 5, to room 8, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    ;.dw 79, 34            ; north(85,82), south (85,34), east(18,48), west(154,48)
    ;.dl room_08           ; New Room name to load.
    ;.db 0                 ; Padding for 12-byte alignment


    ;=============================================================

    ; testing 66
    ; --- Room 5 to Room 66 ---
    .db 5, 66, 0, 0        ; From room 5, to room 66, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; Spawn Coords, north(85,82), south (85,34), east(18,48), west(154,48) 
    .dl room_66           ; New Room name to load. 
    .db 0                 ; Padding for 12-byte alignment
    
    ; testing 66
    ; --- Room 66 to Room 5 ---
    .db 66, 5, 1, 1        ; From room 66, to room 5, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_05           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;=============================================================

    ;------------------------------------------------------------------
    ; Room 6 does not exist
    ;------------------------------------------------------------------ 

    ;------------------------------------------------------------------
    ; Room 7 doors
    ;------------------------------------------------------------------
    ; --- Room 7 to Room 10 ---
    .db 7, 10, 1, 1        ; From room 7, to room 10, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_10           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 7 to Room 9 ---
    .db 7, 9, 0, 0        ; From room 7, to room 9, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_09           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 8 doors
    ;------------------------------------------------------------------
    ; --- Room 8 to Room 5 ---
    .db 8, 5, 1, 1        ; From room 8, to room 5, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_05           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 8 to Room 9 ---
    .db 8, 9, 2, 2        ; From room 8, to room 9, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 10, 48            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_03           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 9 doors
    ;------------------------------------------------------------------
    ; --- Room 9 to Room 8 ---
    .db 9, 8, 3, 3        ; From room 9, to room 8, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 154, 48           ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_08           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 9 to Room 7 ---
    .db 9, 7, 1, 1        ; From room 3, to room 4, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; going north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_07           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 10 doors
    ;------------------------------------------------------------------
    ; --- Room 10 to Room 7 ---
    .db 10, 7, 0, 0       ; From room 10, to room 7, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_07           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 10 to Room 11 ---
    .db 10, 11, 2, 2      ; From room 10, to room 11, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 10, 48            ; going north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_11           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 11 doors
    ;------------------------------------------------------------------
    ; --- Room 11 to Room 10 ---
    .db 11, 10, 3, 3      ; From room 11, to room 10, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 154, 48            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_10           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 11 to Room 12 ---
    .db 11, 12, 1, 1      ; From room, to room, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_12           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 11 to Room 28 ---
    .db 11, 28, 0, 0      ; From room, to room, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; going north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_28           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ;------------------------------------------------------------------
    ; Room 12 doors
    ;------------------------------------------------------------------
    ; --- Room 12 to Room 13 ---
    .db 12, 13, 1, 1      ; From room, to room, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 82            ; north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_13           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment

    ; --- Room 12 to Room 11 ---
    .db 12, 11, 0, 0      ; From room, to room, Door hit Dir S(0)N(1)E(2)W(3), facing Dir  S(0),N(1),E(2),W(3)
    .dw 79, 34            ; going north(85,82), south (85,34), east(18,48), west(154,48)
    .dl room_11           ; New Room name to load.
    .db 0                 ; Padding for 12-byte alignment


    .db $FF               ; End marker