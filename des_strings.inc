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


; Game Over Screen Strings
Dead_String_1:                            
    .db "The Desolate Claimed", 0      ; game over string 1
Dead_String_2:                            
    .db "your life too...", 0          ; game over string 2
Dead_String_3:                            
    .db "Press the M Key", 0           ; game over string 

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
; Aliens Killed                  
Debug_KIL_String:                        
    .db "KIL:", 0        


test_A_String:                        
    .db "a", 0 
test_K_String:                        
    .db "k", 0 
test_I_String:                        
    .db "i", 0 


; Credits
SE02B: .db "Code, GFX, Story etc",0
SE02D: .db "Patrick Prendergast",0
SE02F: .db "Acknowledgements",0
SE031: .db "Durk Kingma - Grayscale",0
SE033: .db "Kerey Roper - Huffman",0
SE035: .db "Joe Pemberton - RLE",0
SE037: .db "Joe Wingbermuehle - Ion",0
SE039: .db "Tijl Coosemans - Venus",0
SE03B: .db "DetachedS - MirageOS",0
SE03D: .db "Testers",0
SE03F: .db "David Sleight",0
SE041: .db "John Sleight",0
SE043: .db "Durk Kingma",0
SE045: .db "Joe Pemberton",0
SE047: .db "Shawn McAndrews",0
SE049: .db "Tom King",0
SE04B: .db "Domi Alex",0
SE04D: .db "Sammy Griff",0
SE04F: .db "Dennis Tseng",0
SE051: .db "AndySoft",0
SE053: .db "jedbouy",0
SE055: .db "Bram Tant",0
SE057: .db "Martin Warmer",0
SE059: .db "Vincent Junemann",0
SE05B: .db "Michael Angel",0
SE05D: .db "Special Thanks & Greetz",0
SE05F: .db "Everyone @ MaxCoderz",0
SE061: .db "Everyone on #TCPA (EFNet)",0
SE063: .db "ticalc.org",0
SE065: .db "No animals were hurt more",0
SE067: .db "than once during the",0
SE069: .db "making of this game.",0
SE06B: .db "Just Kidding ;P",0
SE06D: .db "Thanks for Playing!",0
SE06F: .db "visit www.MaxCoderz.com",0
SE071: .db "ABlakRain",0
SE073: .db "Travis Supalla",0
SE075: .db "Jim Dieckmann",0
SE077: .db "tr1p1ea@yahoo.com.au",0
SCR40: .db "ported to the",0
SCR41: .db "Agon Light 2",0
SCR42: .db "by lovejoy777",0
SCR43: .db "2026 +",0
; Data Cartridge messages
SE079: .db "Im hurt bad . . . I dont|think im gonna make it.|I changed the Level 1|Access Code to: 4057|"
       .db "Maybe that will hold|them off for a while . . .",0
SE07B: .db "For security reasons|I had to change the|Level 2 Access Code.|Maybe now the crew|will stop",$02
       .db "stealing",$02,"stuff!|It is:",0
SE07D: .db "The system is going|haywire. The Level 3|Access Code was over-|written. I only just|recovered it."
       .db "Turns",$02,"out|it is:",0
SE07F: .db "Crew I am honoured to|have served as your|captain. The Level 4|Access Code will get|"
       .db "to the evacuation deck|It is:",0
SE081: .db "Willis and I are stuck.|Needless to say we are|both done for.",$02
       .db "We",$02,"might|have had a chance, but|some idiot changed the|Level 1 Access Code!",0
SE083: .db "I",$02,"dont see",$02,"why I",$02,"should|be the one who has to|fix the generator.|"
       .db "There is no way im|going with those things|all over the place . . .",0
SE085: .db "DrMorgan: Meteorite|shower claimed a lot|of the crew, however|some feature strange|abrations. "
       .db "Almost like|bite marks . . ?",0
SE087: .db "DrMorgan: I found a|strange creature. At|first I thought it was|deceased.",$02
       .db "But",$02,"it",$02,"sprang|up and attacked my|associate! . . .",0
SE089: .db "DrMorgan: To make|matters worse the|creatures are|evolving rapidly. My|associate claims he|"
       .db "saw one over 7 feet!",0
