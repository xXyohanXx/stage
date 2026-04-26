#ifndef PNJ_H
#define PNJ_H
#include "Entite.h"

#include<string>
#include <type_traits>

//la classe Pnj représente un personnage non-joueur dans le jeu, avec des attributs pour savoir s'il est en danger et quel dialogue il a à offrir au joueur. Elle inclut des méthodes pour parler, ainsi que des accesseurs et mutateurs pour les attributs. elle herite de la classe Entite, ce qui signifie qu'elle peut également avoir des attributs et des méthodes liés à sa position, ses points de vie, etc. Cependant, ces détails ne sont pas visibles dans le code fourni, mais ils sont définis dans la classe Entite dont Pnj hérite. 

class Pnj : public Entite {
    private:
        string num;
        bool enDanger;
        string dialogue;

    public:

        Pnj();
        Pnj(string nume);
        void parler();

        //accesseurs et mutateurs pour enDanger
        void setEnDanger(bool danger);
        bool isEnDanger() const;

        //accesseurs et mutateurs pour dialogue
        void setDialogue(const std::string& d);
        std::string getDialogue() const ;
        void charger_dialogue( int n );
};

#endif