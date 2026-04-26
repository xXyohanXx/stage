#include "attaque.h"

//Constructeur de la structure Attaque
Attaque::Attaque(string nom_p , int deg_p , int PosX_p , int PosY_p ) {
    deg = deg_p;
    nom = nom_p;
    time = 0 ;
    PosX = PosX_p;
    PosY = PosY_p;
}