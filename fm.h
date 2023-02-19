#ifndef _FMXPANDERLIB_
#define _FMXPANDERLIB_

#include <stdint.h>

/* Simple Sound Expander FM library 
    Scott Hutter / xlar54
*/

// https://c64.xentax.com/images/programming_the_adlib.txt
// https://c64.xentax.com/index.php/15-testing-ym3812-register-write-timing
// https://www.floodgap.com/retrobits/ckb/secret/sfx.html

#define REG_SELECT  0xDF40
#define DATA_WRITE  0xDF50
#define CHIP_STATUS 0xDF60


//   Channel        1   2   3   4   5   6   7   8   9
//   Operator 1    00  01  02  08  09  0A  10  11  12
//   Operator 2    03  04  05  0B  0C  0D  13  14  15

uint8_t channel_offsets[] = {0x00, 0x01, 0x02, 0x08, 0x09, 0x0a, 0x10, 0x11, 0x12};

uint16_t notetbl[] = {
  //  C     C#      D       D#      E       F       F#      G       G#      A       A#       B
  0x0157, 0x016b, 0x0181, 0x0198, 0x01b0, 0x01ca, 0x01e5, 0x0202, 0x0220, 0x0241, 0x0263, 0x0287, 
  0x0557, 0x056b, 0x0581, 0x0598, 0x05b0, 0x05ca, 0x05e5, 0x0602, 0x0620, 0x0641, 0x0663, 0x0687,
  0x0957, 0x096b, 0x0981, 0x0998, 0x09b0, 0x09ca, 0x09e5, 0x0a02, 0x0a20, 0x0a41, 0x0a63, 0x0a87,
  0x0d57, 0x0d6b, 0x0d81, 0x0d98, 0x0db0, 0x0dca, 0x0de5, 0x0e02, 0x0e20, 0x0e41, 0x0e63, 0x0e87,
  0x1157, 0x116b, 0x1181, 0x1198, 0x11b0, 0x11ca, 0x11e5, 0x1202, 0x1220, 0x1241, 0x1263, 0x1287,
  0x1557, 0x156b, 0x1581, 0x1598, 0x15b0, 0x15ca, 0x15e5, 0x1602, 0x1620, 0x1641, 0x1663, 0x1687,
  0x1957, 0x196b, 0x1981, 0x1998, 0x19b0, 0x19ca, 0x19e5, 0x1a02, 0x1a20, 0x1a41, 0x1a63, 0x1a87,
  0x1d57, 0x1d6b, 0x1d81, 0x1d98, 0x1db0, 0x1dca, 0x1de5, 0x1e02, 0x1e20, 0x1e41, 0x1e63, 0x1e87
};



void fm_init(void) {

    
}


void fm_regwrite(unsigned char value) {

    (*(unsigned char*) (REG_SELECT) = (value));

}

void fm_datawrite(unsigned char value) {

    (*(unsigned char*) (DATA_WRITE) = (value));

}

void fm_setchannelvol(uint8_t channel, uint8_t volume) {

    //Bytes 40-55
    //Level Key Scaling / Total Level
    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // |  Scaling  |             Total Level           |
    // |   Level   | 24    12     6     3    1.5   .75 |

    // operator 1
    fm_regwrite(0x40 + channel_offsets[channel-1]);
    fm_datawrite(volume);

    // operator 2
    fm_regwrite(0x40 + channel_offsets[channel-1] + 3);
    fm_datawrite(volume);
}

void fm_setwaveform(uint8_t channel, uint8_t waveform) {

    // Bytes 20-35
    // Amplitude Modulation / Vibrato / Envelope Generator Type / Keyboard
    // Scaling Rate / Modulator Frequency Multiple
    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // | Amp | Vib | EG  | KSR |  Modulator Frequency  |
    // | Mod |     | Typ |     |       Multiple        |
    // +-----+-----+-----+-----+-----+-----+-----+-----+

    // operator 1
    fm_regwrite(0x01);
    fm_datawrite(0x20);





    // Bytes E0-F5
    // Waveform select
    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // | unused                            | waveform  |
    // |                                   |  select   |
    // +-----+-----+-----+-----+-----+-----+-----+-----+

    // operator 1
    fm_regwrite(0xe0 + channel_offsets[channel-1]);
    fm_datawrite(waveform);

    // operator 2
    fm_regwrite(0xe0 + channel_offsets[channel-1] + 3);
    fm_datawrite(waveform);

}

/***
 *  ADSR values should be in the range of 0 to 15
*/
void fm_setadsr(uint8_t channel, uint8_t attack, uint8_t sustain, uint8_t decay, uint8_t release) {

    uint8_t val1 = (attack << 4) + (decay & 0xf);
    uint8_t val2 = (sustain << 4) + (release & 0xf);

    // Bytes 60-75
    // Attack Rate / Decay Rate

    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // |         Attack        |          Decay        |
    // |          Rate         |          Rate         |
    // +-----+-----+-----+-----+-----+-----+-----+-----+

    // operator 1
    fm_regwrite(0x60 + channel_offsets[channel-1]);
    fm_datawrite(val1);

    // operator 2
    fm_regwrite(0x60 + channel_offsets[channel-1] + 3);
    fm_datawrite(val1);

    // Bytes 80-95
    // Sustain Level / Release Rate
    //
    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // |     Sustain Level     |         Release       |
    // | 24    12     6     3  |          Rate         |
    // +-----+-----+-----+-----+-----+-----+-----+-----+

    // operator 1
    fm_regwrite(0x80 + channel_offsets[channel-1]);
    fm_datawrite(val2);

    // operator 2
    fm_regwrite(0x80 + channel_offsets[channel-1] + 3);
    fm_datawrite(val2);

}

void fm_playsound(uint8_t channel, uint8_t octave, uint8_t noteid) {
    
    uint8_t low = 0;
    uint8_t hi = 0;
    uint16_t fnum = notetbl[(octave * 0x0c) + noteid];
    

    // operator 1
    fm_regwrite(0x20 + channel_offsets[channel-1]);
    fm_datawrite(0x01);

    // operator 2
    fm_regwrite(0x20 + channel_offsets[channel-1] + 3);
    fm_datawrite(0x01);


    // Bytes A0-B8
    // Octave / F-Number / Key-On
    //
    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // |        F-Number (least significant byte)      |  (A0-A8)
    // |                                               |
    // +-----+-----+-----+-----+-----+-----+-----+-----+

    //    7     6     5     4     3     2     1     0
    // +-----+-----+-----+-----+-----+-----+-----+-----+
    // |  Unused   | Key |    Octave       | F-Number  |  (B0-B8)
    // |           | On  |                 | most sig. |
    // +-----+-----+-----+-----+-----+-----+-----+-----+

    low = fnum & 0xff;
    hi = 0x20 | (fnum>>8) & 0xff;

    fm_regwrite(0xa0 + channel_offsets[channel-1]);
    fm_datawrite(low);

    //hi |= (1 << 3); // octave 2
    //hi |= (1 << 5); // key on

    fm_regwrite(0xb0 + channel_offsets[channel-1]);
    fm_datawrite(hi);
}

#endif