#include "Zone.h"
#include "Ennemi.h"
#include "Pnj.h"
#include "attaque.h"

#include <fstream>
#include <string>
#include <type_traits>
#include<vector>
#include<iostream>
using namespace std;

Zone::Zone() {

    nom = "zoneName";
    z_haut = -1;
    z_bas = -1;
    z_droite = -1;
    z_gauche = -1;
    Coffre c;
    coffres.push_back(c);
    num_terrain = "1";
    charger_terrain(num_terrain);

}

Zone::Zone(string num_ter) {

    nom = "zoneName";
    //Ennemi e;
    //e.setPositionX(224);
    //e.setPositionY(224);
    z_haut = -1;
    z_bas = -1;
    z_droite = -1;
    z_gauche = -1;
    //ennemis.push_back(e);
    Coffre c;
    coffres.push_back(c);
    num_terrain = num_ter;
    charger_terrain(num_terrain);
    

}
Zone::Zone(string nb_porte , string num_ter , string dir) {

    nom = "zoneName";
    Ennemi e;
    e.setPositionX(224);
    e.setPositionY(224);
    z_haut = -1;
    z_bas = -1;
    z_droite = -1;
    z_gauche = -1;
    ennemis.push_back(e);
    Coffre c;
    coffres.push_back(c);
    num_terrain = num_ter;
    charger_terrain_selon_nb_porte_selon_dir(nb_porte , num_terrain ,dir );
    

}
char Zone::get_terrain_casse(int x , int y){
    if (x < 0 || x >= 20 || y < 0 || y >= 25) {
        return '-';
    }
    return Terrain[x][y];
}
//char[20][26] Zone::get_terrain(){
//    return Terrain;
//}

int Zone::get_nb_changement_de_zone_possible(){
    return  nb_changement_de_zone_possible;
}
void Zone::set_nb_changement_de_zone_possible(int n){
    nb_changement_de_zone_possible = n;
}
void Zone::set_z_max(int z){
    z_max = z;
}

int Zone::get_z_max(){
    return z_max;
}

void Zone::charger_terrain(string num){
    std::ifstream fichier("data/Terrain/Zone" + num  + ".txt");
    if (fichier.is_open()) {
        //std::getline(fichier, nom);
        //fichier 
        int c = 0;
        string lin;
        for (std::string line; std::getline(fichier, line);){
            set_line_terrain(c,0 , line);
            c += 1; 
        }/*
        while (c <= 25){
            for (int k = 0 ; k < 20 ; k++){
            set_line_terrain(k,c , lin[26]);
            }
            c += 1 ;}
           */ 
        fichier.close();
        
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier pour le chargement de la zone." << std::endl;
    }

}
void Zone::charger_terrain_selon_nb_porte_selon_dir(string num_porte , string num , string dir ){
    std::ifstream fichier("data/Terrain/"+ num_porte + "_Porte" + dir + "/Zone" + num  + ".txt");
    if (fichier.is_open()) {
        //std::getline(fichier, nom);
        //fichier 
        int c = 0;
        string lin;
        for (std::string line; std::getline(fichier, line);){
            set_line_terrain(c,0 , line);
            c += 1; 
        }/*
        while (c <= 25){
            for (int k = 0 ; k < 20 ; k++){
            set_line_terrain(k,c , lin[26]);
            }
            c += 1 ;}
           */ 
        fichier.close();
        
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier pour le chargement de la zone." << std::endl;
    }
}

void Zone::set_line_terrain(int l , int c , string li ){
    if (l < 0 || l >= 24) {
        return;
    }

    if (li.size() > 2){
    for (int c = 0 ; c < 25 ; c++){
    if (c < static_cast<int>(li.size())) {
    Terrain[l][c] = li[c];
    } else {
    Terrain[l][c] = ' ';
    }
    }
    }else {
    if (li == "-2") {
    if (l == 20){
    z_haut = -2;
    }
    else if (l == 21) {
    z_bas = -2;
    
    }
    else if (l == 22) {
    z_droite = -2;
    }
    else if (l == 23) {
    z_gauche = -2;
    }
    }
    //nb_changement_de_zone_possible = li[0]; 
    }
}
int Zone::get_Zone_Gauche_Number(){
    return z_gauche;
}
int Zone::get_Zone_Droite_Number(){
    return z_droite;
}
int Zone::get_Zone_Basse_Number(){
    return z_bas;
}
int Zone::get_Zone_Haute_Number(){
    return z_haut;
} 
void Zone::set_Zone_Gauche_Number(int n ){
    z_gauche =n;
}
void Zone::set_Zone_Droite_Number(int n){
    z_droite = n;
}
void Zone::set_Zone_Basse_Number(int n){
    z_bas = n;
}
void Zone::set_Zone_Haute_Number(int n){
    z_haut = n;
}
void Zone::explorer(){}

//Fonction qui recupere la position des enemie dans un tableau a deux dimensions

vector<vector<int>> Zone::getPosition() {
    vector<vector<int>> positions;
    for (const auto& ennemi : ennemis) {
        vector<int> pos = {ennemi.getPositionX(), ennemi.getPositionY()};
        positions.push_back(pos);
    }
    return positions;
}

vector<Attaque> & Zone::getAttaques(){
return  attaques;

}

vector<Ennemi>  & Zone::getEnnemi(){

    return ennemis;
}
Ennemi  & Zone::get_first_Ennemi(){
    return  ennemis[0];

}

void Zone::move_ennemi_l(int i){
ennemis[i].gauche();

}

void Zone::move_ennemi_r(int i){
ennemis[i].droite();

}

void Zone::move_ennemi_u(int i){
ennemis[i].haut();

}

void Zone::move_ennemi_d(int i ){
ennemis[i].bas();

}


const vector<Ennemi>& Zone::getEnnemi() const {
    return ennemis;
}

string Zone::getNumTerrain() const {
    return num_terrain;
}


vector<Pnj>& Zone::getPNJ() {
    return pnjs;
}

vector<Coffre>& Zone::getCoffres() {
    return coffres;
}

const vector<Coffre>& Zone::getCoffres() const {
    return coffres;
}
