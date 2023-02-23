#ifndef _FMXPANDER_PARSE_LIB_
#define _FMXPANDER_PARSE_LIB_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fm.h"

#define MAX_NOTES 255

typedef struct {
    char name;
    int8_t pitch;
    uint8_t octave;
    char accidental;
    int8_t accidentalVal;
    uint8_t duration;
    uint16_t fnum;
} Note;

int8_t parse_pitch(char pitch_char) {
    switch (pitch_char) {
        case 'C': return 0;
        case 'D': return 2;
        case 'E': return 4;
        case 'F': return 5;
        case 'G': return 7;
        case 'A': return 9;
        case 'B': return 11;
        case 'R': return 12; // rest
        default: return -1;
    }
}

uint8_t parse_octave(char octave_char, uint8_t edge) {
    return (octave_char - '0') + edge;
}

int8_t parse_accidental(char accidental_char) {
    switch (accidental_char) {
        case '#': return 1;
        case 'b': return -1;
        default: return 0;
    }
}

uint8_t parse_duration(char duration_char) {
    switch (duration_char) {
        case 'w': return 240;
        case 'h': return 120;
        case 'q': return 60;
        case 'e': return 30;
        case 's': return 15;
        default: return 60;
    }
}

uint8_t parse_note(char* note_str, Note *note) {
    uint8_t len, i, incOctave;

    note->name = 'x';
    note->pitch = 0;
    note->accidental = ' ';
    note->octave = 255;
    note->duration = 0;
    note->fnum = 0;
    note->accidentalVal=0;
    
    len = strlen(note_str);
    i = 0;
    incOctave=0;

    // parse pitch
    note->pitch = parse_pitch(note_str[i]);
    if (note->pitch == -1) return -1;
    note->name = note_str[i];
    i++;

    // parse optional accidental
    note->accidentalVal = parse_accidental(note_str[i]);
    if (note->accidentalVal != 0) {
        if(note->accidentalVal==1)
            if(note->name != 'B')
                note->accidental='#';
            else {
                note->name='C';
                note->pitch = parse_pitch((char)note->name);
                note->accidental=' ';
                note->accidentalVal=0;
                incOctave=1;
            }           

        if(note->accidentalVal==-1) 
            if(note->name != 'C')
                note->accidental='b';
            else {
                note->name='B';
                note->pitch = parse_pitch((char)note->name);
                note->accidental=' ';
                note->accidentalVal=0;
                incOctave=-1;
            }
        i++;
    } 

    // parse optional octave
    note->octave = -1;
    if (i < len && isdigit(note_str[i])) {
        note->octave = parse_octave(note_str[i], incOctave);
        i++;
    }

    if (note->octave == 255) {
        // if no octave specified, use default octave 4
        note->octave = 4+incOctave;
    }

    // calculate final ym3812 fnum
    note->fnum = notetbl[(note->pitch + note->accidentalVal) + (note->octave * 0x0c)];

    // parse duration
    note->duration = parse_duration(note_str[i]);

    return 0;
}

uint8_t parse_play_command(char* play_command, Note notes[MAX_NOTES]) {
    uint8_t note_count = 0;
    Note note;

    char* token = strtok(play_command, ",");
    while (token != NULL && note_count < MAX_NOTES) {
        parse_note(token, &note);
        if (note.pitch == -1) {
            printf("Error parsing note: %s\n", token);
            return -1;
        }
        notes[note_count] = note;
        note_count++;
        token = strtok(NULL, ",");
    }
    return note_count;
}

#endif