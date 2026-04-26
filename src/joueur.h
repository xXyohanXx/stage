#ifndef Joueur_H
#define Joueur_H 

#include "Entite.h"
#include "attaque.h"

#include <string>
using namespace std;

class Joueur : public Entite {
    private :
        int attaque ;
        int defense ;
        char orientation;
    public:

    Joueur();

    void attaquer(Entite& cible);

    //void esquiver() ; 

    void utiliser_objet();
    void SetOrientation(char c);
    char GetOrientation();
    void setNom(string name);

    //accesseurs et mutateurs pour attaque
    void setAttaque(int a);
    int getAttaque() const;
    //accesseurs et mutateurs pour defense
    void setDefense(int d);
    int getDefense() const;
    };

#endif 
