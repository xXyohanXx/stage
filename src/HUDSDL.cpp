#include "AffichageSDL.h"
#include "AffichageSDLUtils.h"

#include <algorithm>
#include <cstdio>

// Rendu du HUD (PV/ATK/DEF/nom/kills) avec cache de textures texte.

namespace {

bool couleurEgale(const SDL_Color& a, const SDL_Color& b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

SDL_Texture* creerTextureTexteOmbre(SDL_Renderer* renderer, TTF_Font* font, const char* texte, SDL_Color couleurPrincipale) {
    if (renderer == nullptr || font == nullptr || texte == nullptr) {
        return nullptr;
    }

    const SDL_Color couleurOmbre = {0, 0, 0, 255};
    const int offset = 2;

    SDL_Surface* surfaceOmbre = TTF_RenderUTF8_Blended(font, texte, couleurOmbre);
    if (surfaceOmbre == nullptr) {
        return nullptr;
    }

    SDL_Surface* surfacePrincipale = TTF_RenderUTF8_Blended(font, texte, couleurPrincipale);
    if (surfacePrincipale == nullptr) {
        SDL_FreeSurface(surfaceOmbre);
        return nullptr;
    }

    const int largeur = std::max(surfacePrincipale->w, surfaceOmbre->w + offset);
    const int hauteur = std::max(surfacePrincipale->h, surfaceOmbre->h + offset);

    SDL_Surface* surfaceCombinee = SDL_CreateRGBSurfaceWithFormat(0, largeur, hauteur, 32, SDL_PIXELFORMAT_RGBA32);
    if (surfaceCombinee == nullptr) {
        SDL_FreeSurface(surfacePrincipale);
        SDL_FreeSurface(surfaceOmbre);
        return nullptr;
    }

    SDL_SetSurfaceBlendMode(surfaceOmbre, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceBlendMode(surfacePrincipale, SDL_BLENDMODE_BLEND);

    SDL_Rect dstOmbre = {offset, offset, surfaceOmbre->w, surfaceOmbre->h};
    SDL_Rect dstPrincipal = {0, 0, surfacePrincipale->w, surfacePrincipale->h};
    SDL_BlitSurface(surfaceOmbre, nullptr, surfaceCombinee, &dstOmbre);
    SDL_BlitSurface(surfacePrincipale, nullptr, surfaceCombinee, &dstPrincipal);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surfaceCombinee);

    SDL_FreeSurface(surfaceCombinee);
    SDL_FreeSurface(surfacePrincipale);
    SDL_FreeSurface(surfaceOmbre);

    return texture;
}

}

// Affiche les statistiques du joueur sur le panneau de droite.
void AffichageSDL::afficherStats(int pv, int maxPv, int attaque, int defense, const string& nomJoueur, int ennemisTues) {
    SDL_Color blanc = {240, 240, 240, 255};
    SDL_Color rouge = {255, 50, 50, 255};
    const float ECHELLE_HUD = 0.78f;

    if (fontJeu != nullptr) {
        char lignePv[100];
        char ligneAtk[100];
        char ligneDef[100];
        char ligneNom[100];
        char ligneKills[100];

        snprintf(lignePv, sizeof(lignePv), "PV  %d sur %d", pv, maxPv);
        snprintf(ligneAtk, sizeof(ligneAtk), "ATK  %d", attaque);
        snprintf(ligneDef, sizeof(ligneDef), "DEF  %d", defense);
        const string nomAffiche = nomJoueur.empty() ? "Joueur" : nomJoueur;
        snprintf(ligneNom, sizeof(ligneNom), "NOM  %s", nomAffiche.c_str());
        snprintf(ligneKills, sizeof(ligneKills), "ENNEMIS TUES  %d", ennemisTues);

        const array<const char*, 5> lignes = {lignePv, ligneAtk, ligneDef, ligneNom, ligneKills};
        const array<SDL_Color, 5> couleurs = {pv > maxPv / 2 ? blanc : rouge, blanc, blanc, blanc, blanc};
        const array<int, 5> posX = {655, 655, 655, 655, 655};
        const array<int, 5> posY = {90, 124, 158, 192, 226};

        for (int i = 0; i < 5; ++i) {
            const string texteLigne = lignes[i];
            const SDL_Color couleurLigne = couleurs[i];
            HudTextCacheEntry& cache = hudTextCache[i];

            const bool doitRafraichir = (cache.texture == nullptr || cache.texte != texteLigne || !couleurEgale(cache.couleur, couleurLigne));
            if (doitRafraichir) {
                if (cache.texture != nullptr) {
                    SDL_DestroyTexture(cache.texture);
                    cache.texture = nullptr;
                }

                cache.texture = creerTextureTexteOmbre(renderer, fontJeu, texteLigne.c_str(), couleurLigne);
                cache.texte = texteLigne;
                cache.couleur = couleurLigne;
                cache.largeur = 0;
                cache.hauteur = 0;

                if (cache.texture != nullptr) {
                    SDL_QueryTexture(cache.texture, nullptr, nullptr, &cache.largeur, &cache.hauteur);
                }
            }

            if (cache.texture != nullptr) {
                SDL_Rect dst = {
                    posX[i],
                    posY[i],
                    static_cast<int>(cache.largeur * ECHELLE_HUD),
                    static_cast<int>(cache.hauteur * ECHELLE_HUD)
                };

                if (dst.w < 1) dst.w = 1;
                if (dst.h < 1) dst.h = 1;
                SDL_RenderCopy(renderer, cache.texture, nullptr, &dst);
            }
            else {
                AffichageSDLUtils::ombrage_texte(renderer, fontJeu, texteLigne.c_str(), posX[i], posY[i], couleurLigne);
            }
        }
    }
}
