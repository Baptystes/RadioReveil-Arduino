#include "class.h"

#include "header.h"
#include "pitches.h"
class Melodie;
Melodie melodie;
/////////////////////////////////////////////////////////// HORLOGE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Horloge::Horloge():heure(0), minute(0), seconde(0)
{
  
}

void Horloge::addSeconde()
{
  if ((seconde++)>59)
  {
    seconde=0;
    addMinute();
  }
}

void Horloge::resetSeconde()
{
  seconde=0;
}

void Horloge::addMinute(bool modeReglage, byte nbMinutes)
{
    if ((minute+=nbMinutes)>59)
    {
        minute%=60;
        if (modeReglage==0)
            addHeure();
    }
}


void Horloge::addHeure(int nbHeures)
{
    heure=(heure+nbHeures)%24;
}

void Horloge::setTime(byte heureRecu[])
{
  heure = heureRecu[0];
  minute = heureRecu[1];
  seconde = heureRecu[2];
  
}

int Horloge::getTime(int i)
{
  return tabHeure[i];
}

byte Horloge :: getSeconde()
{
  return seconde;
}
byte Horloge :: getMinute()
{
  return minute;
}
byte Horloge :: getHeure()
{
  return heure;
}



void Horloge::calculTab()
{
    tabHeure[3]=minute%10;
    tabHeure[2]=minute/10;
    tabHeure[1]=heure%10;
    tabHeure[0]=heure/10;

}


/////////////////////////////////////////////////////////// REVEIL //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Horloge_Reveille :: Horloge_Reveille(Interrupteur interrupteurTabRecu[]):etat(0), etatSonnerie(0), phase(0)
{
  interrupteurTab = interrupteurTabRecu;
  interrupteurActif[0]=0;
  interrupteurActif[1]=0;
}




Horloge *Horloge_Reveille :: getTempsActivation(byte numeroInterrupteur)
{
  if (numeroInterrupteur==0)
    return &tempsActivation1;
  else
    return &tempsActivation2;
}


bool Horloge_Reveille::compareMinute (byte minuteRecu)
{
  return (minuteRecu==minute);
}

bool Horloge_Reveille::compareHeure (byte heureRecu)
{
  return (heureRecu == heure);
}


void Horloge_Reveille::control (Horloge *heure)
{ 
  
  bool timeIsOk = (compareHeure(heure->getHeure()) && compareMinute(heure->getMinute()));
  if (timeIsOk && phase==0 && etat)
  {
    phase=1;
    Serial.println("ACTIVATION!!!");
    for (int a=0; a<2; a++)
    {
      if (interrupteurActif[a])
        interrupteurTab[a].activer(getTempsActivation(a)->getMinute(), getTempsActivation(a)->getHeure());
    }
    if (etatSonnerie)
      melodie.initialiser();
      
  }
  else if (timeIsOk==0 && phase==1)
    phase=0;
} 




/////////////////////////////////////////////////////////// ECRAN ////////////////////////////////////////////////////////////////////////////////////////////////////////

Ecran::Ecran():clignotementEteint(0), tempsInitiale(0)
{
  
  gndTab[0]=(GND1);
  gndTab[1]=(GND2);
  gndTab[2]=(GND3);
  gndTab[3]=(GND4);

  chiffreCode[0]=0x3F;
  chiffreCode[1]=0x06;
  chiffreCode[2]=0x5B;
  chiffreCode[3]=0x4F;
  chiffreCode[4]=0x66;
  chiffreCode[5]=0x6D;
  chiffreCode[6]=0x7D;
  chiffreCode[7]=0x07;
  chiffreCode[8]=0x7F;
  chiffreCode[9]=0x6F;

  //OFF
  texteTab[0]=0x3F;
  texteTab[1]=0x71;
  texteTab[2]=0x71;
  texteTab[3]=0x00;

  //ON
  texteTab[4]=0x3F;
  texteTab[5]=0x54;
  texteTab[6]=0x00;
  texteTab[7]=0x00;

    //
}

