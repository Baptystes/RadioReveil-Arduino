#ifndef DEF_ECRAN
#define DEF_ECRAN
#include "class.h"
#include "Wire.h"

#define DS3231_I2C_ADDRESS 0x68

#define verrou 12 //Broche connectée au SH_CP du 74HC595
#define clockPin 13 //Broche connectée au DS du 74HC595
#define data 8


#define GND1 3
#define GND2 5
#define GND3 6
#define GND4 9

#define BTN_D 14
#define BTN_G 15

#define LED 7
#define INTERRUPTEUR_1 4
#define INTERRUPTEUR_2 2


#define light 5

//#define POT1 0



//void aff(Horloge *timeToPrint);
class Horloge;

void setTimeModule(Horloge *heure);
void getTimeModule(Horloge *heure);
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year);
void displayTime();
byte decToBcd(byte val);
byte bcdToDec(byte val);




#endif
