#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <iostream>
#include <fstream>
#include <string>

class ContexteJeu; // Déclaration anticipée de la classe ContexteJeu pour éviter les dépendances circulaires 

class Sauvegarde {
public:

    void sauvegarderPartie(const std::string& nom_fichier, ContexteJeu& contexte);
    void chargerPartie(const std::string& nom_fichier, ContexteJeu& contexte);
};

#endif // SAUVEGARDE_H