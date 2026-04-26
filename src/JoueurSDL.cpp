#include "AffichageSDL.h"
#include "AffichageSDLUtils.h"
#include "joueur.h"

#include <SDL2/SDL.h>

// Rendu du joueur: orientation, animation idle/attaque et fallback sprite legacy.

void AffichageSDL::enregistrerDirectionDepuisTouche(SDL_Keycode key) {
    // On mémorise la direction visuelle pour piloter les animations (idle/attaque).
    if (key == SDLK_LEFT || key == SDLK_q) {
        directionJoueur = DIR_GAUCHE;
    }
    else if (key == SDLK_RIGHT || key == SDLK_d) {
        directionJoueur = DIR_DROITE;
    }
    else if (key == SDLK_UP || key == SDLK_z) {
        directionJoueur = DIR_HAUT;
    }
    else if (key == SDLK_DOWN || key == SDLK_s) {
        directionJoueur = DIR_BAS;
    }
}

void AffichageSDL::afficherJoueur(const Joueur& joueur, int cameraX, int cameraY) {
    const int tailleCase = tailleCaseAffichee();
    const int MARGE_FRAME = 2;
    const int DECALAGE_SPRITE_Y = static_cast<int>(2 * zoomNiveau);
    const int x = joueur.getPositionX() / 32;
    const int y = joueur.getPositionY() / 32;

    if (joueur.getPv() <= 0) {
        afficherAnimationMort(joueur, cameraX, cameraY);
        return;
    }
    animationMortActive = false;

    auto dessinerRectJoueur = [&]() {
        SDL_Color couleurJoueur = {0, 255, 0, 255};
        SDL_Rect rect = {
            cameraX + x * tailleCase + 2,
            cameraY + y * tailleCase + 2,
            tailleCase - 4,
            tailleCase - 4
        };

        SDL_SetRenderDrawColor(renderer, couleurJoueur.r, couleurJoueur.g, couleurJoueur.b, couleurJoueur.a);
        SDL_RenderFillRect(renderer, &rect);
    };

    const Uint32 maintenant = SDL_GetTicks();
    const bool enMouvement = (maintenant - dernierTickMouvement) < 260;
    const bool enAttaque = (maintenant - dernierTickAttaque) < 320;

    SDL_Texture* spriteAction = nullptr;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    // Sélection du sprite d'action selon l'orientation et la variante d'attaque.
    if (enAttaque) {
        const int indexVariante = varianteAttaque % 2;
        switch (directionJoueur) {
            case DIR_BAS:
                spriteAction = AffichageSDLUtils::selectionnerVariante(spriteAttaqueBas, indexVariante);
                break;
            case DIR_HAUT:
                spriteAction = AffichageSDLUtils::selectionnerVariante(spriteAttaqueHaut, indexVariante);
                break;
            case DIR_GAUCHE:
                spriteAction = AffichageSDLUtils::selectionnerVariante(spriteAttaqueGauche, indexVariante);
                flip = SDL_FLIP_HORIZONTAL;
                break;
            case DIR_DROITE:
                // Les assets latéraux partagent la même source; la gauche est obtenue par miroir.
                spriteAction = AffichageSDLUtils::selectionnerVariante(spriteAttaqueGauche, indexVariante);
                break;
        }
    }

    // Si pas de sprite d'attaque, on utilise idle si disponible
    if (spriteAction == nullptr && !enAttaque) {
        spriteAction = spriteIdle;
        if (directionJoueur == DIR_GAUCHE) {
            flip = SDL_FLIP_HORIZONTAL;
        }
    }

    if (spriteAction != nullptr) {
        int textureW = 0;
        int textureH = 0;
        if (SDL_QueryTexture(spriteAction, nullptr, nullptr, &textureW, &textureH) == 0 && textureW > 0 && textureH > 0) {
            int frameW = textureH;
            if (frameW <= 0 || frameW > textureW) {
                frameW = textureW;
            }

            int totalFrames = textureW / frameW;
            if (totalFrames <= 0) {
                totalFrames = 1;
            }

            int frameIndex = 0;
            if (enAttaque) {
                frameIndex = static_cast<int>((maintenant / 120) % totalFrames);
            }
            else if (enMouvement) {
                frameIndex = static_cast<int>((maintenant / 220) % totalFrames);
            }
            else {
                // Animation idle pour la sprite de 6 frames
                frameIndex = static_cast<int>((maintenant / 150) % totalFrames);
            }

            int srcX = frameIndex * frameW + MARGE_FRAME;
            int srcW = frameW - (2 * MARGE_FRAME);
            if (srcW <= 0) {
                srcX = frameIndex * frameW;
                srcW = frameW;
            }

            SDL_Rect src = {
                srcX,
                0,
                srcW,
                textureH
            };

            SDL_Rect dst;
            dst.h = static_cast<int>(tailleCase * echelleJoueur);
            if (dst.h < tailleCase) {
                dst.h = tailleCase;
            }
            dst.w = (frameW * dst.h) / textureH;
            if (dst.w < tailleCase) {
                dst.w = tailleCase;
            }
            dst.x = cameraX + x * tailleCase + (tailleCase - dst.w) / 2;
            dst.y = cameraY + y * tailleCase + (tailleCase - dst.h) / 2 + DECALAGE_SPRITE_Y;

            SDL_RenderCopyEx(renderer, spriteAction, &src, &dst, 0.0, nullptr, flip);
            return;
        }
    }

    if (spriteJoueur == nullptr) {
        dessinerRectJoueur();
        return;
    }

    static const int LEGACY_FRAME_W = 40;
    static const int LEGACY_FRAME_H = 64;
    static const int LEGACY_FRAMES_PAR_DIR = 7;

    int textureW = 0;
    int textureH = 0;
    if (SDL_QueryTexture(spriteJoueur, nullptr, nullptr, &textureW, &textureH) != 0 ||
        textureW < LEGACY_FRAME_W || textureH < LEGACY_FRAME_H) {
        dessinerRectJoueur();
        return;
    }

    const int totalFrames = textureW / LEGACY_FRAME_W;
    if (totalFrames <= 0) {
        return;
    }

    int baseFrame = 0;
    SDL_RendererFlip legacyFlip = SDL_FLIP_NONE;

    // Cette sprite sheet contient 3 blocs de 7 frames: bas, haut, droite.
    // La direction gauche est obtenue par miroir horizontal des frames droite.
    switch (directionJoueur) {
        case DIR_BAS:
            baseFrame = 0;
            break;
        case DIR_HAUT:
            baseFrame = 7;
            break;
        case DIR_DROITE:
            baseFrame = 14;
            break;
        case DIR_GAUCHE:
            baseFrame = 14;
            legacyFlip = SDL_FLIP_HORIZONTAL;
            break;
    }

    if (baseFrame >= totalFrames) {
        baseFrame = 0;
        legacyFlip = SDL_FLIP_NONE;
    }

    int framesDirection = totalFrames - baseFrame;
    if (framesDirection > LEGACY_FRAMES_PAR_DIR) {
        framesDirection = LEGACY_FRAMES_PAR_DIR;
    }
    if (framesDirection <= 0) {
        framesDirection = (totalFrames < LEGACY_FRAMES_PAR_DIR) ? totalFrames : LEGACY_FRAMES_PAR_DIR;
        baseFrame = 0;
        legacyFlip = SDL_FLIP_NONE;
    }

    const int frameAnimee = enMouvement ? static_cast<int>((maintenant / 200) % framesDirection) : 0;
    int frameIndex = baseFrame + frameAnimee;
    if (frameIndex >= totalFrames) {
        frameIndex = 0;
    }

    int legacySrcX = frameIndex * LEGACY_FRAME_W + 1;
    int legacySrcW = LEGACY_FRAME_W - 2;
    if (legacySrcW <= 0) {
        legacySrcX = frameIndex * LEGACY_FRAME_W;
        legacySrcW = LEGACY_FRAME_W;
    }

    SDL_Rect src = {
        legacySrcX,
        0,
        legacySrcW,
        LEGACY_FRAME_H
    };

    SDL_Rect dst;
    dst.h = static_cast<int>(tailleCase * echelleJoueur);
    if (dst.h < tailleCase) {
        dst.h = tailleCase;
    }
    dst.w = (LEGACY_FRAME_W * dst.h) / LEGACY_FRAME_H;
    if (dst.w < tailleCase) {
        dst.w = tailleCase;
    }
    dst.x = cameraX + x * tailleCase + (tailleCase - dst.w) / 2;
    dst.y = cameraY + y * tailleCase + (tailleCase - dst.h) / 2 + DECALAGE_SPRITE_Y;

    SDL_RenderCopyEx(renderer, spriteJoueur, &src, &dst, 0.0, nullptr, legacyFlip);
}
