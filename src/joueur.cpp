#include "joueur.h"

#include<iostream>
using namespace std;

Joueur::Joueur() {
    attaque = 10;
    defense = 0;
    orientation = 'd';

}
void Joueur::SetOrientation(char c){
    orientation = c;
}

char Joueur::GetOrientation(){
    return orientation;
}

void Joueur::attaquer(Entite& cible){
    int nouveauxPv = cible.getPv() - attaque;
    if (nouveauxPv < 0) {
        nouveauxPv = 0;
    }
    cible.setPv(nouveauxPv);
}

/*void Joueur::esquiver(){

}*/

void Joueur::utiliser_objet(){
    int stats;



}

void Joueur::setNom(string name) {
    nom = name;
}

void Joueur::setAttaque(int a) {
    attaque = a;
}   

int Joueur::getAttaque() const {
    return attaque;
}

void Joueur::setDefense(int d) {
    defense = d;
}

int Joueur::getDefense() const {
    return defense;
}
