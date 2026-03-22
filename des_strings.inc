; --- From desolstrs.asm and new Agon Strings --- SE131
controls_string1:
    db "     Controls", 0
controls_string2:
    db "Space = Action", 0
controls_string3:
    db "Z = Look Mode", 0
controls_string4:
    db "X = Shoot Mode", 0
controls_string5:
    db "I = Inventory", 0
controls_string6:
    db "C = Close Popups", 0
controls_string7:
    db "R = Restart Game", 0
controls_string8:
    db "S = Start Game", 0


restart_string:
    db "  Restart New Game?||    Press Y or N", 0
SE129:
    db "You dont seem to be able", 0
SE12A:
    db "to use this item here", 0
SE12B:
    db "You dont have any time", 0
SE12C:
    db "to play with this now", 0
SE12D:
    db "It doesnt look like you", 0
SE12E:
    db "can do anything else here", 0
SE12F:
    db "This Generator is damaged", 0
SE130:
    db "All of the panels are loose", 0

; The "No Power" Terminal String
SE131:
    db "This workstation|doesnt seem to|have any power...? ", 0
SE133:
    db "The Workstation has now|successfully booted up", 0

; Game Over Screen Strings
Dead_String_1:                            
    .db "The Desolate has claimed", 0     ; game over string 1
Dead_String_2:                            
    .db "your life too . . .", 0          ; game over string 2

; Inventory Popup Strings
Inv_Title_String:                         
    .db "-INVENTORY-", 0                  ; inventory title string
Inv_Item_String:                          
    .db "No Items", 0                     ; inventory item string

; Debug Strings
; Current Room Number
Debug_CRN_String:                         
    .db "CRN:", 0    
; Is Small Alien Active              
Debug_SAA_String:                        
    .db "SAA:", 0  
    ; Is Big Alien Active
Debug_BAA_String:                        
    .db "BAA:", 0 
; Players Current Direction           
Debug_PCD_String:                          
    .db "PCD:", 0   
; Tile ID Under Player                  
Debug_TPO_String:                        
    .db "TPO:", 0   
; Tile ID In Front Of Player               
Debug_TIP_String:                          
    .db "TIP:", 0   
; Look/Shoot Mode                  
Debug_LSM_String:                        
    .db "LSM:", 0   
; Small Alien HP                  
Debug_SHP_String:                        
    .db "SHP:", 0   
; Big Alien HP                  
Debug_BHP_String:                        
    .db "BHP:", 0        