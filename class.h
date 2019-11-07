#ifndef DEF_HORLOGE
#define DEF_HORLOGE

#include "Arduino.h"
#include "header.h"
typedef struct LampeBouton LampeBouton;



struct LampeBouton
{
  bool etat;
  byte pin;
  long refTemps;
  long tempsActif;
  bool modeParametre;
  byte nombreTab[4];

  int secondesRestantes;
  
 
};


class Bouton;
class Interrupteur;

class Horloge
{
  public:
    Horloge();
    void addTime();
    void addSeconde();
    void addMinute(bool modeReglage=0, byte nombreMinutes=1);
    void addHeure(int nbHeures=1);
    void setTime(byte heureRecu[3]);
    byte getMinute();
    byte getHeure();
    byte getSeconde();

    void setMinute(byte minuteRecu) {minute = minuteRecu;}
    void setSeconde(byte secondeRecu) {seconde = secondeRecu;}
    void setHeure(byte heureRecu) {heure = heureRecu;}

    void resetSeconde();

    void calculTab();
    int getTime(int i);


  protected:
    int heure, minute, seconde;
    byte tabHeure[10];
   



};


class Horloge_Reveille : public Horloge
{
  public:
    Horloge_Reveille(Interrupteur interrupteurTabRecu[2]);
    bool getEtat() {return etat;}
    void switchEtat() {etat = 1-etat;}

    
    //void amorcer (short temps=0);

    bool compareMinute (byte minuteRecu);
    bool compareHeure (byte heureRecu);

    Horloge *getTempsActivation(byte numeroInterrupteur);

    void control (Horloge *heure);

    void switchEtatInterrupteur(byte numInterrupteur) {interrupteurActif[numInterrupteur] = 1 - interrupteurActif[numInterrupteur];}
    bool getEtatInterrupteur(byte numInterrupteur) { return interrupteurActif[numInterrupteur];}   

    bool getEtatSonnerie() {return etatSonnerie;}
    void switchEtatSonnerie() {etatSonnerie = 1-etatSonnerie;}
    

  private:
    bool etat;

    byte phase;
    
    Horloge tempsActivation1;
    Horloge tempsActivation2;
    bool etatSonnerie;
    Interrupteur* interrupteurTab;
    /*Interrupteur *interrupteur1;
    Interrupteur *interrupteur2;*/
    bool interrupteurActif[2];
    


};




class Bouton
{
  public:
    Bouton(short pinRecu);
    byte testAppuie();
    void gestionBouton();
    void switchActive();
    void reset();
    bool chronoDefilementRapide ();
    bool chronoDefilementLent();
    
    
    
  private:
    bool appuiImminent, relachementImminent, estOn;
    byte etat;
    long tempsInitial, tempsEnCours, tempsInitialChrono;
    short pin;
    bool boutonActif;
    //int tempsAppui;
};

class Interrupteur
{
  public:
    Interrupteur (byte pinRecu, Horloge *pointeurHeure);
    void activer (short temps = 0);
    //void desactiver();
    byte getEtat();
    void activer (byte secondeRecu, byte minuteRecu);
    //Horloge *getHorloge();
    void reset();
    
    bool tester ();

  private:
    byte etat;
    byte minuteInitiale, secondeInitiale;
    //short tempsActivation;
    //Horloge tempsActivation;
    byte secondeDesactivation, minuteDesactivation;
    Horloge *heureEnCours;

    bool reveilleActigBuffer;
    
    byte pin;
    
};

class Ecran
{
  public:
    Ecran();
    void aff(Horloge *timeToPrint);
    void affHeures(Horloge *timeToPrint);
    void affMinutes(Horloge *timeToPrint);
    void affHeures();
    void affMinutes();
    void affNombre(byte nombre[4]);
    
    void setLuminosite();
    void resetChrono();

    void affTexte(byte numTexte);
    


  private:
    short luminosite;
    byte gndTab[4], texteTab[8];
    byte chiffreCode[10];
    long clignotementEteint, tempsInitiale;

  
};


#endif

