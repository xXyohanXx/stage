#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include "Objet.h"

#include <string>
#include <vector>

class Inventaire {
private:
    std::vector<Objet> objets;

public:
    Inventaire();

    void ajouterObjet(const Objet& objet);
    bool supprimerObjet(int index);

    int getNombreObjets() const;
    const std::vector<Objet>& getObjets() const;

};

#endif
