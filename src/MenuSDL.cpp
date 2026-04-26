#include "AffichageSDL.h"
#include "AffichageSDLUtils.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <vector>

using namespace std;

// Gestion des ecrans UI SDL: menu principal, chargement, pause, game over et victoire.

namespace {

const string DOSSIER_SAUVEGARDE = "data/sauvegarde";
const size_t LIMITE_SAUVEGARDES = 5;

vector<string> listerSauvegardesDisponibles() {
    namespace fs = std::filesystem;
    vector<string> fichiers;

    std::error_code ec;
    if (!fs::exists(DOSSIER_SAUVEGARDE, ec)) {
        return fichiers;
    }

    for (const fs::directory_entry& entree : fs::directory_iterator(DOSSIER_SAUVEGARDE, ec)) {
        if (ec) {
            break;
        }

        if (!entree.is_regular_file()) {
            continue;
        }

        const fs::path& chemin = entree.path();
        if (chemin.extension() == ".txt") {
            fichiers.push_back(chemin.string());
        }
    }

    std::sort(fichiers.begin(), fichiers.end());
    return fichiers;
}

string nomSauvegardeDepuisChemin(const string& cheminComplet) {
    const std::filesystem::path p(cheminComplet);
    return p.filename().string();
}

}

// Boucle du menu principal avec navigation clavier.
int AffichageSDL::afficherMenu() {
    const vector<string> options = {"Jouer", "Charger une partie", "Quitter"};
    int selection = 0;
    bool enCours = true;

    while (enCours) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 1;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                    selection = (selection - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
                }
                else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                    selection = (selection + 1) % static_cast<int>(options.size());
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    return selection;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return 1;
                }
            }
        }

        if (fondMenu != nullptr) {
            SDL_RenderCopy(renderer, fondMenu, nullptr, nullptr);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 15, 18, 30, 255);
            SDL_RenderClear(renderer);
        }

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color jaune = {255, 215, 0, 255};
        SDL_Color ambre = {255, 190, 0, 255};

        if (fontTitre != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontTitre, "CASTLE MAZE", 100, 100, ambre);
        }

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            SDL_Color couleur = (i == selection) ? jaune : blanc;
            if (fontMenu != nullptr) {
                AffichageSDLUtils::ombrage_texte(renderer, fontMenu, options[i].c_str(), 0, 290 + i * 60, couleur);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    return 1;
}

