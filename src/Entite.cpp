#include "Entite.h"

#include<iostream>
#include <vector>

using namespace std;

Entite::Entite() : nom(), positionX(32), positionY(32), pv(20) {}

void Entite::update(){


}

void Entite::setPositionX(int x) {
    positionX = x;
}
int Entite::getPositionX() const {
    return positionX;
}
void Entite::setPositionY(int y) {
    positionY = y;
}
int Entite::getPositionY() const {
    return positionY;
}
void Entite::setPv(int p) {
    pv = p;
}
int Entite::getPv() const {
    return pv;
}

void Entite::setDefense(int d){

        defense = d;
}

int Entite::getDefense() const {

    return defense;
}

void Entite::gauche() {

    if(getPositionX() > 0)
        setPositionX(getPositionX()-32);

}

void Entite::droite() {

    if(getPositionX() < 800 - 32)
        setPositionX(getPositionX()+32);

}

void Entite::haut() {

    if(getPositionY() > 0)
    setPositionY(getPositionY()-32);
}

void Entite::bas(){

    if(getPositionY() < 640 - 32)
        setPositionY(getPositionY()+32);
} 
