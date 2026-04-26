#ifndef ENNEMI_H
#define ENNEMI_H
#include <string>
#include "Entite.h"
#include "attaque.h"
#include "joueur.h"
using namespace std ;

/**
 * @class Ennemi
 * @brief Ennemi qui hérite de la classe Entite
 * @details Cette classe représente les ennemis que le joueur peut rencontrer dans le jeu.
 * - degats : les dégâts que l'ennemi peut infliger au joueur
 * 
 */
class Ennemi : public Entite {
    
    private :

        int degats;
        int cooldownAttaque; // Temps de recharge entre les attaques de l'ennemi
        char type;

    public : 
        /**
         * @brief Constructeur par defaut de la classe Ennemi
         * @param degats : les dégâts sont initialisés a 10pts.
         */
        Ennemi();
        /**
         * @brief Constructeur avec paramètre.
         * @param degats : le degat est recupéré en entrée parametre.
         * 
         */
        Ennemi(int degats);

        
        
        /**
         * @brief methode attaquer
         * 
         * @param : att est un objet de type Attaque qui contient le nom de l'attaque et les dégâts associés à cette attaque.
         * 
         */
        //void attaquer(Attaque att);
        void attaquer(Joueur& joueur);
        
        /**
         * @brief methode esquiver
         */
        void esquiver( );

        void reduireCooldown();
        bool peutAttaquer() const;
        char getType() const;
        void setType(char c);


        
       

};


#endif
