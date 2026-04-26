#include "AffichageSDL.h"
#include "Pnj.h"

// Rendu SDL des PNJ sur la zone courante.

void AffichageSDL::afficherPNJ(const vector<Pnj>& pnjs, int cameraX, int cameraY) {
    const int tailleCase = tailleCaseAffichee();
    const int tailleSprite = static_cast<int>(tailleCase * echelleEnnemi);
    const int offset = (tailleCase - tailleSprite) / 2;

    for (const Pnj& pnj : pnjs) {
        const int x = cameraX + (pnj.getPositionX() / 32) * tailleCase + offset;
        const int y = cameraY + (pnj.getPositionY() / 32) * tailleCase + offset;

        if (spritePnj != nullptr) {
            SDL_Rect dst = {
                x,
                y,
                tailleSprite,
                tailleSprite
            };
            SDL_RenderCopy(renderer, spritePnj, nullptr, &dst);
        }
        else {
            SDL_Rect rect = {
                x,
                y,
                tailleSprite,
                tailleSprite
            };
            SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
