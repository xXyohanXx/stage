#include "AffichageSDL.h"
#include "AffichageSDLUtils.h"

// Rendu du bandeau de dialogue en bas d'ecran.

void AffichageSDL::afficherDialogue(const std::string& dialogue) {
    if (dialogue.empty()) {
        return;
    }

    const int HAUTEUR_BANDEAU = 80;
    const int MARGE = 10;
    const int Y_BANDEAU = HAUTEUR_FENETRE - HAUTEUR_BANDEAU - 10;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_Rect bandeau = {MARGE, Y_BANDEAU, LARGEUR_FENETRE - 2 * MARGE, HAUTEUR_BANDEAU};
    SDL_RenderFillRect(renderer, &bandeau);

    SDL_SetRenderDrawColor(renderer, 200, 150, 50, 255);
    SDL_RenderDrawRect(renderer, &bandeau);

    if (fontJeu != nullptr) {
        SDL_Color couleurTexte = {240, 240, 240, 255};
        AffichageSDLUtils::ombrage_texte(renderer, fontJeu, dialogue.c_str(), MARGE + 15, Y_BANDEAU + 15, couleurTexte);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
