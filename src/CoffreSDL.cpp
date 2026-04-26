#include "AffichageSDL.h"
#include "Coffre.h"

// Rendu SDL des coffres non recuperes (disparition visuelle apres ouverture).

void AffichageSDL::afficherCoffres(const vector<Coffre>& coffres, int cameraX, int cameraY) {
    const int tailleCase = tailleCaseAffichee();
    const int tailleSprite = static_cast<int>(tailleCase * 3.0f);
    const int offset = (tailleCase - tailleSprite) / 2;

    for (const Coffre& coffre : coffres) {
        if (coffre.getContenu().empty()) {
            continue;
        }

        const int x = cameraX + (coffre.getPositionX() / 32) * tailleCase + offset;
        const int y = cameraY + (coffre.getPositionY() / 32) * tailleCase + offset;

        if (spriteCoffre != nullptr) {
            SDL_Rect dst = {
                x,
                y,
                tailleSprite,
                tailleSprite
            };
            SDL_RenderCopy(renderer, spriteCoffre, nullptr, &dst);
        }
        else {
            SDL_Rect rect = {
                x,
                y,
                tailleSprite,
                tailleSprite
            };
            SDL_SetRenderDrawColor(renderer, 200, 150, 50, 255);
            SDL_RenderFillRect(renderer, &rect);
            
            SDL_SetRenderDrawColor(renderer, 222, 184, 135, 255);
            SDL_Rect rectInner = {
                x + 2,
                y + 2,
                tailleSprite - 4,
                tailleSprite - 4
            };
            SDL_RenderDrawRect(renderer, &rectInner);
        }
    }
}
