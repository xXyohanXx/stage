#include "Ennemi.h"
#include<iostream>
using namespace std;
//Cosntructeur par defaut qui va initialiser les dégats à 10 pts .
Ennemi::Ennemi(){
    degats=10;
    cooldownAttaque = 0;
    type = 'S';
}

//Constructeur avec Parametre :
Ennemi::Ennemi(int D){
    degats = D;
    cooldownAttaque = 0;
}


/*//Methode attaquer 
void Ennemi::attaquer(Attaque att){
    cout << "L'ennemi attaque avec " << att.nom << " et inflige " << att.deg << " points de dégats." << endl;
}*/
char Ennemi::getType() const{
    return type; 
}


void Ennemi::attaquer(Joueur& joueur){
    int nouveauxPv = joueur.getPv() - degats;
    if (nouveauxPv < 0) {
        nouveauxPv = 0;
    }
    joueur.setPv(nouveauxPv);
    cooldownAttaque = 8;
}

bool Ennemi::peutAttaquer() const {
    return cooldownAttaque == 0;
}

void Ennemi::reduireCooldown() {
    if (cooldownAttaque > 0) {
        cooldownAttaque -= 1;
    }
}

//Methode esquiver
void Ennemi::esquiver(){
    cout << "L'ennemi esquive l'attaque du joueur." << endl;
}

void Ennemi::setType(char c){
    type = c;
}