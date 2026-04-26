#include <cstdlib>
#include <ctime>
#include <iostream>
#include <exception>
#include "AffichageSDL.h"
#include "ContexteJeu.h"
#include "Sauvegarde.h"

using namespace std;

// Point d'entree SDL: orchestration menu principal -> creation contexte -> boucle de jeu.


int main(){
    try {

        

        AffichageSDL affichage;
        bool enCours = true;

        while (enCours) {
            int choixMenu = affichage.afficherMenu();

            // 0 = Jouer
            // 1 = Charger une partie
            // 2 = Quitter

            // ----- PARTIE MENU PRINCIPAL -----
            if (choixMenu == 2) {
                cout << "Fermeture depuis le menu principal." << endl;
                return 0;
            }

            // ----- PARTIE JEU -----

            ContexteJeu contexte;

            if (choixMenu == 0) {
                const string nomJoueur = affichage.demanderNomJoueur();
                if (nomJoueur.empty()) {
                    continue;
                }
                contexte.modifierJoueur(nomJoueur);
            }
            else if (choixMenu == 1) {
                string cheminSauvegarde;
                if (!affichage.afficherMenuChargement(cheminSauvegarde)) {
                    continue;
                }
                contexte.chargerPartie(cheminSauvegarde);
            }
            int actionFinJeu = affichage.afficherJeu(contexte);

            if (actionFinJeu == 1) {
                    enCours = false;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Erreur fatale SDL: " << e.what() << endl;
        return 1;
    }

    return 0;

}