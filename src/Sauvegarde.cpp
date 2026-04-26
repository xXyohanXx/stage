#include "Sauvegarde.h"
#include "ContexteJeu.h"
#include "Niveau.h"
#include "Zone.h"
#include "Ennemi.h"

#include <cstdlib>

void Sauvegarde::sauvegarderPartie(const std::string& nom_fichier, ContexteJeu& contexte) {
    std::ofstream fichier(nom_fichier);
    if (!fichier.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier pour la sauvegarde de la partie." << std::endl;
        return;
    }

    Joueur& joueur = contexte.recupereJoueur();

    fichier << joueur.nom << std::endl;
    fichier << joueur.getPositionX() << " " << joueur.getPositionY() << std::endl;
    fichier << joueur.getPv() << std::endl;
    fichier << joueur.getAttaque() << std::endl;
    fichier << joueur.getDefense() << std::endl;
    fichier << joueur.GetOrientation() << std::endl;
    fichier << contexte.get_Zone_Number() << std::endl;
    fichier << contexte.get_Actual_Level() << std::endl;
    fichier << contexte.getScore() << std::endl;

    Niveau& niveau = contexte.recupereNiveau(contexte.get_Actual_Level());
    int nbZones = niveau.get_number_of_zone_in_the_level();
    fichier << nbZones << std::endl;

    for (int i = 0; i < nbZones; ++i) {
        Zone& zone = niveau.getZone(i);

        // Format explicite pour fiabiliser le chargement des zones.
        fichier << "terrain=" << zone.getNumTerrain() << std::endl;

        fichier << zone.get_Zone_Gauche_Number() << " "
                << zone.get_Zone_Droite_Number() << " "
                << zone.get_Zone_Haute_Number() << " "
                << zone.get_Zone_Basse_Number() << std::endl;

        const std::vector<Ennemi>& ennemis = zone.getEnnemi();
        fichier << ennemis.size() << std::endl;

        for (const Ennemi& ennemi : ennemis) {
            fichier << ennemi.getPositionX() << " "
                    << ennemi.getPositionY() << " "
                    << ennemi.getPv() << std::endl;
        }
    }

    fichier.close();
}

void Sauvegarde::chargerPartie(const std::string& nom_fichier, ContexteJeu& contexte) {
    std::ifstream fichier(nom_fichier);
    if (!fichier.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier pour le chargement de la partie." << std::endl;
        return;
    }

    std::string nom;
    int posX, posY, pv, attaque, defense, zoneActuelle, niveauActuel,score;
    char orientation;

    std::getline(fichier, nom);
    fichier >> posX >> posY;
    fichier >> pv;
    fichier >> attaque;
    fichier >> defense;
    fichier >> orientation;
    fichier >> zoneActuelle;
    fichier >> niveauActuel;
    fichier >> score;
    Joueur& joueur = contexte.recupereJoueur();
    joueur.nom = nom;
    joueur.setPositionX(posX);
    joueur.setPositionY(posY);
    joueur.setPv(pv);
    joueur.setAttaque(attaque);
    joueur.setDefense(defense);
    joueur.SetOrientation(orientation);

    contexte.set_Actual_Level(niveauActuel);
    contexte.setScore(score);

    Niveau& niveau = contexte.recupereNiveau(niveauActuel);
    niveau.clearZones();

    int nbZones = 0;
    fichier >> nbZones;

    for (int i = 0; i < nbZones; ++i) {
        std::string terrainToken;
        int gauche = -1;
        int droite = -1;
        int haut = -1;
        int bas = -1;
        int nbEnnemis = 0;
        std::string numTerrain = "1";

        fichier >> terrainToken;
        if (terrainToken.rfind("terrain=", 0) == 0) {
            numTerrain = terrainToken.substr(8);
            fichier >> gauche >> droite >> haut >> bas;
        }
        else {
            // Compatibilite avec les anciennes sauvegardes qui ne stockaient pas le terrain.
            gauche = std::atoi(terrainToken.c_str());
            fichier >> droite >> haut >> bas;
        }
        fichier >> nbEnnemis;

        Zone zoneChargee(numTerrain);
        zoneChargee.set_Zone_Gauche_Number(gauche);
        zoneChargee.set_Zone_Droite_Number(droite);
        zoneChargee.set_Zone_Haute_Number(haut);
        zoneChargee.set_Zone_Basse_Number(bas);
        zoneChargee.getEnnemi().clear();

        for (int j = 0; j < nbEnnemis; ++j) {
            int ennemiX, ennemiY, ennemiPv;
            fichier >> ennemiX >> ennemiY >> ennemiPv;

            Ennemi ennemi;
            ennemi.setPositionX(ennemiX);
            ennemi.setPositionY(ennemiY);
            ennemi.setPv(ennemiPv);
            zoneChargee.getEnnemi().push_back(ennemi);
        }

        niveau.addZone(zoneChargee);
    }

    fichier.close();

    if (nbZones == 0) {
        Zone zoneParDefaut;
        niveau.addZone(zoneParDefaut);
        zoneActuelle = 0;
    }

    if (zoneActuelle < 0 || zoneActuelle >= niveau.get_number_of_zone_in_the_level()) {
        zoneActuelle = 0;
    }

    contexte.set_Zone_Number(zoneActuelle);
}
