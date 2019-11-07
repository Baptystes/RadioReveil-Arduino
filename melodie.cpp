#include "pitches.h"
#include "Arduino.h"



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
Melodie :: Melodie () : noteEnCours(0), tempsAttente(0), duration(0), etat(0), nombreDeNotes(sizeof(melody)/sizeof(int))
{
  
   
}


void Melodie :: jouer()
{
  if (etat)
  {
    
    if (millis()> (duration*1.2 + tempsAttente+160))
    {
      
      noteEnCours++;
      duration = PACE*noteDurations[noteEnCours];
      tempsAttente = millis();
     
    }
    else if (millis()>duration*1.2 + tempsAttente+100)
    {
      
    }
    else if (noteEnCours<nombreDeNotes)
    {
      tone(10, melody[noteEnCours],duration);      
    }
    else
    {
      etat=0;
      noteEnCours=0;
    }
 //   delay (duration*1.2);
  }
}


void Melodie :: initialiser ()
{
  noteEnCours=0;
  duration = PACE*noteDurations[noteEnCours];
  tempsAttente = duration + millis();
  etat=1;
}