SE08B: .db "It truly baffles me|considering our cargo|why they insist on|equipping the ship with|ONLY "
       .db "Ion-Phasers.|They are useless!!!",0
SE08D: .db "Capt Millin: We are|losing crew fast.",$02,"I",$02,"sent|Willis to re-animate|the clones. "
       .db "Right now|they are the only|chance we have . . .",0
SE08F: .db "Capt Millin: I am very|anxious to see the|clones in action. I|hear that they are the|"
       .db "most advanced clone|soldiers ever created.",0
SE091: .db "Capt Millin: We are|transporting a cargo|of genetically|modified clones to a|IH-2 Military Facility|"
       .db "for field testing . . .",0
SE093: .db "Capt Millin: I have|just recieved word|that the facility on|earth was destroyed.|"
       .db "We now carry the only|clone prototypes.",0
SE095: .db "Willis: We are in BIG|trouble.",$02,"Only one",$02,"clone|survived the",$02,"meteorite|shower.",$02
       .db "And the",$02,"system|says Re-animation will|take over 6 hours!!!",0
SE097: .db "This is ludicrous!|65 credits for",$02,"a proton|bar? How are we|meant to snack while|we work? "
       .db "The agency|is",$02,"getting",$02,"on",$02,"my",$02,"nerves.",0
;
SE09B: .db $10,"No Data Cartridge|",$1F,$04,"Selected",0
SE09D: .db "MaxCoderz Presents",0
SE09F: .db "a tr1p1ea game",0
SE0A1: .db "Items Found (/24)|Enemies Killed|Player Deaths|Awards||Press 'm' for menu",0
SE0A3: .db "OverWrite Current Game?|Enter - Yes :: G - No",0
SE0A5: .db "- Controls -",0
SE0A7: .db "Arrows: Movement|Space: Look/shoot|I: Inventory|Z/X: Look/shoot mode|"
       .db "C: Close all pop-ups|M: Main Menu",0
SE0A9: .db "Earn 3 Good Awards for|an Extended Ending!",0
SE0AB: .db "Sir Miss-A-Lot",0
SE0AD: .db "Sherlock Holmes",0
SE0AF: .db "Running Scared",0
SE0B1: .db "Terminator",0
SE0B3: .db "Over & Over Again",0
SE0B5: .db "Survivor",0
SE0B7: .db $18,"Ion Phaser",0
SE0B9: .db "$",0  ; Arrow down
SE0BB: .db " - INVENTORY - ",0
; Death
SE0BD: .db "The Desolate has claimed|your life too...||",0
; Messages
SE0BF: .db "OMG! This Person Is DEAD!",0
SE0C1: .db "What Happened Here!?!",0
SE0C3: .db $0A,"Another Dead Person.",0
SE0C5: .db " Search Reveals Nothing",0
SE0C7: .db $0F,"This Person is Dead . . .",0
SE0C9: .db "They Seem To Be Holding",0
SE0CB: .db "Something",0
SE0CD: .db $0F,"Hey Whats This",$06,".",$06,".",$06,". ?",0
SE0CF: .db "You Picked Up A",0
SE0D1: .db "You Already Have The",0
SE0D3: .db "You dont have a|",$1E,"Weapon to equip!",0
SE0D5: .db "It is not wise to proceed|",$19,"without a weapon.",0
SE0D7: .db "You cant enter that sector|",$09,"Life-Support is offline.",0
SE0D9: .db "You cant enter until the|AirLock is re-pressurised",0
SE0DB: .db "---- N o  I t e m ----", 0
SE0DD: .db $05,"Door Locked",0
SE0DF: .db "INVALID",$02,"CODE",0
SE0E1: .db $0E,"Accepted!",0
SE0E3: .db $1F,"You Need A|Data Cartridge Reader",0
; Inventory items
SE0E5: .db $06,"Data Cartridge Reader",0
SE0E7: .db $18,"Data Cartridge 1",0
SE0E9: .db $18,"Data Cartridge 2",0
SE0EB: .db $18,"Data Cartridge 3",0
SE0ED: .db $18,"Data Cartridge 4",0
SE0EF: .db $18,"Data Cartridge 5",0
SE0F1: .db $18,"Data Cartridge 6",0
SE0F3: .db $18,"Data Cartridge 7",0
SE0F5: .db $18,"Data Cartridge 8",0
SE0F7: .db $18,"Data Cartridge 9",0
SE0F9: .db $14,"Data Cartridge 10",0
SE0FB: .db $14,"Data Cartridge 11",0
SE0FD: .db $14,"Data Cartridge 12",0
SE0FF: .db $14,"Data Cartridge 13",0
SE101: .db $14,"Data Cartridge 14",0
SE103: .db $14,"Data Cartridge 15",0
SE105: .db $14,"Data Cartridge 16",0
SE107: .db $1F,$0B,"Power Drill",0
SE109: .db $06,"Life Support Data Disk",0
SE10B: .db $1F,$06,"Air-Lock Tool",0
SE10D: .db $12,"Box of Power Cells",0
SE10F: .db $1F,$07,"Pile of Parts",0
SE111: .db $1F,$0A,"Duck Idol ;)",0
SE113: .db $1F,$08,"Rubik's Cube",0
; Story
SE115: .db "In the Distant Future.....",0
SE117: .db "'The Desolate' Space Cruiser|leaves orbit. Its mission is|secret, its cargo classified.|"
       .db "6014 Cycles into the journey|the ship encounters a savage|meteorite shower.||"
       .db "Contact with Desolate is lost.",0
