#include "Inventaire.h"

using namespace std;

Inventaire::Inventaire() {}

void Inventaire::ajouterObjet(const Objet& objet) {
    objets.push_back(objet);
}

bool Inventaire::supprimerObjet(int index) {
    if (index < 0 || index >= objets.size()) {
        return false;
    }

    objets.erase(objets.begin() + index);
    return true;
}

int Inventaire::getNombreObjets() const {
    return objets.size();
}

const vector<Objet>& Inventaire::getObjets() const {
    return objets;
}
