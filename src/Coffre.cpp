#include "Coffre.h"
#include<iostream>

using namespace std;

Coffre::Coffre():ouvert(false), positionX(0), positionY(0) {}
Coffre::Coffre(int x, int y):ouvert(false), positionX(x), positionY(y) {}

void Coffre::ouvrir(){

    ouvert = true;
}

bool Coffre::estOuvert() const {
    return ouvert;
}

int Coffre::getPositionX()const{
    return positionX;
}

int Coffre::getPositionY()const {
    return positionY;
}

void Coffre::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

std::vector<Objet>& Coffre::getContenu() {
    return contenu;
}

const std::vector<Objet>& Coffre::getContenu() const {
    return contenu;
}

void Coffre::ajouterObjet(const Objet& objet) {
    contenu.push_back(objet);
}
