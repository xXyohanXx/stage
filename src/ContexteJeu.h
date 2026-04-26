/**
 * @file ContexteJeu.h
 * @brief Etat global de la partie et logique de haut niveau du gameplay.
 */

#ifndef CONTEXTEJEU_H
#define CONTEXTEJEU_H

#include "Ennemi.h"
#include "joueur.h"
#include "Niveau.h"
#include "Inventaire.h"
#include "attaque.h"
#include "Sauvegarde.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @class ContexteJeu
 * @brief Centralise joueur, niveau, inventaire, score et sauvegarde.
 */
class ContexteJeu {

    private:
        Joueur joueur;
        //Niveau niveau[4];
        vector<Niveau> niveau;
        //Inventaire inventaire; // probléme de destructeur 
        int zone_actuel;
        int niveau_actuel;
        int ennemis_tues;
        int objets_recuperes;
        int score;

        Sauvegarde sauvegarde;
        Inventaire inventaire; 
        //string dialogue_actuel;
        string dialogue_actuel;
        bool dans_village;
        bool boss_vaincu;

        

    public:
        
        /** @brief Initialise une nouvelle partie. */
        ContexteJeu();

        /** @brief Met a jour le deplacement des ennemis de la zone courante. */
        void deplacement_ennemis();
        void deplacement_slime(Ennemi &en);

        /** @brief Change le nom du joueur courant. */
        void modifierJoueur(string name);

        /** @brief Retourne une reference sur le niveau demande. */
        Niveau &recupereNiveau(int n);

        /** @brief Retourne une reference sur le joueur courant. */
        Joueur &recupereJoueur();

        /** @brief Traite une action clavier (deplacement, attaque, interaction). */
        void actionClavier(char c);
        int get_Zone_Number();
        string get_dialoge();
        void set_Zone_Number(int n);
        int get_Actual_Level();
        void set_Actual_Level(int n);
        int get_Nombre_Ennemis_Tues() const;
        void incrementer_Ennemis_Tues();
        bool verifPV(Entite& cible);
        void deplacement_boss(Ennemi &en);

        int get_Objets_Recuperes() const;
        void incrementer_Objets_Recuperes();

        int getScore() const;
        void calculerScore();
         bool estDansVillage() const;
        
        //methode pour sauvegarder la partie en cours
        void sauvegarderPartie(const string& nom_fichier);
        void chargerPartie(const string& nom_fichier);

        vector<Objet> getInventaire();
        void setScore(int s);
        bool estBossVaincu() const;
};

#endif