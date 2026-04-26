#ifndef ATTAQUE_H
#define ATTAQUE_H

#include <string>
using namespace std;
/**
 * @struct Attaque
 * @brief Représente une attaque dans le jeu
 * @details Cette structure contient le nom de l'attaque et les dégâts associés à cette attaque
 * - nom : le nom de l'attaque
 * - deg : les dégâts que l'attaque inflige
 */
struct Attaque{
    
    string nom;
    int time ;
    int deg;
    int PosX;
    int PosY;
    
    /**
     * @brief Attaque : Constructeur de la structure Attaque
     * @param nom : le nom de l'attaque
     * @param deg : les dégâts que l'attaque inflige
     */
    Attaque(string nom , int deg , int PosX , int PosY) ;
};

#endif