void Ecran::aff (Horloge *timeToPrint)
{
  for (int i=0; i<4; i++)
  {
    digitalWrite(gndTab[i],0);
    
    digitalWrite(verrou, LOW);//!!!!!!!!!  //1 desactivé
    shiftOut(data, clockPin,MSBFIRST, (chiffreCode[timeToPrint->getTime(i)]));//(datapin, clockpin, data)
    //Serial.println(timeToPrint->getTime(i));
    digitalWrite(verrou, HIGH);//!!!!!!!!
    delay(light);
    digitalWrite(gndTab[i], 1);
        
  }
}

void Ecran::affNombre (byte nombre[])
{
  for (int i=0; i<4; i++)
  {
    digitalWrite(gndTab[i],0);
    
    digitalWrite(verrou, LOW);//!!!!!!!!!  //1 desactivé
    shiftOut(data, clockPin,MSBFIRST, (chiffreCode[nombre[i]]));//(datapin, clockpin, data)
    digitalWrite(verrou, HIGH);//!!!!!!!!
    delay(light);
    digitalWrite(gndTab[i], 1);
        
  }
}

void Ecran::affTexte(byte numTexte)
{
  for (int i=0; i<4; i++)
  {
    if (texteTab[numTexte*4 + i]!=0x00)
    {
      digitalWrite(gndTab[i],0);
      digitalWrite(verrou, LOW);//!!!!!!!!!  //1 desactivé
      shiftOut(data, clockPin,MSBFIRST, (texteTab[numTexte*4 + i]));//(datapin, clockpin, data)
      //Serial.println(timeToPrint->getTime(i));
      digitalWrite(verrou, HIGH);//!!!!!!!!
    }
    else
      digitalWrite(gndTab[i], 1);
    delay(light);
    digitalWrite(gndTab[i], 1);
  }  
}





void Ecran::affHeures (Horloge *timeToPrint)
{ 
  for (int i=0; i<2; i++)
  {
   
    if (clignotementEteint==0)
    {
      digitalWrite(gndTab[i],0);
      digitalWrite(verrou, LOW);//!!!!!!!!!
      shiftOut(data, clockPin,MSBFIRST, (chiffreCode[timeToPrint->getTime(i)]));//(datapin, clockpin, data)
      //Serial.println(timeToPrint->getTime(i));
      digitalWrite(verrou, HIGH);//!!!!!!!!
    }
    else
       digitalWrite(gndTab[i],1);
    delay(light);
    digitalWrite(gndTab[i], 1);
  }
  for (int i=2; i<4; i++)
  {
    digitalWrite(gndTab[i],0);
    digitalWrite(verrou, LOW);//!!!!!!!!!
    shiftOut(data, clockPin,MSBFIRST, (chiffreCode[timeToPrint->getTime(i)]));//(datapin, clockpin, data)
    //Serial.println(timeToPrint->getTime(i));
    digitalWrite(verrou, HIGH);//!!!!!!!!
    delay(light);
    digitalWrite(gndTab[i], 1);
  }

  
   if ((millis()-tempsInitiale)>400)
    {
      clignotementEteint = 1-clignotementEteint;
      tempsInitiale=millis();
    }
}


void Ecran::affMinutes (Horloge *timeToPrint)
{ 
  for (int i=0; i<2; i++)
  {
    digitalWrite(gndTab[i],0); 
    digitalWrite(verrou, LOW);//!!!!!!!!!
    shiftOut(data, clockPin,MSBFIRST, (chiffreCode[timeToPrint->getTime(i)]));//(datapin, clockpin, data)
    //Serial.println(timeToPrint->getTime(i));
    digitalWrite(verrou, HIGH);//!!!!!!!!
    delay(light);
    digitalWrite(gndTab[i], 1);
  } 

  for (int i=2; i<4; i++)
  {
    
    if (clignotementEteint==0)
    {
      digitalWrite(gndTab[i],0);
      digitalWrite(verrou, LOW);//!!!!!!!!!
      shiftOut(data, clockPin,MSBFIRST, (chiffreCode[timeToPrint->getTime(i)]));//(datapin, clockpin, data)
      //Serial.println(timeToPrint->getTime(i));
      digitalWrite(verrou, HIGH);//!!!!!!!!
    }
    else if (clignotementEteint==1)
      digitalWrite(gndTab[i],1);
    delay(light);
    digitalWrite(gndTab[i], 1);
  }

   if ((millis()-tempsInitiale)>400)
    {
      clignotementEteint = 1-clignotementEteint;
      tempsInitiale=millis();
    }
}

