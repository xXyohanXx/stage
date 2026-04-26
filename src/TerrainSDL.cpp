#include "AffichageSDL.h"

// Rendu des tuiles de terrain: village/donjon, bordures, decor et effets d'ambiance.

namespace {

bool estTerrain(char tuile) {
    return tuile == ' ' || tuile == 'C' || tuile == 'O';
}

bool estBordure(char tuile) {
    return tuile == '-';
}

SDL_Rect atlasRect(SDL_Texture* texture, int colonnes, int lignes, int index) {
    SDL_Rect rect = {0, 0, 0, 0};
    if (texture == nullptr || colonnes <= 0 || lignes <= 0 || index < 0) {
        return rect;
    }

    int textureW = 0;
    int textureH = 0;
    if (SDL_QueryTexture(texture, nullptr, nullptr, &textureW, &textureH) != 0 || textureW <= 0 || textureH <= 0) {
        return rect;
    }

    const int cellW = textureW / colonnes;
    const int cellH = textureH / lignes;
    if (cellW <= 0 || cellH <= 0) {
        return rect;
    }

    const int maxIndex = colonnes * lignes;
    index %= maxIndex;

    rect.x = (index % colonnes) * cellW;
    rect.y = (index / colonnes) * cellH;
    rect.w = cellW;
    rect.h = cellH;
    return rect;
}

}

