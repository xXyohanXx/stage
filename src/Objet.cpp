#include "Objet.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

Objet::Objet() {

    nom ="objectName";
    type ="typeName";
    valeurs = {};
    stats = {};

}

Objet::Objet(const std::string& nom, const std::string& type, const std::vector<std::string>& stats, const std::vector<int>& valeurs) : nom(nom), type(type), stats(stats), valeurs(valeurs) {}

std::string Objet::getNom() const {
    return nom;
}

std::string Objet::getType() const {
    return type;
}

std::vector<int> Objet::getValeurs() const {
    return valeurs;
}

std::vector<std::string> Objet::getStats() const {
    return stats;
}
void Objet::appliquer() {}

void Objet::charger_objet(std::string num){
    std::ifstream fichier("data/Objet/Objet"+ num +".txt");
    if (fichier.is_open()) {
        int c = 0;
        int n = 0;
        for (std::string line; std::getline(fichier, line);){
            if (c== 0){
                nom = line;
            }
            else if (c == 1) {
                type = line;
            }
            else if (c == 2) {
                if( line == "1" ) n =1;
                else if (line == "2") n = 2;
                else if (line == "3") n = 3;
                
                
            }
            else if (n > 0 ){
                stats.push_back(line);
            }
            else {
                stats.push_back(line);
            }
            c += 1; 
        }
        fichier.close();
        
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier pour le chargement de la zone." << std::endl;
    }

}