bool AffichageSDL::afficherMenuChargement(string& cheminSauvegarde) {
    cheminSauvegarde.clear();
    int selection = 0;
    int offsetAffichage = 0;
    string message = "Entree: charger  Suppr: supprimer  Echap: retour";

    while (true) {
        vector<string> sauvegardes = listerSauvegardesDisponibles();
        const int nbSauvegardes = static_cast<int>(sauvegardes.size());

        if (nbSauvegardes == 0) {
            selection = 0;
            offsetAffichage = 0;
        }
        else {
            if (selection >= nbSauvegardes) {
                selection = nbSauvegardes - 1;
            }
            if (selection < 0) {
                selection = 0;
            }
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }

            if (event.type != SDL_KEYDOWN) {
                continue;
            }

            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }

            if (nbSauvegardes > 0 && (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z)) {
                selection = (selection - 1 + nbSauvegardes) % nbSauvegardes;
            }
            else if (nbSauvegardes > 0 && (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)) {
                selection = (selection + 1) % nbSauvegardes;
            }
            else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                if (nbSauvegardes == 0) {
                    message = "Aucune sauvegarde disponible.";
                }
                else {
                    cheminSauvegarde = sauvegardes[selection];
                    return true;
                }
            }
            else if (nbSauvegardes > 0 && (event.key.keysym.sym == SDLK_DELETE || event.key.keysym.sym == SDLK_BACKSPACE)) {
                const string nomAffiche = nomSauvegardeDepuisChemin(sauvegardes[selection]);
                if (afficherAlerteConfirmation("Supprimer " + nomAffiche + " ?")) {
                    std::error_code ec;
                    const bool supprime = std::filesystem::remove(sauvegardes[selection], ec);
                    if (supprime && !ec) {
                        message = "Sauvegarde supprimee: " + nomAffiche;
                        if (selection > 0) {
                            selection -= 1;
                        }
                    }
                    else {
                        message = "Erreur de suppression.";
                    }
                }
            }
        }

        if (fondMenu != nullptr) {
            SDL_RenderCopy(renderer, fondMenu, nullptr, nullptr);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 15, 18, 30, 255);
            SDL_RenderClear(renderer);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect panneau = {70, 90, 660, 430};
        SDL_RenderFillRect(renderer, &panneau);

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color jaune = {255, 215, 0, 255};
        SDL_Color ambre = {255, 190, 0, 255};

        if (fontTitre != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontTitre, "CHARGER", 120, 40, ambre);
        }

        string compteur = "Sauvegardes: " + to_string(nbSauvegardes) + " / " + to_string(LIMITE_SAUVEGARDES);
        if (fontJeu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontJeu, compteur.c_str(), 100, 120, blanc);
            AffichageSDLUtils::ombrage_texte(renderer, fontJeu, message.c_str(), 100, 470, blanc);
        }

        const int maxLignes = 6;
        if (selection < offsetAffichage) {
            offsetAffichage = selection;
        }
        if (selection >= offsetAffichage + maxLignes) {
            offsetAffichage = selection - maxLignes + 1;
        }

        for (int i = 0; i < maxLignes; ++i) {
            const int index = offsetAffichage + i;
            if (index >= nbSauvegardes) {
                break;
            }

            const string nomAffiche = nomSauvegardeDepuisChemin(sauvegardes[index]);
            SDL_Color couleur = (index == selection) ? jaune : blanc;
            const string ligne = (index == selection ? "> " : "  ") + nomAffiche;

            if (fontMenu != nullptr) {
                AffichageSDLUtils::ombrage_texte(renderer, fontMenu, ligne.c_str(), 100, 165 + i * 50, couleur);
            }
        }

        if (nbSauvegardes == 0 && fontMenu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontMenu, "Aucune sauvegarde", 100, 220, blanc);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

string AffichageSDL::demanderNomJoueur() {
    string nomJoueur;
    bool enCours = true;
    Uint32 tickCurseur = SDL_GetTicks();
    bool curseurVisible = true;

    SDL_StartTextInput();

    while (enCours) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_StopTextInput();
                return "";
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !nomJoueur.empty()) {
                    nomJoueur.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    SDL_StopTextInput();
                    if (nomJoueur.empty()) {
                        return "Joueur";
                    }
                    return nomJoueur;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_StopTextInput();
                    return "";
                }
            }
            else if (event.type == SDL_TEXTINPUT) {
                if (nomJoueur.size() < 16) {
                    for (int i = 0; event.text.text[i] != '\0'; ++i) {
                        const unsigned char c = static_cast<unsigned char>(event.text.text[i]);
                        if (std::isalnum(c) || c == ' ' || c == '_' || c == '-') {
                            nomJoueur.push_back(static_cast<char>(c));
                            if (nomJoueur.size() >= 16) {
                                break;
                            }
                        }
                    }
                }
            }
        }

        if ((SDL_GetTicks() - tickCurseur) > 450) {
            tickCurseur = SDL_GetTicks();
            curseurVisible = !curseurVisible;
        }

        if (fondMenu != nullptr) {
            SDL_RenderCopy(renderer, fondMenu, nullptr, nullptr);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 15, 18, 30, 255);
            SDL_RenderClear(renderer);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
        SDL_Rect panneau = {100, 170, 600, 260};
        SDL_RenderFillRect(renderer, &panneau);

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color jaune = {255, 215, 0, 255};
        SDL_Color ambre = {255, 190, 0, 255};

        if (fontTitre != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontTitre, "NOM", 260, 70, ambre);
        }

        if (fontMenu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontMenu, "Entrez votre nom", 130, 200, blanc);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 220);
        SDL_Rect champ = {140, 280, 520, 68};
        SDL_RenderDrawRect(renderer, &champ);

        string texteAffiche = nomJoueur;
        if (texteAffiche.empty()) {
            texteAffiche = "Joueur";
        }

        if (curseurVisible) {
            texteAffiche += "_";
        }

        if (fontMenu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontMenu, texteAffiche.c_str(), 160, 290, jaune);
        }

        if (fontJeu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontJeu, "Entree: valider  -  Echap: retour", 180, 370, blanc);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_StopTextInput();
    return "";
}

