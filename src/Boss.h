/**
 * @file Boss.h
 * @brief Declaration de l'entite Boss.
 */

#ifndef BOSS_H
#define BOSS_H

#include "Ennemi.h"

#include<string>

/**
 * @class Boss
 * @brief Specialisation d'Ennemi pour le boss principal.
 */
class Boss: public Ennemi {

    private: 

        int pvBonus;
        std::string attaqueSpeciale;

    public:

        /** @brief Construit un boss avec ses attributs specifiques. */
        Boss();

        /** @brief Execute une attaque speciale du boss. */
        void attaquerSpecial(Attaque attS);




};




#endif