void AffichageSDL::afficherTerrain(const char terrain[20][26], int cameraX, int cameraY, bool dansVillage, int zoneNumber, bool dessinerFond) {
    const int largeurTerrain = 25;
    const int hauteurTerrain = 20;
    const int tailleCase = tailleCaseAffichee();
    SDL_Rect vue = {0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};

    // Effet sombre applique sur la zone intermediaire (10) et tout le donjon.
    const bool estZonePortail = (zoneNumber == 10);
    const bool estZoneDonjon = !dansVillage;
    const bool activerEffetSombre = estZonePortail || estZoneDonjon;

    // Fond eau par défaut, ou noir si portail
    if (dessinerFond) {
        if (activerEffetSombre) {
            SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 72, 164, 166, 255);
        }
        SDL_RenderFillRect(renderer, &vue);
    }

    SDL_Texture* textureEau = (tilesTerrain[4] != nullptr) ? tilesTerrain[4] : tilemapTopDown;
    SDL_Texture* textureFoam = decorWaterFoam;
    const array<SDL_Texture*, 9>& tilesActifs = dansVillage ? tilesTerrainEau : tilesTerrainDonjon;
    SDL_Texture* atlasSol = (tilesActifs[4] != nullptr) ? tilesActifs[4] : tilemapFlat;

    const int tileCoinSupGauche = 0;
    const int tileCoteHaut = 1;
    const int tileCoinSupDroit = 2;
    const int tileCoteGauche = 3;
    const int tileCentre = 4;
    const int tileCoteDroit = 5;
    const int tileCoinInfGauche = 6;
    const int tileCoteBas = 7;
    const int tileCoinInfDroit = 8;

    // Animation mousse désactivée

    for (int y = 0; y < hauteurTerrain; ++y) {
        for (int x = 0; x < largeurTerrain; ++x) {
            SDL_Rect dst = {
                cameraX + x * tailleCase,
                cameraY + y * tailleCase,
                tailleCase,
                tailleCase
            };

            if (textureEau != nullptr) {
                SDL_RenderCopy(renderer, textureEau, nullptr, &dst);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 72, 164, 166, 255);
                SDL_RenderFillRect(renderer, &dst);
            }

            const bool terrainCase = estTerrain(terrain[y][x]);
            const bool bordureCase = estBordure(terrain[y][x]);

            if (terrainCase) {
                if (tilesActifs[tileCentre] != nullptr) {
                    SDL_RenderCopy(renderer, tilesActifs[tileCentre], nullptr, &dst);
                }
                else if (atlasSol != nullptr) {
                    const SDL_Rect srcSol = atlasRect(atlasSol, 10, 4, tileCentre);
                    SDL_RenderCopy(renderer, atlasSol, &srcSol, &dst);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 72, 122, 80, 255);
                    SDL_RenderFillRect(renderer, &dst);
                }
                continue;
            }

            // Afficher les maisons (carrés bleus)
            if (terrain[y][x] == 'M') {
                const int hauteurHero = static_cast<int>(tailleCase * echelleJoueur);
                int hauteurMaison = hauteurHero * 2;
                const int hauteurMaisonMin = tailleCase * 2;
                const int hauteurMaisonMax = tailleCase * 4;
                if (hauteurMaison < hauteurMaisonMin) {
                    hauteurMaison = hauteurMaisonMin;
                }
                if (hauteurMaison > hauteurMaisonMax) {
                    hauteurMaison = hauteurMaisonMax;
                }

                int largeurMaison = hauteurMaison;
                if (houseBlue != nullptr) {
                    int textureW = 0;
                    int textureH = 0;
                    if (SDL_QueryTexture(houseBlue, nullptr, nullptr, &textureW, &textureH) == 0 && textureW > 0 && textureH > 0) {
                        largeurMaison = (textureW * hauteurMaison) / textureH;
                    }
                }

                SDL_Rect dstMaison = {
                    dst.x + (tailleCase - largeurMaison) / 2,
                    dst.y + (tailleCase - hauteurMaison),
                    largeurMaison,
                    hauteurMaison
                };

                if (houseBlue != nullptr) {
                    SDL_RenderCopy(renderer, houseBlue, nullptr, &dstMaison);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 50, 100, 200, 255);
                    SDL_RenderFillRect(renderer, &dstMaison);
                }
                continue;
            }

            // Afficher les murs/bordures avec Rock4.png
            if (bordureCase) {
                const bool assombrirRochers = !dansVillage;
                const Uint8 murModR = 120;
                const Uint8 murModG = 120;
                const Uint8 murModB = 120;
                int tileIndex = tileCentre;
                if (y == 0 && x == 0) tileIndex = tileCoinSupGauche;
                else if (y == 0 && x == largeurTerrain - 1) tileIndex = tileCoinSupDroit;
                else if (y == hauteurTerrain - 1 && x == 0) tileIndex = tileCoinInfGauche;
                else if (y == hauteurTerrain - 1 && x == largeurTerrain - 1) tileIndex = tileCoinInfDroit;
                else if (y == 0) tileIndex = tileCoteHaut;
                else if (y == hauteurTerrain - 1) tileIndex = tileCoteBas;
                else if (x == 0) tileIndex = tileCoteGauche;
                else if (x == largeurTerrain - 1) tileIndex = tileCoteDroit;

                if (tilesActifs[tileIndex] != nullptr) {
                    if (assombrirRochers) {
                        SDL_SetTextureColorMod(tilesActifs[tileIndex], murModR, murModG, murModB);
                    }
                    SDL_RenderCopy(renderer, tilesActifs[tileIndex], nullptr, &dst);
                    if (assombrirRochers) {
                        SDL_SetTextureColorMod(tilesActifs[tileIndex], 255, 255, 255);
                    }
                }
                else if (atlasSol != nullptr) {
                    const SDL_Rect srcSol = atlasRect(atlasSol, 10, 4, tileIndex);
                    if (assombrirRochers) {
                        SDL_SetTextureColorMod(atlasSol, murModR, murModG, murModB);
                    }
                    SDL_RenderCopy(renderer, atlasSol, &srcSol, &dst);
                    if (assombrirRochers) {
                        SDL_SetTextureColorMod(atlasSol, 255, 255, 255);
                    }
                }
                else {
                    if (assombrirRochers) {
                        SDL_SetRenderDrawColor(renderer, 45, 70, 55, 255);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 72, 122, 80, 255);
                    }
                    SDL_RenderFillRect(renderer, &dst);
                }

                if (rockWall != nullptr) {
                    SDL_Rect dstRock = {
                        dst.x - 1,
                        dst.y - 1,
                        dst.w + 3,
                        dst.h + 3
                    };
                    if (assombrirRochers) {
                        SDL_SetTextureColorMod(rockWall, murModR, murModG, murModB);
                    }
                    SDL_RenderCopy(renderer, rockWall, nullptr, &dstRock);
                    if (assombrirRochers) {
                        SDL_SetTextureColorMod(rockWall, 255, 255, 255);
                    }
                }
                continue;
            }

            SDL_SetRenderDrawColor(renderer, 72, 164, 166, 255);
            SDL_RenderFillRect(renderer, &dst);

        }
    }

    // Effet de shadow/obscurite pour la transition et le donjon.
    if (activerEffetSombre && dessinerFond) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        // Premier overlay semi-transparent foncé
        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 150);
        SDL_Rect overlayRect = {0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
        SDL_RenderFillRect(renderer, &overlayRect);

        // Si on a la texture shadow, l'afficher aussi
        if (tilemapShadows != nullptr) {
            for (int y = 0; y < hauteurTerrain; ++y) {
                for (int x = 0; x < largeurTerrain; ++x) {
                    SDL_Rect dst = {
                        cameraX + x * tailleCase,
                        cameraY + y * tailleCase,
                        tailleCase,
                        tailleCase
                    };
                    SDL_RenderCopy(renderer, tilemapShadows, nullptr, &dst);
                }
            }
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}
