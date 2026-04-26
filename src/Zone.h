#ifndef ZONE_H
#define ZONE_H

#include "Ennemi.h"
#include "Pnj.h"
#include "Coffre.h"
#include "attaque.h"

#include <string>
#include<vector>

using namespace std;

class Zone {

    private:
        string nom;
        vector<Pnj> pnjs;
        vector<Ennemi> ennemis;
        vector<Coffre> coffres;
        vector<Attaque> attaques;
        int z_droite ;
        int z_gauche ;
        int z_bas ;
        int z_haut ;
        int z_max ;
        string num_terrain;
        //char Terrain[20][26];
        int nb_changement_de_zone_possible;
        
    public:
        void set_line_terrain(int l , int c , string li );    
        int get_nb_changement_de_zone_possible();
        void set_nb_changement_de_zone_possible(int n);
        
        Zone();
        char Terrain[20][26];
        Zone(string num);
        Zone(string nb_porte , string num , string dir);
        //char [20][26] get_terrain();
        void explorer();
        char get_terrain_casse(int x , int y );
        void set_z_max(int z);
        int get_z_max();
        int get_Zone_Gauche_Number();
        int get_Zone_Droite_Number();
        int get_Zone_Basse_Number();
        int get_Zone_Haute_Number();
        void set_Zone_Gauche_Number(int n);
        void set_Zone_Droite_Number(int n);
        void set_Zone_Haute_Number(int n);
        void set_Zone_Basse_Number(int n );
        vector<vector<int>> getPosition();
        vector<Ennemi>  & getEnnemi();
        vector<Attaque> & getAttaques();
        const vector<Ennemi>& getEnnemi() const;
        string getNumTerrain() const;
        void charger_terrain(string num);
        void charger_terrain_selon_nb_porte_selon_dir(string num_porte , string num , string dir);
        Ennemi & get_first_Ennemi();
        void move_ennemi_d(int i);
        void move_ennemi_u(int i);
        void move_ennemi_l(int i);
        void move_ennemi_r(int i);
        vector<Pnj> & getPNJ() ;
        vector<Coffre>& getCoffres();
        const vector<Coffre>& getCoffres() const;
            
};
#endif
