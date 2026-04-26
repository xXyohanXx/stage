#ifndef JEU_TEXT
#define JEU_TEXT

#include "ContexteJeu.h"
#include "winTxt.h"

#include <string>
#include <iostream>
using namespace std ;

class Jeu_text{
    public :
        ContexteJeu contexte;

        void commencer();
        void afficher_zone(int n);
        void txtBoucle();
        void txtAff(WinTXT &win );

        string afficher_inventaire();


};
#endif
