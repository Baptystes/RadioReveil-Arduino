#include "pitches.h"
#include "class.h"
#include "header.h"


long refTemps=1000;
byte mode(0), reglage(0), phaseReglage(0);

    

long refTempsLed=500;
bool ledEtat=0;
Horloge heure;
Ecran ecran;

bool ledEtatMode0(0);
long refTempsLedMode0(0);

bool buf=0;

LampeBouton lampeBouton = {0, LED, 0, 0,0};



Bouton bouton_D(BTN_D);
Bouton bouton_G(BTN_G);

Interrupteur interrupteurTab[]={Interrupteur (INTERRUPTEUR_1, &heure), Interrupteur (INTERRUPTEUR_2, &heure)};
Horloge_Reveille reveilleTab[]={Horloge_Reveille(interrupteurTab), Horloge_Reveille(interrupteurTab)}; //(&interrupteurTab[0],&interrupteurTab[1]), Horloge_Reveille(&interrupteurTab[0],&interrupteurTab[1])};

//Interrupteur interrupteur_1(INTERRUPTEUR_1, &heure);

extern Melodie melodie;

int testAppuie_D(0), testAppuie_G(0);


void setup() {
lampeBouton.secondesRestantes=0;
  
    Wire.begin();
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    bcdToDec(Wire.read() & 0x7f);
    bcdToDec(Wire.read());
    bcdToDec(Wire.read() & 0x3f);
    bcdToDec(Wire.read());
    bcdToDec(Wire.read());
    bcdToDec(Wire.read());
    if (bcdToDec(Wire.read())==0)
    {
      setDS3231time(0,0,0,0,0,0,1);
    }
   
   
   
   /*reveille.calculTab();
   reveille.addMinute();
   reveille.calculTab();*/
    
    lampeBouton.nombreTab[0]=0;
    lampeBouton.nombreTab[1]=0;
    lampeBouton.nombreTab[2]=0;
    lampeBouton.nombreTab[3]=0;
    lampeBouton.tempsActif = 10;
   
   pinMode(GND1,OUTPUT);
   pinMode(GND2,OUTPUT);
   pinMode(GND3,OUTPUT);
   pinMode(GND4,OUTPUT);

   pinMode(LED, OUTPUT);
   pinMode(INTERRUPTEUR_1, OUTPUT);
   pinMode(INTERRUPTEUR_2, OUTPUT);
   //digitalWrite(INTERRUPTEUR_1, 0);

       //On met les broches en sortie
   pinMode(verrou, OUTPUT);
   pinMode(clockPin, OUTPUT);
   pinMode(data, OUTPUT);

   //pinMode(BTN_D, INPUT);
   //pinMode(BTN_G, INPUT);
   

   digitalWrite( GND1, 1);
   digitalWrite( GND2, 1);
   digitalWrite( GND3, 1);
   digitalWrite( GND4, 1);
 
   Serial.begin(9600);
   const char tempo[]=__TIME__;
    int ano, mes, dia, hora, minu, seg;
    sscanf(tempo, "%d:%d:%d", &hora, &minu, &seg);
   /*
   for (int a=0; a<hora; a++)
   {
    heure.addHeure();
   }

   for (int a=0; a<minu; a++)
   {
      heure.addMinute(1);
      Serial.println(heure.getMinute());
   }*/

}
 


