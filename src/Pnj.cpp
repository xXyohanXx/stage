#include "Pnj.h"

#include <fstream>
#include<iostream>
#include <string>
#include <vector>

using namespace std;

Pnj::Pnj() : num("1") ,enDanger(false), dialogue()  {}
Pnj::Pnj(string nume) : enDanger(false) , dialogue(){
    num = nume;
}

void Pnj::parler() {

    
}

void Pnj::setEnDanger(bool danger) {
            enDanger = danger;
        }
bool Pnj::isEnDanger() const {
            return enDanger;
        }
void Pnj::setDialogue(const string& d) {
    dialogue = d;
}

string Pnj::getDialogue() const {
    return dialogue;
}

void Pnj::charger_dialogue( int n ){
    if (num.empty()) {
        dialogue.clear();
        return;
    }

    std::ifstream fichier("data/Dialogue/PNJ" + num  + ".txt");
    if (fichier.is_open()) {
        int c = 0;
        dialogue.clear();
        for (std::string line; std::getline(fichier, line);){
            if (c == n){
            dialogue = line;
            break;
            }
            c += 1; 
        } 
        fichier.close();
        
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier pour le chargement du dialogue." << std::endl;
    }
}


