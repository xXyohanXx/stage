/**
 * @file AffichageSDL.h
 * @brief Interface SDL principale pour l'affichage du jeu Castle Maze.
 *
 * Cette classe centralise la fenetre SDL, le renderer, le chargement des assets,
 * et toutes les vues du jeu (menus, jeu en cours, game over, victoire, HUD).
 */

#ifndef _AFFICHAGESDL_H_
#define _AFFICHAGESDL_H_
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <array>
#include <string>
#include <vector>
using namespace std;

class Ennemi;
class Joueur;
class Pnj;
class Coffre;

/**
 * @class AffichageSDL
 * @brief Couche d'affichage SDL (menus + rendu jeu) de Castle Maze.
 *
 * Cette classe ne contient pas la logique metier du gameplay.
 * Elle affiche l'etat fourni par le contexte de jeu et transforme
 * les entrees clavier en actions de haut niveau (deplacement, attaque, menus).
 */
class AffichageSDL {
    private:

        struct HudTextCacheEntry {
            SDL_Texture* texture;
            string texte;
            SDL_Color couleur;
            int largeur;
            int hauteur;

            HudTextCacheEntry()
                : texture(nullptr), texte(""), couleur({0, 0, 0, 255}), largeur(0), hauteur(0) {}
        };

        enum DirectionJoueur {
            DIR_HAUT,
            DIR_BAS,
            DIR_GAUCHE,
            DIR_DROITE
        };

        // Attributs pour la fenêtre, le rendu, les textures et les polices

        SDL_Window* window;// Fenêtre principale

        SDL_Renderer* renderer;// Rendu pour dessiner dans la fenêtre

        SDL_Texture* fondMenu;// Texture pour le fond du menu

        SDL_Texture* spriteJoueur;// Sprite sheet du joueur

        SDL_Texture* spriteIdle;
        array<SDL_Texture*, 2> spriteAttaqueBas;
        array<SDL_Texture*, 2> spriteAttaqueHaut;
        array<SDL_Texture*, 2> spriteAttaqueGauche;
        SDL_Texture* spriteEffetAttaque;
        SDL_Texture* spriteEnnemi;
        SDL_Texture* spriteBoss;
        SDL_Texture* spriteCoffre;
        SDL_Texture* spritePnj;
        SDL_Texture* spriteMort;
        SDL_Texture* decorWaterFoam;
        SDL_Texture* decorRockSmall;
        SDL_Texture* decorRockLarge;
        SDL_Texture* rockWall;
        SDL_Texture* houseBlue;
        SDL_Texture* tilemapTopDown;
        SDL_Texture* tilemapFlat;
        SDL_Texture* tilemapElevation;
        SDL_Texture* tilemapShadows;
        array<SDL_Texture*, 9> tilesTerrain;
        array<SDL_Texture*, 9> tilesTerrainEau;
        array<SDL_Texture*, 9> tilesTerrainDonjon;

        TTF_Font* fontTitre;// Police pour le titre

        TTF_Font* fontMenu;// Police pour le menu

        TTF_Font* fontJeu;// Police pour le texte du jeu (stats, messages, etc.)

        DirectionJoueur directionJoueur;// Derniere direction entree

        Uint32 dernierTickMouvement;// Temps du dernier mouvement
        Uint32 dernierTickAttaque;// Temps de la derniere attaque
        Uint32 dernierTickActionDeplacement;// Cooldown entre deux deplacements
        Uint32 dernierTickActionAttaque;// Cooldown entre deux attaques
        int varianteAttaque;// Alterne entre les sprites d'attaque 1 et 2
        float zoomNiveau;
        float echelleJoueur;
        float echelleEnnemi;
        bool animationMortActive;
        Uint32 tickDebutAnimationMort;
        array<HudTextCacheEntry, 5> hudTextCache;
        
        
        // Constantes pour l'affichage du terrain et des éléments
        static const int TAILLE_CASE = 24;
        static const int LARGEUR_FENETRE = 800;
        static const int HAUTEUR_FENETRE = 600;
        static const int TERRAIN_X = 50;
        static const int TERRAIN_Y = 80;

        // Méthodes pour l'affichage des éléments du jeu :
        
        void afficherTerrain(const char terrain[20][26], int cameraX, int cameraY, bool dansVillage, int zoneNumber = 0, bool dessinerFond = true);//
        
        void afficherJoueur(const Joueur& joueur, int cameraX, int cameraY);//
        void afficherAnimationMort(const Joueur& joueur, int cameraX, int cameraY);//

        void afficherEnnemis(const vector<Ennemi>& ennemis, int cameraX, int cameraY, bool afficherBoss = true);//
        
        void afficherPNJ(const vector<class Pnj>& pnjs, int cameraX, int cameraY);//
        
        void afficherCoffres(const vector<class Coffre>& coffres, int cameraX, int cameraY);//
        
        void afficherDialogue(const std::string& dialogue);//
        
        void afficherStats(int pv, int maxPv, int attaque, int defense, const string& nomJoueur, int ennemisTues);//
        
        //méthode pour le menu de pause :   
        int afficherMenuPause(bool& sauvegarde);//
        
        bool afficherAlerteConfirmation(const string& message);//

        void enregistrerDirectionDepuisTouche(SDL_Keycode key);
        int tailleCaseAffichee() const;
        void calculerCamera(int joueurX, int joueurY, int tailleCase, int& cameraX, int& cameraY) const;

    public:

        /** @brief Initialise SDL, les polices et les textures du jeu. */
        AffichageSDL();
    
        /** @brief Libere toutes les ressources SDL allouees. */
        ~AffichageSDL();
    
        /** @brief Affiche le menu principal et retourne le choix utilisateur. */
        int afficherMenu();

        /** @brief Affiche le menu de chargement et renvoie le chemin de sauvegarde choisi. */
        bool afficherMenuChargement(string& cheminSauvegarde);

        /** @brief Ouvre l'ecran de saisie du nom du joueur. */
        string demanderNomJoueur();
    
        /** @brief Boucle de jeu SDL principale. */
        int afficherJeu(class ContexteJeu& contexte);

        // Permet d'ajuster la taille du joueur a l'affichage.
        void setEchelleJoueur(float echelle);

        // Permet d'ajuster la taille des ennemis a l'affichage.
        void setEchelleEnnemi(float echelle);
    
        /** @brief Affiche l'ecran de game over. */
        int afficherGameOver();

        /** @brief Affiche l'ecran de victoire quand le boss est vaincu. */
        int afficherVictoire();
        
        /** @brief Point d'entree de sauvegarde expose via l'affichage (legacy). */
        void sauvegarderPartie(class ContexteJeu& contexte);
    
        
};

#endif