void loop()
{

        
         melodie.jouer();

        //Serial.println(__TIME__);
        
        //Serial.println(A1);
        
        
        if (millis()>= refTemps)
        {
          //Serial.println(heure.getSeconde());
          if (reglage==0 || mode==1)
          {
            getTimeModule(&heure);
            //heure.addSeconde();            
            heure.calculTab();
           

            for (int a=0; a<2; a++)
            {
              reveilleTab[a].control(&heure);
              if (interrupteurTab[a].getEtat()==1)
              {
                   if (interrupteurTab[a].tester() )
                   {
                    interrupteurTab[a].reset();
                    //interrupteurTab[a].desactiver();
                   }
              }
            }
            
            
          }

          if (lampeBouton.etat==1)
          {
            if ((lampeBouton.secondesRestantes--)<2)
            {
              
              lampeBouton.etat=0;
              digitalWrite(lampeBouton.pin, 0);
            }
          }

          
          refTemps+=1000;
        }

        Serial.println(lampeBouton.secondesRestantes);
        //Serial.println(lampeBouton.secondesRestantes);
        //Serial.print(digitalRead(BTN_D));
        /*if (digitalRead(BTN_D)==1)
        {
          heure.addMinute();
          heure.calculTab();
        }*/

        //interrupteur_1.tester();
        testAppuie_D = bouton_D.testAppuie();
        testAppuie_G = bouton_G.testAppuie();
        
        if (testAppuie_G ==7 && reglage==0 && lampeBouton.modeParametre==false)
        {
          mode=(mode+1)%3;
          digitalWrite(LED, 0);
          
          if (lampeBouton.etat==1 && mode==0 && lampeBouton.pin==LED)
            digitalWrite(LED, 1);
          else if (mode>0)
          {
            ledEtat=1;
            refTempsLed=millis();
          }
        }
     

        
        if (mode==0)
        {        
          
          if (testAppuie_G==6 && reglage==0)
          {
            reglage=1; phaseReglage=1;
            bouton_G.reset();
            ecran.resetChrono();
          }
          else if (reglage==1)
          {
            reglageHorloge(&heure, 1);
            if (phaseReglage==3)
            {
              phaseReglage=0;
              reglage=0;
              heure.resetSeconde();
              setTimeModule (&heure);
            }
          }
          else if (testAppuie_D==3 && (interrupteurTab[0].getEtat() || interrupteurTab[1].getEtat()))
          {
            
              interrupteurTab[0].reset();
              interrupteurTab[1].reset();
              bouton_D.reset();
          }
          
          else if (testAppuie_D==7)
          {
            //melodie.initialiser();
            if (lampeBouton.modeParametre==true)
            {
              lampeBouton.modeParametre=false;
              //lampeBouton.tempsActif = (lampeBouton.nombreTab[1]*60 + lampeBouton.nombreTab[2]*10);
              lampeBouton.tempsActif = lampeBouton.nombreTab[1]*60 + lampeBouton.nombreTab[2]*10;
              Serial.print(":");Serial.println(lampeBouton.secondesRestantes);
              lampeBouton.nombreTab[1]=0;
              lampeBouton.nombreTab[2]=0;
              //Serial.println("BLOP");
            }
            else
            {
              if (lampeBouton.etat)
              {
                lampeBouton.secondesRestantes = 0;
                
                buf=1;
              }
              
              for (int a=0; a<2; a++)
              {
                if (interrupteurTab[a].getEtat()==2)
                {
                  interrupteurTab[a].reset();
                  buf=1;
                }
              }
              if (buf==0)
              {
                lampeBouton.etat=1;
                lampeBouton.secondesRestantes = lampeBouton.tempsActif;
                //lampeBouton.refTemps = millis()+(lampeBouton.tempsActif)*1000;
                //Serial.println(lampeBouton.tempsActif);
                digitalWrite(lampeBouton.pin, 1);
              }
              else
                buf=0;
              //Serial.println("OkAY");
              
            }
            
            
          }
          else if (testAppuie_D >=6)
          {
            
            if (testAppuie_G==7)
            {
              lampeBouton.nombreTab[2]=1;
              lampeBouton.nombreTab[1]=0;
            }
            
            lampeBouton.modeParametre=true;
            ecran.affNombre(lampeBouton.nombreTab);
            if (bouton_D.chronoDefilementLent())
            {
              lampeBouton.nombreTab[2]++;
              if (lampeBouton.nombreTab[2]>5)
              {
                lampeBouton.nombreTab[2]=0;
                lampeBouton.nombreTab[1]++;
                if (lampeBouton.nombreTab[1]>9)
                  lampeBouton.nombreTab[1]=0;
              }
              //lampeBouton.refTemps = 100*lampeBouton.nombreTab[2] + 10*lampeBouton.nombreTab[3];
            }
          }
          else
           ecran.aff(&heure);
          
        }
        else if (mode>0)
        {
          if (millis()>refTempsLed)
          {
            refTempsLed+=(500-reveilleTab[mode-1].getEtat()*400);
            ledEtat=1-ledEtat;
            digitalWrite(LED, ledEtat);
          }
          
          if (testAppuie_G==6 && reglage==0)
          {
            reglage=1; phaseReglage=1;
            bouton_G.reset();
            ecran.resetChrono();
          }
          else if (testAppuie_D==7 && reglage==0)
          {
            reveilleTab[mode-1].switchEtat();
          }
          else if (reglage==1)
          {
            
            if (phaseReglage<3)
              reglageHorloge(&reveilleTab[mode-1], 1);
            else if (phaseReglage==3)
            {
              ecran.affTexte(reveilleTab[mode-1].getEtatInterrupteur(0));
              if (testAppuie_D==7)
                reveilleTab[mode-1].switchEtatInterrupteur(0);
              else if (testAppuie_G==7)
              {
                //reveilleTab[mode-1].setHeure(heure.getHeure());
                //reveilleTab[mode-1].setMinute(heure.getMinute()+mode);
                //reveilleTab[mode-1].calculTab();
                
                phaseReglage++;
                
              }
            }
            else if (phaseReglage<6 && reveilleTab[mode-1].getEtatInterrupteur(0))
            {
              reglageHorloge(reveilleTab[mode-1].getTempsActivation(0), 4);
            }
            else if (phaseReglage<6 && reveilleTab[mode-1].getEtatInterrupteur(0)==0)
            {
              phaseReglage=6;
            }

            else if (phaseReglage==6)
            {
              ecran.affTexte(reveilleTab[mode-1].getEtatInterrupteur(1));
              if (testAppuie_D==7)
                reveilleTab[mode-1].switchEtatInterrupteur(1);
              else if (testAppuie_G==7)
              {                
                phaseReglage++;
              }
            }
            else if (phaseReglage<9 && reveilleTab[mode-1].getEtatInterrupteur(1))
            {
              reglageHorloge(reveilleTab[mode-1].getTempsActivation(1), 7);
            }
            else if (phaseReglage<9 && reveilleTab[mode-1].getEtatInterrupteur(1)==0)
            {
              phaseReglage=9;
            }
            else if (phaseReglage==9)
            {
              ecran.affTexte(reveilleTab[mode-1].getEtatSonnerie());
              if (testAppuie_D==7)
                reveilleTab[mode-1].switchEtatSonnerie();
              else if (testAppuie_G==7)
              {                
                phaseReglage++;
              }
            }
            else
            {
              phaseReglage=0;
              reglage=0;
              
            }
          }
          else
            ecran.aff(&reveilleTab[mode-1]);
        }
          
/*        if (digitalRead(BTN_G)==1 && !(millis()%50))
        {
          //heure.addHeure();
        }*/
        
}      

