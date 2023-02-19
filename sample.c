#include <stdio.h>
#include "fm.h"

void main(void) {

    printf("FM Demo");

    
    fm_setchannelvol(1, 0x00);
    fm_setchannelvol(2, 0x00);
    fm_setchannelvol(3, 0x00);

    fm_setwaveform(1, 1);
    fm_setwaveform(2, 1);
    fm_setwaveform(3, 1);

    fm_setadsr(1, 13,9,4,13);
    fm_setadsr(2, 13,9,4,13);
    fm_setadsr(3, 13,9,4,13);

    fm_playsound(1, 4, 0);
    fm_playsound(2, 4, 4);
    fm_playsound(3, 3, 7);

}