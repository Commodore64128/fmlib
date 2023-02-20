#include <stdio.h>
#include <unistd.h>
#include "fm.h"

void play_instruments() {

    // set volume
    fm_setchannelvol(1, 0x00);
    fm_setchannelvol(2, 0x00);
    fm_setchannelvol(3, 0x00);

    printf(" Piano...\n");
    fm_setinstrument(PIANO);
    fm_playsound(1, 4, 0);
    sleep(3);

    printf(" Brass...\n");
    fm_setinstrument(BRASS);
    fm_playsound(1, 4, 0);
    sleep(3);

    printf(" Guitar...\n");
    fm_setinstrument(GUITAR);
    fm_playsound(1, 4, 0);
    sleep(3);

}

void play_chord() {

    fm_setchannelvol(1, 0x00);
    fm_setchannelvol(2, 0x00);
    fm_setchannelvol(3, 0x00);

    fm_setinstrument(PIANO);

    printf("\nMajor C Chord\n");
    fm_playsound(1, 4, 0);
    fm_playsound(2, 4, 4);
    fm_playsound(3, 3, 7);
}

void main(void) {

    printf("%cFM Demo\n",0x93);
    printf("Accessing the SFX Sound Expander\n");
    printf("CC65 Library Demo v1.0\n\n");

    play_instruments();
    play_chord();

    

}