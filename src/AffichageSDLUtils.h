#ifndef AFFICHAGE_SDL_UTILS_H
#define AFFICHAGE_SDL_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <array>
#include <string>
#include <vector>

namespace AffichageSDLUtils {

/**
 * @brief Dessine un texte avec ombre portée.
 * @param renderer Renderer SDL de destination.
 * @param font Police TTF à utiliser.
 * @param text Chaîne UTF-8 à afficher.
 * @param x Position X du texte.
 * @param y Position Y du texte.
 * @param mainColor Couleur principale du texte.
 */
void ombrage_texte(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color mainColor);

/**
 * @brief Charge la première texture disponible dans une liste de chemins.
 * @param renderer Renderer SDL utilisé pour créer la texture.
 * @param chemins Liste ordonnée de chemins candidats.
 * @return Pointeur vers la texture chargée, ou nullptr si aucun chargement ne réussit.
 */
SDL_Texture* chargerTextureOptionnelle(SDL_Renderer* renderer, const std::vector<std::string>& chemins);

/**
 * @brief Sélectionne une variante de sprite avec fallback sécurisé.
 * @param variantes Tableau de deux variantes de textures.
 * @param indexVariante Index souhaité (0 ou 1).
 * @return Texture sélectionnée, ou nullptr si aucune variante n'est disponible.
 */
SDL_Texture* selectionnerVariante(const std::array<SDL_Texture*, 2>& variantes, int indexVariante);

}

#endif