void Ecran::resetChrono()
{
  tempsInitiale=millis();
  clignotementEteint=0;
}






void Ecran::setLuminosite()
{
  
  luminosite=map(analogRead(A0), 0, 1023, 0, 255);
  //Serial.println(luminosite);
}




/////////////////////////////////////////////////////////// BOUTON ////////////////////////////////////////////////////////////////////////////////////////////////////////


Bouton::Bouton(short pinRecu):tempsInitial(0), appuiImminent(0), relachementImminent(0), etat(0), pin(pinRecu), estOn(0), boutonActif(1), tempsInitialChrono(0)
{
   
}

byte Bouton::testAppuie()
{
  
  int etatDuBouton=digitalRead(pin);//(analogRead(pin)>1000);
  //Serial.print(etatDuBouton);
  if (estOn==0 && etatDuBouton==1)
  {
    //Serial.println("*1");
    estOn=1;
    tempsInitial=millis();
    appuiImminent=true;
    etat=1;
  }
  else if (estOn==1 && etatDuBouton==0)
  {
    //Serial.println("*2");
    appuiImminent=false;
    relachementImminent=true;
    estOn=0;
    etat=7;
  }
  else if (estOn==1 && etatDuBouton==1)
  {
    if (etat==1)
      etat=2;
    if (etat<4 && (millis()-tempsInitial)>1000)
    {
      if (etat==2)
        etat=3;
      else if (etat==3)
        etat=4;
    }
    else if ((millis()-tempsInitial)>2000)
    {
      if (etat==4)
        etat=5;
      else if (etat==5)
        etat=6;
    }
    
    
     
    //Serial.println("*3");
    appuiImminent=false;
    relachementImminent=false;
  }
  else if (etat==7 || relachementImminent)
  {
    relachementImminent=0;
    etat=0;
    //Serial.println("*4");relachementImminent=0;
    //appuiImminent=0;
  }
  //Serial.print(etat);

  if (boutonActif==0 && etat==7)
  {
    boutonActif=1;
    etat=0;
  }
  
  return etat;
}


void Bouton::switchActive()
{
  boutonActif= 1-boutonActif;
}

void Bouton::reset()
{
  boutonActif=0;
}

bool Bouton::chronoDefilementRapide ()
{
  if (millis()-tempsInitialChrono>100)
  {
    tempsInitialChrono = millis();
    return 1;
  }
  else 
    return 0;
}

bool Bouton::chronoDefilementLent ()
{
   if (millis()-tempsInitialChrono>700)
  {
    tempsInitialChrono = millis();
    return 1;
  }
  else 
    return 0; 
}



/////////////////////////////////////////////////////////// INTERRUPTEUR ////////////////////////////////////////////////////////////////////////////////////////////////////////

Interrupteur :: Interrupteur (byte pinRecu, Horloge *pointeurHeure): pin(pinRecu), etat(0), heureEnCours(pointeurHeure), minuteDesactivation(0), secondeDesactivation(0)
{
  //digitalWrite(pin, 1);
}


void Interrupteur :: activer (byte secondeRecu, byte minuteRecu)
{
  if (secondeRecu!=0 || minuteRecu!=0)
  {
    secondeInitiale = heureEnCours->getSeconde();
    minuteInitiale = heureEnCours->getMinute();
    minuteDesactivation=0;
    if ((secondeDesactivation=(secondeInitiale + secondeRecu))>59)
    {
      secondeDesactivation%=60;
      minuteDesactivation++;
      
    }
    minuteDesactivation=(minuteInitiale+minuteRecu)%60;
    etat=1;
  }
  else
    etat=2;
  digitalWrite(pin, 1);
  
}
/*
void Interrupteur :: desactiver()
{
  etat=2;
  digitalWrite(pin, 0);
}
*/
void Interrupteur :: reset()
{
  etat=0;
  digitalWrite(pin, 0);
}

byte Interrupteur :: getEtat ()
{
  return etat;
}



bool Interrupteur :: tester ()
{
  if ((heureEnCours->getSeconde()==secondeDesactivation) && (heureEnCours->getMinute()==minuteDesactivation))
    return 1;
  else
    return 0;
}