SE119: .db "The ship sustains heavy|damage. Onboard a cryo-|genic incubation cell|finishes re-animation. "
       .db "Its|occupant steps out of the|chamber not knowing who he|is, or what he is going to do.||"
       .db "But at least he is alive.",0
; Escape
SE11B: .db "The onboard guidance|system picks up a mining|facility close by. The course|is set",$02
       .db "and you sit back,",$02,"finally|free of 'The Desolate' & its|murderous hord of Aliens.|"
       .db "You were a clone, an|experiment. Now, 'you' are|the sole survivor.",0
SE11D: .db "System Alert triggered: |'Foreign Objects Detected|OnBoard'. The Aliens have|attached to the hull!|"
       .db "'Main Drive System Offline'|The ship was on a crash|course for",$02,"the Mining Facility||"
       .db "It wasnt over yet",$06,".",$06,".",$06,".",0
SE11F: .db "The End",0
;
SAccessLevel1: .db $1B,"Level 1|",$09,"Access Code|",$15,"Required",0
SAccessLevel2: .db $1B,"Level 2|",$09,"Access Code|",$15,"Required",0
SAccessLevel3: .db $1B,"Level 3|",$09,"Access Code|",$15,"Required",0
SAccessLevel4: .db $1B,"Level 4|",$09,"Access Code|",$15,"Required",0
;
SE129: .db "You dont seem to be able|",$0F,"to use this item here",0
SE12B: .db "You dont have any time|",$08,"to play with this now",0
SE12D: .db $10,"It doesnt look like you|",$04,"can do anything else here",0
SE12F: .db "This Generator is damaged|All of the panels are loose",0
SE131: .db $0F,"This Workstation doesnt|seem to have any power...?",0
SE133: .db $0A,"The Workstation has now|",$09,"successfully booted up",$06,"$",0
SE135: .db "The Workstation Ejected A|",$1F,$05,"Data Cartridge 2",0
SE137: .db " You use the Power Drill|to Repair the Generator",0
SE139: .db $12,"Life-Support System|has been fully restored",0
SE13B: .db "The Evacuation Deck has|",$09,"been re-pressurised",0
SE13D: .db "You Insert a Power Cell.|Guidance System Online",0
SE13F: .db $0C,"The Life Support System|",$15,"needs Re-Configuring",0
SE141: .db $1F,$04,"AirLock Control &| Re-Pressurisation Station",0
SE143: .db $05,"This MainFrame is missing|",$1F,$16,"a Power Cell",0
SE145: .db $09,"This Pod cant naviagate.|Guidance System is offline",0
SQuit: .db "Its now safe to turn off|",$1F,$0E,"your computer.",0