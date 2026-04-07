;-----------------------
; File: des_sounds.inc
; Logic: 
;------------------------

; Command 0: Play Note
; VDU 23, 0, $85, channel, 0, volume, frequency; duration;

; Command 1: Status
; VDU 23, 0, $85, channel, 1

; Command 2: Set Volume
; VDU 23, 0, $85, channel, 2, volume

; Command 3: Set Frequency
; VDU 23, 0, $85, channel, 3, frequency;

; Command 4: Set waveform
; VDU 23, 0, $85, channel, 4, waveformOrSample, [bufferId;]

; Command 5: Sample management
; VDU 23, 0, $85, channelOrSample, 5, sampleCommand, [parameters]

; Command 5, 0: Load sample
; VDU 23, 0, $85, sample, 5, 0, length; lengthHighByte, <sampleData>

; Command 5, 1: Clear sample
; VDU 23, 0, $85, sample, 5, 1

; Command 5, 2: Create a sample from a buffer
; VDU 23, 0, $85, channel, 5, 2, bufferId; format, [sampleRate;]

; Command ?: Select sample
; VDU 23, 0, $85, channel, 4, 8, bufferId;

; Command 5, 3: Set sample base frequency
; VDU 23, 0, $85, sample, 5, 3, frequency;

; Command 5, 4: Set sample frequency for a sample by buffer ID
; VDU 23, 0, $85, channel, 5, 4, bufferId; frequency;

; Command 6: Volume envelope
; VDU 23, 0, $85, channel, 6, type, [parameters]

; Command 7: Frequency envelope
; VDU 23, 0, $85, channel, 7, type, [parameters]

; Command 8: Enable Channel
; VDU 23, 0, $85, channel, 8

; Command 9: Disable Channel
; VDU 23, 0, $85, channel, 9

; Command 10: Reset Channel
; VDU 23, 0, $85, channel, 10

; Command 11: Seek to position
; VDU 23, 0, $85, channel, 11, position; positionHighByte

; Command 12: Set duration
; VDU 23, 0, $85, channel, 12, duration; durationHighByte

; Command 13: Set sample rate
; VDU 23, 0, $85, channel, 13, sampleRate;

; Command 14: Set channel waveform parameters
; VDU 23, 0, $85, channel, 14, parameter, value

;-------------------------------------------------------
; Set Sound Waveform
; Arguments:
;   chan: Channel (0-3)
;   wave: Waveform type 
;   0=Square, 1=Triangle, 2=Sawtooth, 3=Sine, 4=Noise, 5=VIC Noise
;
; SET_SOUND_WAVEFORM chan, wave
;-------------------------------------------------------
;-------------------------------------------------------
; Play a sound on a channel
; Arguments:
;   chan: Channel (0-3)
;   vol:  Volume (0-127)
;   freq: Frequency (Hz)
;   dur:  Duration (ms)
;
; PLAY_SOUND chan, vol, freq, dur
;--------------------------------------------------------

;-------------------------------------------
; Desolate Sounds
;--------------------------------------------

sound_play_crash:
    PLAY_SOUND 0, 127, 400, 150     ; Mid note
    PLAY_SOUND 0, 127, 300, 150     ; Lower note
    PLAY_SOUND 0, 127, 200, 400     ; Long low note
    PLAY_SOUND 1, 100, 50, 500      ; Add a bit of white noise "crash"

    ret

sound_play_bump:
    SET_SOUND_WAVEFORM 0, 1
    ;PLAY_SOUND 0, 127, 400, 150     ; Mid note
    PLAY_SOUND 0, 30, 300, 30     ; Lower note
    PLAY_SOUND 0, 40, 700, 30     ; Long low note
   ; PLAY_SOUND 1, 100, 50, 500      ; Add a bit of white noise "crash"

    ret



    ; 10 REM Set channel 1 to use an ADSR envelope
    ; 20 VDU 23, 0, &85, 1, 6, 1, 400; 100; 100, 2000;
    ; 30 REM Play a note on channel 1
    ; 40 VDU 23, 0, &85, 1, 0, 60, 440; 1500;






;----------------
; Sound SFX
;----------------
;---------------
; Sound Set-Up
;---------------
playMenu_nav:
    ; Menu Navigation sfx
    ld hl, startMenu_nav
    ld bc, endMenu_nav - startMenu_nav
    rst.lil $18
    ret
