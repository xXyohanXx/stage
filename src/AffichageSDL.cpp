#include "AffichageSDL.h"
#include "AffichageSDLUtils.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace std;

// Module d'initialisation SDL: cree la fenetre, charge les assets et gere leur destruction.

// Constructeur et destructeur pour initialiser et nettoyer les ressources SDL
AffichageSDL::AffichageSDL() {
    // Valeurs par defaut pour garantir un nettoyage partiel sur echec d'initialisation.
    window = nullptr;
    renderer = nullptr;
    fondMenu = nullptr;
    spriteJoueur = nullptr;
    spriteIdle = nullptr;
    spriteAttaqueBas = {nullptr, nullptr};
    spriteAttaqueHaut = {nullptr, nullptr};
    spriteAttaqueGauche = {nullptr, nullptr};
    spriteEffetAttaque = nullptr;
    spriteEnnemi = nullptr;
    spriteBoss = nullptr;
    spriteCoffre = nullptr;
    spritePnj = nullptr;
    spriteMort = nullptr;
    decorWaterFoam = nullptr;
    decorRockSmall = nullptr;
    decorRockLarge = nullptr;
    rockWall = nullptr;
    houseBlue = nullptr;
    tilemapTopDown = nullptr;
    tilemapFlat = nullptr;
    tilemapElevation = nullptr;
    tilemapShadows = nullptr;
    tilesTerrain = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    tilesTerrainEau = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    tilesTerrainDonjon = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    fontTitre = nullptr;
    fontMenu = nullptr;
    fontJeu = nullptr;
    for (HudTextCacheEntry& entree : hudTextCache) {
        entree.texture = nullptr;
        entree.texte.clear();
        entree.couleur = {0, 0, 0, 255};
        entree.largeur = 0;
        entree.hauteur = 0;
    }

    auto echecInitialisation = [&](const string& message) {
        if (fontTitre != nullptr) TTF_CloseFont(fontTitre);
        if (fontMenu != nullptr) TTF_CloseFont(fontMenu);
        if (fontJeu != nullptr) TTF_CloseFont(fontJeu);
        if (renderer != nullptr) SDL_DestroyRenderer(renderer);
        if (window != nullptr) SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        throw runtime_error(message);
    };

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        echecInitialisation(string("Erreur d'initialisation de SDL: ") + SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    // Création de la fenêtre
    window = SDL_CreateWindow("Castle Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        echecInitialisation(string("Erreur de creation de la fenetre: ") + SDL_GetError());
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        echecInitialisation(string("Erreur de creation du renderer: ") + SDL_GetError());
    }

    // Initialisation de TTF
    if (TTF_Init() < 0) {
        echecInitialisation(string("Erreur d'initialisation de TTF: ") + TTF_GetError());
    }

    // Initialisation de SDL_image (PNG/JPG)
    if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)) {
        cout << "Avertissement IMG_Init: " << IMG_GetError() << endl;
    }

    // Image de fond pour le menu
    fondMenu = IMG_LoadTexture(renderer, "data/img/castle.png");
    if (fondMenu == nullptr) {
        cout << "Aucune image de fond: " << IMG_GetError() << endl;
    }
    
    // Chargement des polices
    fontTitre = TTF_OpenFont("data/font/8bitlimr.ttf", 100);
    fontMenu  = TTF_OpenFont("data/font/8bitlim.ttf", 50);
    fontJeu = TTF_OpenFont("data/font/8bitlimr.ttf", 20);

    // Vérification du chargement des polices
    if (fontTitre == nullptr || fontMenu == nullptr || fontJeu == nullptr) {
        cout << "Avertissement: police non chargee: " << TTF_GetError() << endl;
    }
    
    // Sprites d'actions (racine data/, relies aux titres des fichiers)
    spriteIdle = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/idle.png", "data/sprite final/joueur/run.png"});

    spriteAttaqueHaut[0] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attackup1.png"});
    spriteAttaqueHaut[1] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attackup2.png"});

    spriteAttaqueBas[0] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attackdown.png"});
    spriteAttaqueBas[1] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attackdown2.png"});
    
    spriteAttaqueGauche[0] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attack1.png"});
    spriteAttaqueGauche[1] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attack2.png"});
    
    spriteEffetAttaque = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/joueur/attaque/SlashFX 5A v2_4.png", "data/sprite final/joueur/attaque/SlashFX 5A v3_3.png"});
    spriteEnnemi = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Torch_Red_0_0.png",
        
    });

    spriteBoss = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Decorations/outpout/boss.png",
        "data/sprite final/Decorations/outpout/boss.png"
    });

    spriteCoffre = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Decorations/outpout/G_Idle.png",
        "data/sprite final/Decorations/outpout/G_Idle.png"
    });

    spritePnj = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Decorations/outpout/pnj.png",
        "data/sprite final/Decorations/outpout/pnj.png"
    });



    spriteMort = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/img/heros/Dead/dead_0_0.png"
    });

    tilemapTopDown = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Tileset/Tilemap_color2.png",
        "data/sprite final/Tileset/Tilemap_color1.png",
        "data/sprite final/Tileset/Tilemap_color5.png"
    });

    tilemapFlat = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Ground/Tilemap_Flat.png",
        "data/img/pour plus tard/Terrain/Ground/Tilemap_Flat.png"
    });

    tilemapElevation = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Ground/Tilemap_Elevation.png",
        "data/img/pour plus tard/Terrain/Ground/Tilemap_Elevation.png"
    });

    tilemapShadows = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Tileset/Shadow.png",
        "data/sprite final/Ground/Shadows.png",
        "data/img/pour plus tard/Terrain/Ground/Shadows.png"
    });

    

    rockWall = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Decorations/Rocks/Rock4.png"
    });

    houseBlue = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {
        "data/sprite final/Decorations/House_Blue.png"
    });

    tilesTerrainEau[0] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_sup_G.png"});
    tilesTerrainEau[1] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/haut_sup.png"});
    tilesTerrainEau[2] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_sup_D.png"});
    tilesTerrainEau[3] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/gauche.png"});
    tilesTerrainEau[4] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/milieu.png"});
    tilesTerrainEau[5] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/droit.png"});
    tilesTerrainEau[6] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_inf_G.png"});
    tilesTerrainEau[7] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/bas_inf.png"});
    tilesTerrainEau[8] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_inf_D.png"});

    tilesTerrain[0] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_sup_G_eau.png"});
    tilesTerrain[1] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/haut_sup_eau.png"});
    tilesTerrain[2] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_sup_D_eau.png"});
    tilesTerrain[3] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/gauche_eau.png"});
    tilesTerrain[4] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/milieu_eau.png"});
    tilesTerrain[5] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/droit_eau.png"});
    tilesTerrain[6] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_inf_G_eau.png"});
    tilesTerrain[7] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/bas_inf_eau.png"});
    tilesTerrain[8] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite final/tiles_terrain/coin_inf_D_eau.png"});

    // Set alternatif utilise hors village (donjon).
    tilesTerrainDonjon[0] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/coin_g_haut.png"});
    tilesTerrainDonjon[1] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/haut_milieu.png"});
    tilesTerrainDonjon[2] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/coin_d_haut.png"});
    tilesTerrainDonjon[3] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/cote_gauche.png"});
    tilesTerrainDonjon[4] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/milieu.png"});
    tilesTerrainDonjon[5] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/cote_droit.png"});
    tilesTerrainDonjon[6] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/coin_g_bas.png"});
    tilesTerrainDonjon[7] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/bas_milieu.png"});
    tilesTerrainDonjon[8] = AffichageSDLUtils::chargerTextureOptionnelle(renderer, {"data/sprite_donjon/coin_d_bas.png"});

    if (spriteIdle == nullptr &&  spriteAttaqueGauche[0] == nullptr) {
        cout << "Aucun sprite d'action trouve dans data/*.png, fallback sur l'ancien format." << endl;
    }

    directionJoueur = DIR_BAS;
    dernierTickMouvement = 0;
    dernierTickAttaque = 0;
    dernierTickActionDeplacement = 0;
    dernierTickActionAttaque = 0;
    varianteAttaque = 0;
    // Conserve la logique en grille 32x32 et adapte le rendu de base a 32 px.
    zoomNiveau = 32.0f / static_cast<float>(TAILLE_CASE);
    echelleJoueur = 3.0f;
    echelleEnnemi = 3.0f;
    animationMortActive = false;
    tickDebutAnimationMort = 0;
}

