#include "AffichageSDL.h"
#include "ContexteJeu.h"
#include "Niveau.h"
#include "Zone.h"
#include "joueur.h"
#include "Ennemi.h"
#include "attaque.h"

#include <SDL2/SDL.h>
#include <cctype>
#include <filesystem>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Boucle de jeu SDL: entrees clavier, camera, rendu et transitions de fin de partie.

namespace {

const string DOSSIER_SAUVEGARDE = "data/sauvegarde";
const size_t LIMITE_SAUVEGARDES = 5;

size_t compterSauvegardes() {
    namespace fs = std::filesystem;
    std::error_code ec;
    if (!fs::exists(DOSSIER_SAUVEGARDE, ec)) {
        return 0;
    }

    size_t total = 0;
    for (const fs::directory_entry& entree : fs::directory_iterator(DOSSIER_SAUVEGARDE, ec)) {
        if (ec) {
            break;
        }
        if (entree.is_regular_file() && entree.path().extension() == ".txt") {
            total += 1;
        }
    }
    return total;
}

string nettoyerNomFichier(const string& nom) {
    string nettoye;
    for (char c : nom) {
        const unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalnum(uc)) {
            nettoye.push_back(c);
        }
        else if (c == ' ' || c == '-' || c == '_') {
            nettoye.push_back('_');
        }
    }

    if (nettoye.empty()) {
        return "joueur";
    }
    return nettoye;
}

string cheminSauvegardeAuto(const Joueur& joueur) {
    return DOSSIER_SAUVEGARDE + "/" + nettoyerNomFichier(joueur.nom) + ".txt";
}

map<int, pair<int, int>> construirePositionsVillage(Niveau& niveau) {
    map<int, pair<int, int>> positions;

    const int nbZones = niveau.get_number_of_zone_in_the_level();
    if (nbZones <= 0) {
        return positions;
    }

    const int MAX_ZONES_VILLAGE = 10;
    const int borneVillage = (nbZones < MAX_ZONES_VILLAGE) ? nbZones : MAX_ZONES_VILLAGE;

    queue<int> aExplorer;
    positions[0] = {0, 0};
    aExplorer.push(0);

    while (!aExplorer.empty()) {
        const int idZone = aExplorer.front();
        aExplorer.pop();

        if (idZone < 0 || idZone >= borneVillage) {
            continue;
        }

        Zone& zone = niveau.getZone(idZone);
        const int x = positions[idZone].first;
        const int y = positions[idZone].second;

        const array<pair<int, pair<int, int>>, 4> voisins = {{
            {zone.get_Zone_Gauche_Number(), {-1, 0}},
            {zone.get_Zone_Droite_Number(), {1, 0}},
            {zone.get_Zone_Haute_Number(), {0, -1}},
            {zone.get_Zone_Basse_Number(), {0, 1}}
        }};

        for (const auto& voisin : voisins) {
            const int idVoisin = voisin.first;
            const int dx = voisin.second.first;
            const int dy = voisin.second.second;

            if (idVoisin < 0 || idVoisin >= borneVillage) {
                continue;
            }

            if (positions.find(idVoisin) == positions.end()) {
                positions[idVoisin] = {x + dx, y + dy};
                aExplorer.push(idVoisin);
            }
        }
    }

    return positions;
}

}

