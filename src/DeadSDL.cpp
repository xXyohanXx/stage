#include "AffichageSDL.h"
#include "joueur.h"

#include <SDL2/SDL.h>
#include <algorithm>

// Rendu de l'animation de mort du joueur et fallback visuel sans sprite.

// Lance et dessine l'animation de mort du joueur jusqu'a sa derniere frame.
void AffichageSDL::afficherAnimationMort(const Joueur& joueur, int cameraX, int cameraY) {
    const int tailleCase = tailleCaseAffichee();
    const int x = joueur.getPositionX() / 32;
    const int y = joueur.getPositionY() / 32;
    const int FRAMES_MORT = 7;
    const int DUREE_FRAME_MS = 120;
    const Uint32 maintenant = SDL_GetTicks();

    if (!animationMortActive) {
        animationMortActive = true;
        tickDebutAnimationMort = maintenant;
    }

    if (spriteMort == nullptr) {
        SDL_SetRenderDrawColor(renderer, 180, 40, 40, 255);
        SDL_Rect rect = {
            cameraX + x * tailleCase + 2,
            cameraY + y * tailleCase + 2,
            tailleCase - 4,
            tailleCase - 4
        };
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    int textureW = 0;
    int textureH = 0;
    if (SDL_QueryTexture(spriteMort, nullptr, nullptr, &textureW, &textureH) != 0 ||
        textureW < FRAMES_MORT || textureH <= 0) {
        return;
    }

    const int frameW = textureW / FRAMES_MORT;
    if (frameW <= 0) {
        return;
    }

    const Uint32 ecoule = maintenant - tickDebutAnimationMort;
    int frameIndex = static_cast<int>(ecoule / DUREE_FRAME_MS);
    frameIndex = std::min(frameIndex, FRAMES_MORT - 1);

    SDL_Rect src = {
        frameIndex * frameW,
        0,
        frameW,
        textureH
    };

    int hauteur = static_cast<int>(tailleCase * echelleJoueur);
    if (hauteur < tailleCase) {
        hauteur = tailleCase;
    }
    int largeur = (frameW * hauteur) / textureH;
    if (largeur < tailleCase) {
        largeur = tailleCase;
    }

    SDL_Rect dst = {
        cameraX + x * tailleCase + (tailleCase - largeur) / 2,
        cameraY + y * tailleCase + (tailleCase - hauteur) / 2,
        largeur,
        hauteur
    };

    SDL_RenderCopy(renderer, spriteMort, &src, &dst);
}