void reglageHorloge(Horloge *horloge, byte phaseReglageHorloge)
{
            if ((phaseReglage - phaseReglageHorloge)==0)
            {
              ecran.affHeures(horloge);
              //Serial.println(testAppuie_D);
              if (testAppuie_D==7)
              {
                horloge->addHeure();
                horloge->calculTab();
                ecran.resetChrono();
              }
              else if (testAppuie_G==7)
              {
                phaseReglage++;
                ecran.resetChrono();
              }
              else if ((testAppuie_D >=4) && bouton_D.chronoDefilementRapide())
              {
                horloge->addHeure();
                horloge->calculTab();
                ecran.resetChrono();
              }
            }
            else if ((phaseReglage - phaseReglageHorloge)==1)
            {
              ecran.affMinutes(horloge);
              //Serial.println(testAppuie_D);
              if (testAppuie_D==7)
              {
                
                horloge->addMinute(1);
                horloge->calculTab();
                ecran.resetChrono();
              }
              else if (testAppuie_G==7)
              {
                phaseReglage++;
                //reglage=0;
                ecran.resetChrono();
                
              }
              else if ((testAppuie_D >=4) && bouton_D.chronoDefilementRapide())
              {
                horloge->addMinute(1);
                horloge->calculTab();
                ecran.resetChrono();
              }
            }
            
  
}