// Destructeur pour libérer les ressources
AffichageSDL::~AffichageSDL() {
    for (HudTextCacheEntry& entree : hudTextCache) {
        if (entree.texture != nullptr) {
            SDL_DestroyTexture(entree.texture);
            entree.texture = nullptr;
        }
    }

    if (fondMenu != nullptr) SDL_DestroyTexture(fondMenu);
    if (spriteIdle != nullptr) SDL_DestroyTexture(spriteIdle);
    for (SDL_Texture* sprite : spriteAttaqueBas) {
        if (sprite != nullptr) SDL_DestroyTexture(sprite);
    }
    for (SDL_Texture* sprite : spriteAttaqueHaut) {
        if (sprite != nullptr) SDL_DestroyTexture(sprite);
    }
    for (SDL_Texture* sprite : spriteAttaqueGauche) {
        if (sprite != nullptr) SDL_DestroyTexture(sprite);
    }
    if (spriteEffetAttaque != nullptr) SDL_DestroyTexture(spriteEffetAttaque);
    if (spriteEnnemi != nullptr) SDL_DestroyTexture(spriteEnnemi);
    if (spriteBoss != nullptr) SDL_DestroyTexture(spriteBoss);
    if (spriteCoffre != nullptr) SDL_DestroyTexture(spriteCoffre);
    if (spritePnj != nullptr) SDL_DestroyTexture(spritePnj);
    if (spriteMort != nullptr) SDL_DestroyTexture(spriteMort);
    if (decorWaterFoam != nullptr) SDL_DestroyTexture(decorWaterFoam);
    if (decorRockSmall != nullptr) SDL_DestroyTexture(decorRockSmall);
    if (decorRockLarge != nullptr) SDL_DestroyTexture(decorRockLarge);
    if (rockWall != nullptr) SDL_DestroyTexture(rockWall);
    if (houseBlue != nullptr) SDL_DestroyTexture(houseBlue);
    if (tilemapTopDown != nullptr) SDL_DestroyTexture(tilemapTopDown);
    if (tilemapFlat != nullptr) SDL_DestroyTexture(tilemapFlat);
    if (tilemapElevation != nullptr) SDL_DestroyTexture(tilemapElevation);
    if (tilemapShadows != nullptr) SDL_DestroyTexture(tilemapShadows);
    for (SDL_Texture* tuile : tilesTerrain) {
        if (tuile != nullptr) SDL_DestroyTexture(tuile);
    }
    for (SDL_Texture* tuile : tilesTerrainEau) {
        if (tuile != nullptr) SDL_DestroyTexture(tuile);
    }
    for (SDL_Texture* tuile : tilesTerrainDonjon) {
        if (tuile != nullptr) SDL_DestroyTexture(tuile);
    }
    if (spriteJoueur != nullptr) SDL_DestroyTexture(spriteJoueur);
    if (fontTitre != nullptr) TTF_CloseFont(fontTitre);
    if (fontMenu != nullptr) TTF_CloseFont(fontMenu);
    if (fontJeu != nullptr) TTF_CloseFont(fontJeu);
    if (renderer != nullptr) SDL_DestroyRenderer(renderer);
    if (window != nullptr) SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

// Le menu et l'ecran de fin sont implementes dans MenuSDL.cpp.

int AffichageSDL::tailleCaseAffichee() const {
    int taille = static_cast<int>(TAILLE_CASE * zoomNiveau);
    if (taille < 8) {
        taille = 8;
    }
    return taille;
}

void AffichageSDL::calculerCamera(int joueurX, int joueurY, int tailleCase, int& cameraX, int& cameraY) const {
    const int largeurCarte = 25 * tailleCase;
    const int hauteurCarte = 20 * tailleCase;

    cameraX = (LARGEUR_FENETRE / 2) - (joueurX * tailleCase + tailleCase / 2);
    cameraY = (HAUTEUR_FENETRE / 2) - (joueurY * tailleCase + tailleCase / 2);

    if (largeurCarte <= LARGEUR_FENETRE) {
        cameraX = (LARGEUR_FENETRE - largeurCarte) / 2;
    }
    else {
        cameraX = std::max(LARGEUR_FENETRE - largeurCarte, std::min(0, cameraX));
    }

    if (hauteurCarte <= HAUTEUR_FENETRE) {
        cameraY = (HAUTEUR_FENETRE - hauteurCarte) / 2;
    }
    else {
        cameraY = std::max(HAUTEUR_FENETRE - hauteurCarte, std::min(0, cameraY));
    }
}

void AffichageSDL::setEchelleEnnemi(float echelle) {
    // Limite la taille pour eviter des rendus extremes.
    echelleEnnemi = std::max(0.6f, std::min(3.0f, echelle));
}

void AffichageSDL::setEchelleJoueur(float echelle) {
    // Limite la taille pour eviter des rendus extremes.
    echelleJoueur = std::max(0.8f, std::min(3.0f, echelle));
}