int AffichageSDL::afficherJeu(ContexteJeu& contexte) {
    // Copie de l'etat initial pour permettre un 'rejouer' propre apres game over.
    ContexteJeu contexteDebutNiveau = contexte;
    bool enCours = true;
    bool sauvegarde = false;
    bool pause = false;
    bool sauvegardeAutoMortEffectuee = false;
    const Uint32 DELAI_GAMEOVER_MS = 5000;
    Uint32 dernierTickDeplacementEnnemi = 0;
    const Uint32 DELAI_DEPLACEMENT_ENNEMI_MS = 220;
    const float ZOOM_MIN_GLOBAL = 0.50f;
    const float ZOOM_MIN_DONJON_X = static_cast<float>(LARGEUR_FENETRE) / (25.0f * static_cast<float>(TAILLE_CASE));
    const float ZOOM_MIN_DONJON_Y = static_cast<float>(HAUTEUR_FENETRE) / (20.0f * static_cast<float>(TAILLE_CASE));
    const float ZOOM_MIN_DONJON = (ZOOM_MIN_DONJON_X > ZOOM_MIN_DONJON_Y) ? ZOOM_MIN_DONJON_X : ZOOM_MIN_DONJON_Y;

    while (enCours) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 1;
            }

            if (event.type == SDL_KEYDOWN) {
                const Uint32 maintenant = SDL_GetTicks();
                const Uint32 DELAI_DEPLACEMENT_MS = 120;
                const Uint32 DELAI_ATTAQUE_MS = 260;

                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    pause = true;
                }
                else if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_EQUALS || event.key.keysym.sym == SDLK_KP_PLUS) {
                    zoomNiveau += 0.10f;
                    if (zoomNiveau > 2.50f) {
                        zoomNiveau = 2.50f;
                    }
                }
                else if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS) {
                    zoomNiveau -= 0.10f;
                    float zoomMinimumAutorise = ZOOM_MIN_GLOBAL;
                    if (!contexte.estDansVillage()) {
                        zoomMinimumAutorise = ZOOM_MIN_DONJON;
                    }
                    if (zoomNiveau < zoomMinimumAutorise) {
                        zoomNiveau = zoomMinimumAutorise;
                    }
                }
                // Entrées actives uniquement hors pause et tant que le joueur est vivant.
                else if (!pause && contexte.recupereJoueur().getPv() > 0) {
                    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                        if ((maintenant - dernierTickActionDeplacement) >= DELAI_DEPLACEMENT_MS) {
                            enregistrerDirectionDepuisTouche(event.key.keysym.sym);
                            dernierTickMouvement = maintenant;
                            dernierTickActionDeplacement = maintenant;
                            contexte.actionClavier('z');
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                        if ((maintenant - dernierTickActionDeplacement) >= DELAI_DEPLACEMENT_MS) {
                            enregistrerDirectionDepuisTouche(event.key.keysym.sym);
                            dernierTickMouvement = maintenant;
                            dernierTickActionDeplacement = maintenant;
                            contexte.actionClavier('s');
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_q) {
                        if ((maintenant - dernierTickActionDeplacement) >= DELAI_DEPLACEMENT_MS) {
                            enregistrerDirectionDepuisTouche(event.key.keysym.sym);
                            dernierTickMouvement = maintenant;
                            dernierTickActionDeplacement = maintenant;
                            contexte.actionClavier('q');
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                        if ((maintenant - dernierTickActionDeplacement) >= DELAI_DEPLACEMENT_MS) {
                            enregistrerDirectionDepuisTouche(event.key.keysym.sym);
                            dernierTickMouvement = maintenant;
                            dernierTickActionDeplacement = maintenant;
                            contexte.actionClavier('d');
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_e) {
                        // Une attaque déclenche l'animation pendant une courte fenêtre temporelle.
                        if ((maintenant - dernierTickActionAttaque) >= DELAI_ATTAQUE_MS) {
                            dernierTickAttaque = maintenant;
                            dernierTickActionAttaque = maintenant;
                            varianteAttaque = (varianteAttaque + 1) % 2;
                            contexte.actionClavier('e');
                        }
                    }
                }
            }
        }

        if (pause) {
            int choixPause = afficherMenuPause(sauvegarde);

            if (choixPause == 0) {
                pause = false;
            }
            else if (choixPause == 1) {
                const string nomSauvegarde = "data/sauvegarde/" + nettoyerNomFichier(contexte.recupereJoueur().nom) + ".txt";
                std::error_code ec;
                const bool existeDeja = std::filesystem::exists(nomSauvegarde, ec);
                const size_t nbSauvegardes = compterSauvegardes();

                if (!existeDeja && nbSauvegardes >= LIMITE_SAUVEGARDES) {
                    afficherAlerteConfirmation("Limite de 5 sauvegardes atteinte. Supprimez-en une via Charger une partie.");
                    continue;
                }

                contexte.sauvegarderPartie(nomSauvegarde);
                sauvegarde = true;
                pause = false;
            }
            else if (choixPause == 2) {
                return 0;
            }
            else if (choixPause == 3) {
                return 1;
            }
            continue;
        }

        // Reutilise la logique deja presente dans ContexteJeu (deplacement_slime, etc.).
        const Uint32 maintenant = SDL_GetTicks();
        if ((maintenant - dernierTickDeplacementEnnemi) >= DELAI_DEPLACEMENT_ENNEMI_MS) {
            contexte.deplacement_ennemis();
            dernierTickDeplacementEnnemi = maintenant;
        }

        SDL_SetRenderDrawColor(renderer, 15, 18, 30, 255);
        SDL_RenderClear(renderer);

        Joueur& joueur = contexte.recupereJoueur();
        Niveau& niveau = contexte.recupereNiveau(contexte.get_Actual_Level());
        Zone& zone = niveau.getZone(contexte.get_Zone_Number());

        // En donjon, on interdit tout dezoom qui ferait depasser la zone de la fenetre.
        if (!contexte.estDansVillage() && zoomNiveau < ZOOM_MIN_DONJON) {
            zoomNiveau = ZOOM_MIN_DONJON;
        }

        const vector<Ennemi>& ennemis = zone.getEnnemi();
        vector<Attaque>& attaques = zone.getAttaques();
        const int tailleCase = tailleCaseAffichee();
        const int joueurTileX = joueur.getPositionX() / 32;
        const int joueurTileY = joueur.getPositionY() / 32;

        int effetTextureW = 0;
        int effetTextureH = 0;
        int effetFrameW = 0;
        int effetFramesTotal = 0;
        if (spriteEffetAttaque != nullptr && SDL_QueryTexture(spriteEffetAttaque, nullptr, nullptr, &effetTextureW, &effetTextureH) == 0 && effetTextureW > 0 && effetTextureH > 0) {
            effetFrameW = effetTextureH;
            if (effetFrameW <= 0 || (effetTextureW % effetFrameW) != 0) {
                effetFrameW = 192;
            }
            if (effetFrameW > 0) {
                effetFramesTotal = effetTextureW / effetFrameW;
            }
            if (effetFramesTotal <= 0) {
                effetFrameW = effetTextureW;
                effetFramesTotal = 1;
            }
        }

        const bool renduContinuVillage = contexte.estDansVillage();
        map<int, pair<int, int>> positionsVillage;
        int zoneCouranteOffsetTilesX = 0;
        int zoneCouranteOffsetTilesY = 0;

        int worldMinTileX = 0;
        int worldMinTileY = 0;
        int worldMaxTileX = 25;
        int worldMaxTileY = 20;

        if (renduContinuVillage) {
            positionsVillage = construirePositionsVillage(niveau);
            const auto itZoneCourante = positionsVillage.find(contexte.get_Zone_Number());
            if (itZoneCourante != positionsVillage.end()) {
                zoneCouranteOffsetTilesX = itZoneCourante->second.first * 25;
                zoneCouranteOffsetTilesY = itZoneCourante->second.second * 20;
            }

            if (!positionsVillage.empty()) {
                int minZoneX = positionsVillage.begin()->second.first;
                int maxZoneX = positionsVillage.begin()->second.first;
                int minZoneY = positionsVillage.begin()->second.second;
                int maxZoneY = positionsVillage.begin()->second.second;

                for (const auto& entree : positionsVillage) {
                    if (entree.second.first < minZoneX) minZoneX = entree.second.first;
                    if (entree.second.first > maxZoneX) maxZoneX = entree.second.first;
                    if (entree.second.second < minZoneY) minZoneY = entree.second.second;
                    if (entree.second.second > maxZoneY) maxZoneY = entree.second.second;
                }

                worldMinTileX = minZoneX * 25;
                worldMinTileY = minZoneY * 20;
                worldMaxTileX = (maxZoneX + 1) * 25;
                worldMaxTileY = (maxZoneY + 1) * 20;
            }
        }

        const int joueurWorldTileX = zoneCouranteOffsetTilesX + joueurTileX;
        const int joueurWorldTileY = zoneCouranteOffsetTilesY + joueurTileY;

        int cameraX = 0;
        int cameraY = 0;
        if (renduContinuVillage && !positionsVillage.empty()) {
            cameraX = (LARGEUR_FENETRE / 2) - (joueurWorldTileX * tailleCase + tailleCase / 2);
            cameraY = (HAUTEUR_FENETRE / 2) - (joueurWorldTileY * tailleCase + tailleCase / 2);

            const int largeurMondePx = (worldMaxTileX - worldMinTileX) * tailleCase;
            const int hauteurMondePx = (worldMaxTileY - worldMinTileY) * tailleCase;

            if (largeurMondePx <= LARGEUR_FENETRE) {
                cameraX = (LARGEUR_FENETRE - largeurMondePx) / 2 - worldMinTileX * tailleCase;
            }
            else {
                const int minCameraX = LARGEUR_FENETRE - worldMaxTileX * tailleCase;
                const int maxCameraX = -worldMinTileX * tailleCase;
                if (cameraX < minCameraX) cameraX = minCameraX;
                if (cameraX > maxCameraX) cameraX = maxCameraX;
            }

            if (hauteurMondePx <= HAUTEUR_FENETRE) {
                cameraY = (HAUTEUR_FENETRE - hauteurMondePx) / 2 - worldMinTileY * tailleCase;
            }
            else {
                const int minCameraY = HAUTEUR_FENETRE - worldMaxTileY * tailleCase;
                const int maxCameraY = -worldMinTileY * tailleCase;
                if (cameraY < minCameraY) cameraY = minCameraY;
                if (cameraY > maxCameraY) cameraY = maxCameraY;
            }
        }
        else {
            calculerCamera(joueurTileX, joueurTileY, tailleCase, cameraX, cameraY);
        }

        // Ordre de rendu: terrain -> ennemis -> joueur -> coffres -> pnj -> effets -> HUD -> dialogue.
        if (renduContinuVillage && !positionsVillage.empty()) {
            bool premiereZone = true;
            for (const auto& entree : positionsVillage) {
                const int idZoneAffichee = entree.first;
                const int decalageX = entree.second.first * 25 * tailleCase;
                const int decalageY = entree.second.second * 20 * tailleCase;
                Zone& zoneAffichee = niveau.getZone(idZoneAffichee);
                afficherTerrain(zoneAffichee.Terrain, cameraX + decalageX, cameraY + decalageY, true, idZoneAffichee, premiereZone);
                premiereZone = false;
            }
        }
        else {
            afficherTerrain(zone.Terrain, cameraX, cameraY, contexte.estDansVillage(), contexte.get_Zone_Number());
        }

        const int zoneCouranteOffsetPxX = zoneCouranteOffsetTilesX * tailleCase;
        const int zoneCouranteOffsetPxY = zoneCouranteOffsetTilesY * tailleCase;
        afficherEnnemis(ennemis, cameraX + zoneCouranteOffsetPxX, cameraY + zoneCouranteOffsetPxY);
        afficherJoueur(joueur, cameraX + zoneCouranteOffsetPxX, cameraY + zoneCouranteOffsetPxY);
        afficherCoffres(zone.getCoffres(), cameraX + zoneCouranteOffsetPxX, cameraY + zoneCouranteOffsetPxY);
        afficherPNJ(zone.getPNJ(), cameraX + zoneCouranteOffsetPxX, cameraY + zoneCouranteOffsetPxY);

        // Les attaques restent centrées sur le joueur, comme avant.
        for (int i = static_cast<int>(attaques.size()) - 1; i >= 0; --i) {
            const int tileX = attaques[i].PosX / 32;
            const int tileY = attaques[i].PosY / 32;

            if (spriteEffetAttaque != nullptr && effetFramesTotal > 0 && effetFrameW > 0) {
                int frameIndex = attaques[i].time / 2;
                if (frameIndex >= effetFramesTotal) {
                    frameIndex = effetFramesTotal - 1;
                }

                SDL_Rect src = {
                    frameIndex * effetFrameW,
                    0,
                    effetFrameW,
                    effetTextureH
                };

                const int tailleEffet = static_cast<int>(tailleCase * 4.0f);
                SDL_Rect dst = {
                    cameraX + joueurWorldTileX * tailleCase + (tailleCase - tailleEffet) / 2,
                    cameraY + joueurWorldTileY * tailleCase + (tailleCase - tailleEffet) / 2,
                    tailleEffet,
                    tailleEffet
                };

                const int attaqueWorldTileX = zoneCouranteOffsetTilesX + tileX;
                const int attaqueWorldTileY = zoneCouranteOffsetTilesY + tileY;
                const int dx = attaqueWorldTileX - joueurWorldTileX;
                const int dy = attaqueWorldTileY - joueurWorldTileY;
                double angle = 0.0;
                SDL_RendererFlip flipEffet = SDL_FLIP_NONE;

                if (dx < 0) {
                    flipEffet = SDL_FLIP_HORIZONTAL;
                }
                else if (dy < 0) {
                    angle = 90.0;
                }
                else if (dy > 0) {
                    angle = -90.0;
                }

                SDL_RenderCopyEx(renderer, spriteEffetAttaque, &src, &dst, angle, nullptr, flipEffet);
                attaques[i].time += 1;
                if (attaques[i].time > (effetFramesTotal * 2)) {
                    attaques.erase(attaques.begin() + i);
                }
            }
            else {
                const int tailleAttaque = (tailleCase > 8) ? (tailleCase - 8) : 2;
                SDL_Rect rect = {
                    cameraX + joueurWorldTileX * tailleCase + 4,
                    cameraY + joueurWorldTileY * tailleCase + 4,
                    tailleAttaque,
                    tailleAttaque
                };

                SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255);
                SDL_RenderFillRect(renderer, &rect);

                attaques[i].time += 1;
                if (attaques[i].time > 16) {
                    attaques.erase(attaques.begin() + i);
                }
            }
        }

        afficherStats(joueur.getPv(), 100, joueur.getAttaque(), joueur.getDefense(), joueur.nom, contexte.get_Nombre_Ennemis_Tues());
        afficherDialogue(contexte.get_dialoge());
        SDL_RenderPresent(renderer);

        if (contexte.estBossVaincu()) {
            const int choixVictoire = afficherVictoire();
            if (choixVictoire == -1) {
                return 1;
            }
            return 0;
        }

        // Auto-sauvegarde unique quand le joueur meurt.
        if (joueur.getPv() <= 0 && !sauvegardeAutoMortEffectuee) {
            contexte.sauvegarderPartie(cheminSauvegardeAuto(joueur));
            sauvegardeAutoMortEffectuee = true;
        }

        // On laisse jouer l'animation de mort au moins 10 secondes avant d'afficher le menu Game Over.
        if (joueur.getPv() <= 0 && animationMortActive && (SDL_GetTicks() - tickDebutAnimationMort) >= DELAI_GAMEOVER_MS) {
            const int choixGameOver = afficherGameOver();
            if (choixGameOver == 0) {
                contexte = contexteDebutNiveau;
                // Si la sauvegarde chargee etait deja a 0 PV, on relance vivant au meme point.
                if (contexte.recupereJoueur().getPv() <= 0) {
                    contexte.recupereJoueur().setPv(joueur.getPv()+20);
                }
                animationMortActive = false;
                tickDebutAnimationMort = 0;
                sauvegardeAutoMortEffectuee = false;
                continue;
            }
            if (choixGameOver == -1) {
                return 1;
            }
            return 0;
        }

        SDL_Delay(16);
    }

    return 1;
}
