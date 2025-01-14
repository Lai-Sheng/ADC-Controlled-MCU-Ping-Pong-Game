#include "Note_Freq.h"

#define NOTE_DURATION 250

// Jingle Bells
uint16_t game_over_song_tone[] = {
                                    G3, E4, D4, C4, G3,
                                    G3, E4, D4, C4, A3,
                                    A3, F4, E4, D4, B3,
                                    G4, G4, F4, D4, E4,
                                    G3, E4, D4, C4, G3,
                                    G3, E4, D4, C4, A3,
                                    A3, F4, E4, D4, G4, G4, G4, G4,
                                    A4, G4, F4, D4, C4, G4,
                                    E4, E4, E4, E4, E4, E4,
                                    E4, G4, C4, D4, E4,
                                    F4, F4, F4, F4, F4, E4, E4, E4,
                                    E4, D4, D4, E4, D4, G4,
                                    E4, E4, E4, E4, E4, E4,
                                    E4, G4, C4, D4, E4,
                                    F4, F4, F4, F4, F4, E4, E4, E4,
                                    G4, G4, F4, D4, C4,
                                };
uint16_t game_over_song_beat[] = {
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, 2 * NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION,
                                    NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION, NOTE_DURATION,
                                };

uint16_t wall_sound_t = 16;
uint16_t wall_sound_freq = 226;

uint16_t paddle_sound_t = 96;
uint16_t paddle_sound_freq = 459;

uint16_t point_sound_t = 257;
uint16_t point_sound_freq = 490;
