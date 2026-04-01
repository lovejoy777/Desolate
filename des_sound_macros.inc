; -----------------------------------------------------------------------------
; SOUND MACROS FILE
; -----------------------------------------------------------------------------

; Play a sound on a channel
; Arguments:
;   chan: Channel (0-3)
;   vol:  Volume (0-127)
;   freq: Frequency (Hz)
;   dur:  Duration (ms)

; Example Basic Sounds
; PLAY_SOUND 0, 127, 400, 150     ; Mid note
; PLAY_SOUND 0, 127, 300, 150     ; Lower note
; PLAY_SOUND 0, 127, 200, 400     ; Long low note
; PLAY_SOUND 1, 100, 50, 500      ; Add a bit of white noise "crash"

macro PLAY_SOUND chan, vol, freq, dur
    ld a, 23 
    rst.lil $10
    ld a, 0 
    rst.lil $10
    ld a, $85       ;133 dec
    rst.lil $10
    ld a, chan 
    rst.lil $10
    ld a, 0              ; Command 0: Play Note
    rst.lil $10 
    ld a, vol 
    rst.lil $10
    ld a, freq & $FF 
    rst.lil $10
    ld a, freq >> 8 & $FF 
    rst.lil $10
    ld a, dur & $FF 
    rst.lil $10
    ld a, dur >> 8 & $FF 
    rst.lil $10
endmacro

; Set Sound Waveform
; Arguments:
;   chan: Channel (0-3)
;   wave: Waveform type 
;         0=Square, 1=Triangle, 2=Sawtooth, 3=Sine, 4=Noise, 5=VIC Noise
macro SET_SOUND_WAVEFORM chan, wave
    ld a, 23
    rst.lil $10
    ld a, 0
    rst.lil $10
    ld a, $85          ; 133 dec
    rst.lil $10
    ld a, chan
    rst.lil $10
    ld a, 4             ; Command 4: Set Waveform
    rst.lil $10
    ld a, wave
    rst.lil $10
endmacro