// Affiche le menu de fin de partie et retourne le choix utilisateur.
int AffichageSDL::afficherGameOver() {
    const vector<string> options = {"Rejouer", "Retour menu principal"};
    int selection = 0;

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return -1;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                    selection = (selection - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
                }
                else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                    selection = (selection + 1) % static_cast<int>(options.size());
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    return selection;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return 1;
                }
            }
        }

        // Overlay translucide pour laisser le decor visible derriere le menu Game Over.
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        SDL_Rect fullScreen = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color rouge = {220, 50, 50, 255};
        SDL_Color jaune = {255, 215, 0, 255};

        if (fontTitre != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontTitre, "GAME OVER", 90, 110, rouge);
        }

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            SDL_Color couleur = (i == selection) ? jaune : blanc;
            if (fontMenu != nullptr) {
                AffichageSDLUtils::ombrage_texte(renderer, fontMenu, options[i].c_str(), 120, 290 + i * 70, couleur);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

// Affiche l'ecran de victoire quand le boss est vaincu.
int AffichageSDL::afficherVictoire() {
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return -1;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_ESCAPE) {
                    return 0;
                }
            }
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 165);
        SDL_Rect fullScreen = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color orFonce = {255, 190, 0, 255};
        SDL_Color jaune = {255, 215, 0, 255};

        if (fontTitre != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontTitre, "VICTOIRE", 130, 120, orFonce);
        }
        if (fontMenu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontMenu, "Boss vaincu !", 190, 280, jaune);
        }
        if (fontJeu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontJeu, "Entree ou Echap pour retourner au menu principal", 120, 410, blanc);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

// Menu pause disponible pendant la partie.
int AffichageSDL::afficherMenuPause(bool& sauvegarde) {
    const vector<string> options = {"Reprendre", "Sauvegarder", "Menu principal", "Quitter le jeu"};
    int selection = 0;
    bool validationMenu = false;

    while (!validationMenu) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return -1;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                    selection = (selection - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
                }
                else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                    selection = (selection + 1) % static_cast<int>(options.size());
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    validationMenu = true;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 15, 18, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect fullScreen = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color jaune = {255, 215, 0, 255};
        SDL_Color ambre = {255, 190, 0, 255};

        if (fontTitre != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontTitre, "PAUSE", 250, 100, ambre);
        }

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            SDL_Color couleur = (i == selection) ? jaune : blanc;
            if (fontMenu != nullptr) {
                AffichageSDLUtils::ombrage_texte(renderer, fontMenu, options[i].c_str(), 100, 250 + i * 70, couleur);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return selection;
}

// Boite de dialogue de confirmation reutilisable.
bool AffichageSDL::afficherAlerteConfirmation(const string& message) {
    const vector<string> options = {"Oui", "Non"};
    int selection = 0;
    bool validation = false;

    while (!validation) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_q) {
                    selection = 0;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                    selection = 1;
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    validation = true;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 15, 18, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
        SDL_Rect fullScreen = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_Color blanc = {240, 240, 240, 255};
        SDL_Color jaune = {255, 215, 0, 255};
        SDL_Color rouge = {255, 50, 50, 255};

        if (fontJeu != nullptr) {
            AffichageSDLUtils::ombrage_texte(renderer, fontJeu, message.c_str(), 150, 200, rouge);
        }

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            SDL_Color couleur = (i == selection) ? jaune : blanc;
            if (fontMenu != nullptr) {
                AffichageSDLUtils::ombrage_texte(renderer, fontMenu, options[i].c_str(), 250 + i * 150, 350, couleur);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return selection == 0;
}
