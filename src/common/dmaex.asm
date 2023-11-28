.include "hdr.asm"

.EQU REG_TMW               $212E
.EQU REG_W12SEL            $2123
.EQU REG_W34SEL            $2124
.EQU REG_WOBJSEL           $2125
.EQU REG_HDMAEN            $420C

.SECTION ".dmas14_channels_6_and_7_text" SUPERFREE

; void setModeHdmaWindow67(u8 bgrnd, u8 bgrndmask,u8* hdmatableL,u8* hdmatableR)
; 8 9 10-13 14-17
setModeHdmaWindow67:
    php
    phb
    phx

    sep #$20
    lda #0
    pha
    plb

    lda 8,s                                                   ; got all the flags to active windows on BG1..4
    ora #$10                                                  ; also add obj in window effect
    sta REG_TMW                                               ; active or not window

    lda 8,s
    and #$0C                                                  ; if effect on BG3 or BG4, not same register
    bne +

    lda 9,s                                                   ; got all the flags to mask effect (inside, outside on BG1..2)
    sta REG_W12SEL
    bra ++
+:  
    lda 9,s                                                   ; got all the flags to mask effect (inside, outside on BG3..4)
    sta REG_W34SEL

++: lda 9,s                                                   ; todo : find a way to manage easily objects -> currently, it works only for BG1 
    sta REG_WOBJSEL
    
    stz $4360                                                 ; 1 register, write once
    lda #$28                                                  ; 2128  Window 1 Left Position (X1)
    sta $4361                                                 ; destination
    
    lda 12,s                                                  ; bank address of left  table
    sta $4364 
    
    stz $4370                                                 ; 1 register, write once
    lda #$29                                                  ; 2129 Window 1 Right Position (X2)
    sta $4371 
    
    lda 16,s                                                  ; bank address of right table
    sta $4374 

    rep #$20
    
    lda 10,s                                                  ; low address of left table
    sta $4362                                                 ; low address of right table
    
    lda 14,s                                                
    sta $4372 

    sep #$20
    lda #$C0                                                  ; channel 6 & 7       11000000
    sta.l   REG_HDMAEN

    plx
    plb
    plp
    rtl 

.ENDS

.SECTION ".dmaCopyVramChannel1_text" SUPERFREE

;---------------------------------------------------------------------------
; void dmaCopyVramChannel1(u8 * source, u16 address, u16 size);
dmaCopyVramChannel1:
    php

;   jsr.w   _wait_nmid
    rep #$20
    lda 9,s 
    sta.l   $2116           ; address for VRAM write(or read)

    lda 11,s
    sta.l   $4315           ; number of bytes to be copied
    lda 5,s 
    sta.l   $4312           ; data offset in memory

    sep #$20                ; 8bit A
    lda #$80
    sta.l   $2115           ; VRAM address increment value designation
    lda 7,s                 ; bank address of data in memory
    sta.l   $4314
    lda #1
    sta.l   $4310           ; 1= word increment
    lda #$18
    sta.l   $4311           ; 2118 is the VRAM gate

    lda #2                  ; turn on bit 1 (channel 0) of DMA
    sta.l   $420b

    plp
    rtl

.ENDS
