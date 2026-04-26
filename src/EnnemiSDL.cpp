#include "AffichageSDL.h"
#include "Ennemi.h"

// Rendu des ennemis SDL, y compris le boss via un sprite dedie.

void AffichageSDL::afficherEnnemis(const vector<Ennemi>& ennemis, int cameraX, int cameraY, bool afficherBoss) {
    const int tailleCase = tailleCaseAffichee();
    const int tailleEnnemi = static_cast<int>(tailleCase * echelleEnnemi);
    const int offset = (tailleCase - tailleEnnemi) / 2;

    constexpr int FRAMES_IDLE = 7;
    constexpr int DUREE_FRAME_MS = 140;

    for (const Ennemi& ennemi : ennemis) {
        const bool estBoss = (ennemi.getType() == 'B') && afficherBoss;
        SDL_Texture* spriteActuel = estBoss ? spriteBoss : spriteEnnemi;
        
        SDL_Rect src = {0, 0, 0, 0};
        const bool utiliserSprite = (spriteActuel != nullptr)
            && (SDL_QueryTexture(spriteActuel, nullptr, nullptr, &src.w, &src.h) == 0)
            && (src.w > 0)
            && (src.h > 0);

        const int x = cameraX + (ennemi.getPositionX() / 32) * tailleCase + offset;
        const int y = cameraY + (ennemi.getPositionY() / 32) * tailleCase + offset;

        if (utiliserSprite) {
            if (!estBoss && src.w >= FRAMES_IDLE && ((src.w / FRAMES_IDLE) > 0)) {
                src.w /= FRAMES_IDLE;
                src.x = static_cast<int>((SDL_GetTicks() / DUREE_FRAME_MS) % FRAMES_IDLE) * src.w;
            }
            
            SDL_Rect dst = {
                x,
                y,
                tailleEnnemi,
                tailleEnnemi
            };
            
            if (!estBoss && src.w >= FRAMES_IDLE) {
                SDL_RenderCopy(renderer, spriteActuel, &src, &dst);
            } else {
                SDL_RenderCopy(renderer, spriteActuel, nullptr, &dst);
            }
        }
        else {
            SDL_Rect rect = {
                x,
                y,
                tailleEnnemi,
                tailleEnnemi
            };
            
            if (estBoss) {
                SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 50, 120, 255, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
