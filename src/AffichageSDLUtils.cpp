#include "AffichageSDLUtils.h"

namespace AffichageSDLUtils {

// Outils utilitaires SDL partages: rendu texte, chargement robuste des textures, fallback de variantes.

// Rendu d'un texte principal avec ombre pour améliorer la lisibilite.
void ombrage_texte(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color mainColor) {
    if (renderer == nullptr || font == nullptr || text == nullptr) {
        return;
    }

    SDL_Color ombreCouleur = {0, 0, 0, 255};
    const int offset = 2;

    SDL_Surface* surfaceOmbre = TTF_RenderUTF8_Blended(font, text, ombreCouleur);
    if (surfaceOmbre == nullptr) {
        return;
    }

    SDL_Texture* textureOmbre = SDL_CreateTextureFromSurface(renderer, surfaceOmbre);
    if (textureOmbre == nullptr) {
        SDL_FreeSurface(surfaceOmbre);
        return;
    }

    SDL_Rect shadowRect = {x + offset, y + offset, surfaceOmbre->w, surfaceOmbre->h};
    SDL_RenderCopy(renderer, textureOmbre, nullptr, &shadowRect);

    SDL_DestroyTexture(textureOmbre);
    SDL_FreeSurface(surfaceOmbre);

    SDL_Surface* mainSurface = TTF_RenderUTF8_Blended(font, text, mainColor);
    if (mainSurface == nullptr) {
        return;
    }

    SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(renderer, mainSurface);
    if (mainTexture == nullptr) {
        SDL_FreeSurface(mainSurface);
        return;
    }

    SDL_Rect mainRect = {x, y, mainSurface->w, mainSurface->h};
    SDL_RenderCopy(renderer, mainTexture, nullptr, &mainRect);

    SDL_DestroyTexture(mainTexture);
    SDL_FreeSurface(mainSurface);
}

SDL_Texture* chargerTextureOptionnelle(SDL_Renderer* renderer, const std::vector<std::string>& chemins) {
    // Retourne la premiere texture chargee avec succes dans la liste.
    for (const std::string& chemin : chemins) {
        SDL_Texture* texture = IMG_LoadTexture(renderer, chemin.c_str());
        if (texture != nullptr) {
            return texture;
        }
    }
    return nullptr;
}

SDL_Texture* selectionnerVariante(const std::array<SDL_Texture*, 2>& variantes, int indexVariante) {
    // Fallback: variante demandee -> variante 0 -> nullptr.
    const int indexNormalise = (indexVariante == 1) ? 1 : 0;
    if (variantes[indexNormalise] != nullptr) {
        return variantes[indexNormalise];
    }
    if (variantes[0] != nullptr) {
        return variantes[0];
    }
    return nullptr;
}

}
