/**
 * @file Coffre.h
 * @brief Declaration de la classe Coffre.
 */

#ifndef COFFRE_H
#define COFFRE_H

#include "Objet.h"

#include<vector>

/**
 * @class Coffre
 * @brief Conteneur d'objets interactif present dans une zone.
 */
class Coffre
{
private:
    bool ouvert;
    std::vector<Objet> contenu;
    int positionX;
    int positionY;

public:
    /** @brief Cree un coffre vide ferme a la position (0,0). */
    Coffre();

    /** @brief Cree un coffre ferme a une position donnee. */
    Coffre(int x, int y);

    /** @brief Ouvre le coffre pour permettre la recuperation du contenu. */
    void ouvrir();

    /** @brief Indique si le coffre est deja ouvert. */
    bool estOuvert() const;

    int getPositionX()const;
    int getPositionY()const;
    void setPosition(int x, int y);
    
    std::vector<Objet>& getContenu();
    const std::vector<Objet>& getContenu() const;
    void ajouterObjet(const Objet& objet);
};

#endif