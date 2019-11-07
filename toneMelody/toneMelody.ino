#include "pitches.h"

//array of notes
int melody[] = {
  NO, DO, RE, FA, NO, LA, SOL, LA, SOL, FA, NO,    FA, SOL, FA, SOL, FA, SOL, FA, RE,  NO,  DO, FA, FA, LA, RE2, DO2, SI, LA, SOL, 
  SOL, DO2, RE2, DO2, DO2, RE2, DO2, RE2, DO2, LA, NO, FA, SOL, FA, SOL, FA, RE2, FA, SOL, 
  LA, DO2, LA, DO2, LA, FA, RE, DO, FA/*, RE, FA ,FA, MI, RE, DO, DO, DO, SOL, FA, MI, FA,
  FA, FA, MI, RE, DO, SOL, SOL, SOL, FA, SOL, LA, LA, LA, DO, DO2,
  RE2, RE2, RE2, SI, LA, SOL, LA, SI, DO2, RE2, DO2, LA, SOL, FA, LA, RE, RE, RE, LA, RE, RE, RE, SI, SI, SI, SI, LA, SOL, FA, MI*/
  
};

int noteDurations[] = {
  2, 2, 2, 2,  2, 4, 2,  2, 1, 1,2, 2,  2, 2, 2, 2,  2, 1, 1, 2, 2,  4, 2, 2,  2, 2, 2, 2, 8,
  2, 2, 2, 2,  2, 4, 2,  2, 1, 1, 2, 2,  2, 2, 2, 2,  2, 4, 2,
  2, 2, 2, 2,  2, 2, 2, 2,  4, 4/* 2, 2, 2, 2,  4, 4,  2, 2, 2, 2,  8,
  2, 2, 2, 2,  4, 4,  2, 2, 2, 2,  8,  2, 2, 2, 2,
  2, 2, 2, 2,  2, 2, 2, 2,  2, 2, 2, 2,  6, 2,  2, 2, 2, 2,  2, 2, 2, 2,  2, 2, 4,  2, 2, 2, 2, 6*/
};
int pace = 100; 
void setup()
{
  // change pace of music("speedy")
}
void loop() {
  for (int Note = 0; Note <sizeof(melody)/sizeof(int); Note++) {//counter of Notes (54 limit the array)
    int duration = pace*noteDurations[Note];//Adjust duration with the pace of music
    tone(10, melody[Note],duration); //Play note

// to distinguish the notes, set a minimum time between them.
    delay(duration*1.2);
  }
}

