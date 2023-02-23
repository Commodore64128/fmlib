#include <stdio.h>
#include <unistd.h>
#include "fm.h"
#include "fm_parse.h"

void play_instruments() {

    // set volume
    fm_setchannelvol(1, 255);
    fm_setchannelvol(2, 255);
    fm_setchannelvol(3, 255);

    printf(" Piano...\n");
    fm_setinstrument(PIANO);
    fm_playsound(1, 4, 0);
    sleep(2);

    printf(" Brass...\n");
    fm_setinstrument(BRASS);
    fm_playsound(1, 4, 0);
    sleep(2);

    printf(" Guitar...\n");
    fm_setinstrument(GUITAR);
    fm_playsound(1, 4, 0);
    sleep(2);

}

void play_chord() {

    fm_setchannelvol(1, 255);
    fm_setchannelvol(2, 255);
    fm_setchannelvol(3, 255);

    fm_setinstrument(PIANO);

    printf("\nMajor C Chord\n");
    fm_playsound(1, 4, 0);
    fm_playsound(2, 4, 4);
    fm_playsound(3, 3, 7);
    sleep(4);
}

int play_music() {

    char play_command[] = "G3e,G3e,A3e,G3e,C4e,B3h,G3e,G3e,A3e,G3e,D4e,C4h,G3e,G3e,G4e,E4e,C4e,B3e,A3h,F4e,F4e,E4e,C4e,D4e,C4w";
    Note notes[MAX_NOTES];
 
    uint8_t i;
    int note_count;
 
    uint32_t jiffystart;
    uint32_t jiffynow;

    fm_setchannelvol(1, 255);
    fm_setchannelvol(2, 255);

    fm_setinstrument(BRASS);

    for(i=0;i<8;i++)
        fm_channeloff(i);

    note_count = parse_play_command(play_command, notes);
    
    if (note_count == -1) {
        return -1;
    }
    printf("Parsed %d notes:\n", note_count);

    for (i = 0; i < note_count; i++) {
        
        jiffystart = JIFFYCLOCK;
        
        printf("%c%c%d ", notes[i].name, notes[i].accidental, notes[i].octave);
        
        fm_playfnum(1,notes[i].fnum);

        while(1) {
            jiffynow = JIFFYCLOCK;
            if(jiffynow-jiffystart > notes[i].duration)
                break;
        }

        fm_channeloff(1);
    }

    return 0;

}

void main(void) {

    printf("%cFM Demo\n",0x93);
    printf("Accessing the SFX Sound Expander\n");
    printf("CC65 Library Demo v1.0\n\n");

    play_instruments();
    play_chord();
    play_music();

    

}