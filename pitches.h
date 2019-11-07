#define PACE 23

#define NO 0
#define DO  262
#define RE  294
#define MI  330
#define FA  349
#define SOL  392
#define LA  440
#define SI  485
#define DO2  525
#define RE2  587
#define MI2  659
#define FA2  698


class Melodie
{
  public:
    void jouer ();
    void initialiser();
    Melodie();
  
  private:
    
    
    bool etat;
    long tempsAttente;
    short noteEnCours;
    int duration, nombreDeNotes;
    
};