startMenu_nav:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-1                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 10                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 100                             ; duration (ignored for samples) .init_small
endMenu_nav:

playNav_tick:
    ; Menu Navigation sfx
    ld hl, startNav_tick
    ld bc, endNav_tick - startNav_tick
    rst.lil $18
    ret
startNav_tick:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-2                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 6                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 100                             ; duration (ignored for samples) .init_small
endNav_tick:

playSelect:
    ; Select sfx
    ld hl, startSelect
    ld bc, endSelect - startSelect
    rst.lil $18
    ret
startSelect:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-3                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 20                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 200                             ; duration (ignored for samples) .init_small
endSelect:

playLaser:
    ; Laser sfx
    ld hl, startLaser
    ld bc, endLaser - startLaser
    rst.lil $18
    ret
startLaser:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-5                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 30                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 100                             ; duration (ignored for samples) .init_small
endLaser:

playBump:
    ; Bump sfx
    ld hl, startBump
    ld bc, endBump - startBump
    rst.lil $18
    ret
startBump:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-6                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 10                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 200                             ; duration (ignored for samples) .init_small
endBump:

playDoor:
    ; Laser sfx
    ld hl, startDoor
    ld bc, endDoor - startDoor
    rst.lil $18
    ret
startDoor:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-7                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 8                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 600                            ; duration (ignored for samples) .init_small
endDoor:

playError:
    ; Error sfx
    ld hl, startError
    ld bc, endError - startError
    rst.lil $18
    ret
startError:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-8                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 40                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 100                             ; duration (ignored for samples) .init_small
endError:

playGlitch:
    ; Error sfx
    ld hl, startGlitch
    ld bc, endGlitch - startGlitch
    rst.lil $18
    ret
startGlitch:
    ; Command 0: Play note
    ; VDU 23, 0, &85, Channel, Command, Volume, Frequency; Duration;
    ; set wave form as sample
    .db 23,0,$85                        ; do sound (audio command)
    .db 1,4,-9                          ; channel 1, command 4, sample id -1
    ; Play Note
    .db 23,0,$85                        ; do sound
    .db 1,0, 10                        ; channel 1, command 0 (play note), volume 127 (max)
    .dw 125                             ; freq (ignored for samples)
    .dw 100                             ; duration (ignored for samples) .init_small
endGlitch:



;------------------------------------
; Experimental for native envelopes.
;------------------------------------
playZap:
    ; Zap sfx
    ld hl, startZap
    ld bc, endZap - startZap
    rst.lil $18
    ret
startZap:
    ;------------------------------
    ; Command 6: Volume envelope
    ;------------------------------
    ; Type 1: ADSR
    ; VDU 23, 0, &85, channel, 6, 1, attack; decay; sustain, release;
    .db 23,0,$85                       ; Do sound (audio command)
    .db 0,6,1                          ; Channel 0, command 6, type 1

    .dw 126,50                         ; Attack 400, Decay 100
    .db 50                             ; Sustain 100
    .dw 100                             ; release 2000 m/s

    ;--------------------------------
    ; Command 7: Frequency envelope
    ;--------------------------------
    ; VDU 23, 0, &85, channel, 7, type, [parameters]
    ; VDU 23, 0, &85, channel, 7, 1, phaseCount, controlByte, stepLength;
    .db 23,0,$85                       ; Do sound (audio command)
    .db 0,7,1                          ; Channel 0, command 7, type 1 (stepped freq)
    .db 2,3                            ; phaseCount, controlByte

    .dw 30                             ; stepLength m/s
    .dw 40                             ; phase 1 adjustment
    .dw 6                              ; phase 1 number of steps
    .dw -30                            ; phase 2 adjustment
    .dw 4                              ; phase 2 number of steps

    ; Play Note
    .db 23,0,$85                        ; do sound:  audio command
    .db 0,0,127                         ; channel 1: command 0 (play note), volume 60
    .dw 550                             ; freq: note we are playing is an "A" (440Hz)
    .dw 40                            ; duration: 1500 m/s
endZap:




;----------
; usage:
;----------
   ; laser sfx
   ; call playLaser

   ; Scifi Door sfx
   ; call playDoor

   ; Footsteps sfx
   ; call playFootsteps

