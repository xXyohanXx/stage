#include "ContexteJeu.h"
#include "Ennemi.h"
#include "Inventaire.h"
#include "Niveau.h"
#include "Entite.h"
#include "Pnj.h"
#include "Zone.h"
#include "attaque.h"
#include "joueur.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include<string>
#include <vector>
using namespace std;

ContexteJeu::ContexteJeu() : joueur(), niveau()  , zone_actuel(0) , niveau_actuel(0), ennemis_tues(0), objets_recuperes(0), score(0),sauvegarde(), boss_vaincu(false) {
    Niveau n;
    niveau.push_back(n);
    dialogue_actuel = "";
    dans_village = true;
}

//méthode pour modifier le nom du joueur avec la méthode setNom de la classe Joueur, en utilisant le nom passé en paramètre. Cela permet de personnaliser le nom du joueur dans le contexte du jeu.
void ContexteJeu::modifierJoueur(string name) {
    joueur.setNom(name);
}
void ContexteJeu::deplacement_ennemis(){
for (int i = 0 ; i < recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi().size()   ; i++){
        switch (recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi()[i].getType()) {
            case 'S':
            deplacement_slime(recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi()[i]);
            break;
            case 'B':
            deplacement_boss(recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi()[i]);
            break;
        
        }

        /*Ennemi &ennemi = recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi()[i];
        ennemi.reduireCooldown();
        bool ennemiAGauche = ennemi.getPositionX() == joueur.getPositionX() - 32 && ennemi.getPositionY() == joueur.getPositionY();
        bool ennemiADroite = ennemi.getPositionX() == joueur.getPositionX() + 32 && ennemi.getPositionY() == joueur.getPositionY();
        bool ennemiAuDessus = ennemi.getPositionY() == joueur.getPositionY() - 32 && ennemi.getPositionX() == joueur.getPositionX();
        bool ennemiEnDessous = ennemi.getPositionY() == joueur.getPositionY() + 32 && ennemi.getPositionX() == joueur.getPositionX();

        if ((ennemiAGauche || ennemiADroite || ennemiAuDessus || ennemiEnDessous) && ennemi.peutAttaquer()) {
            ennemi.attaquer(joueur);
        }*/
		}

}

void ContexteJeu::deplacement_slime(Ennemi &en){
        int n ;
        n = rand() % 20;
        switch (n) {
        case 0:
            if ((recupereJoueur().getPositionX() == en.getPositionX() - 32)&&(recupereJoueur().getPositionY() == en.getPositionY())){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(en.getPositionY() / 32 , (en.getPositionX() - 32) / 32);
                if(c != '-' && c != 'M'){
                    en.gauche();
                }
            }
            break;
        
        case 1:
            if ((recupereJoueur().getPositionX() == en.getPositionX() + 32)&&(recupereJoueur().getPositionY() == en.getPositionY())){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(en.getPositionY() / 32 , (en.getPositionX() + 32) / 32);
                if(c != '-' && c != 'M'){
                    en.droite();
                }
            
            }
            break;
        case 2:
            if ((recupereJoueur().getPositionX() == en.getPositionX() )&&(recupereJoueur().getPositionY() == en.getPositionY()+ 32 )){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((en.getPositionY() + 32) / 32 , en.getPositionX() / 32);
                if(c != '-' && c != 'M'){
                    en.bas();
                }
            
            }
            break;
        case 3:
            if ((recupereJoueur().getPositionX() == en.getPositionX())&&(recupereJoueur().getPositionY() == en.getPositionY() - 32)){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((en.getPositionY() - 32) / 32 , en.getPositionX() / 32);
                if(c != '-' && c != 'M'){
                    en.haut();
                }
            
            }
            break;
        }

}
void ContexteJeu::deplacement_boss(Ennemi &en){
        int n;
        int n1 ;
        int n3;

        n3 = rand() % 3;
        if(n3 == 0){
        if (recupereJoueur().getPositionX() - en.getPositionX() > 0) n = 1;
        else n =0  ;

        if (recupereJoueur().getPositionY() - en.getPositionY() > 0) n1 = 2;
        else n1 = 3 ;

        switch (n) {
        case 0:
            if ((recupereJoueur().getPositionX() == en.getPositionX() - 32)&&(recupereJoueur().getPositionY() == en.getPositionY())){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(en.getPositionY() / 32 , (en.getPositionX() - 32) / 32);
                if(c != '-' && c != 'M'){
                    en.gauche();
                }
            }
            break;
        
        case 1:
            if ((recupereJoueur().getPositionX() == en.getPositionX() + 32)&&(recupereJoueur().getPositionY() == en.getPositionY())){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(en.getPositionY() / 32 , (en.getPositionX() + 32) / 32);
                if(c != '-' && c != 'M'){
                    en.droite();
                }
            
            }
            break;
        case 2:
            if ((recupereJoueur().getPositionX() == en.getPositionX() )&&(recupereJoueur().getPositionY() == en.getPositionY()+ 32 )){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((en.getPositionY() + 32) / 32 , en.getPositionX() / 32);
                if(c != '-' && c != 'M'){
                    en.bas();
                }
            
            }
            break;
        case 3:
            if ((recupereJoueur().getPositionX() == en.getPositionX())&&(recupereJoueur().getPositionY() == en.getPositionY() - 32)){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((en.getPositionY() - 32) / 32 , en.getPositionX() / 32);
                if(c != '-' && c != 'M'){
                    en.haut();
                }
            
            }
            break;}
        }



        //n = rand() % 20;
        switch (n1) {
        case 0:
            if ((recupereJoueur().getPositionX() == en.getPositionX() - 32)&&(recupereJoueur().getPositionY() == en.getPositionY())){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(en.getPositionY() / 32 , (en.getPositionX() - 32) / 32);
                if(c != '-' && c != 'M'){
                    en.gauche();
                }
            }
            break;
        
        case 1:
            if ((recupereJoueur().getPositionX() == en.getPositionX() + 32)&&(recupereJoueur().getPositionY() == en.getPositionY())){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(en.getPositionY() / 32 , (en.getPositionX() + 32) / 32);
                if(c != '-' && c != 'M'){
                    en.droite();
                }
            
            }
            break;
        case 2:
            if ((recupereJoueur().getPositionX() == en.getPositionX() )&&(recupereJoueur().getPositionY() == en.getPositionY()+ 32 )){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((en.getPositionY() + 32) / 32 , en.getPositionX() / 32);
                if(c != '-' && c != 'M'){
                    en.bas();
                }
            
            }
            break;
        case 3:
            if ((recupereJoueur().getPositionX() == en.getPositionX())&&(recupereJoueur().getPositionY() == en.getPositionY() - 32)){
            en.attaquer(recupereJoueur());
            }
            else {
                char c = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((en.getPositionY() - 32) / 32 , en.getPositionX() / 32);
                if(c != '-' && c != 'M'){
                    en.haut();
                }
            
            }
            break;
        }

}


Niveau &ContexteJeu::recupereNiveau(int n){
    return niveau[n];
    //return niveau;
}

Joueur& ContexteJeu::recupereJoueur(){
    return joueur;
} 
//void Deplacel
void ContexteJeu::actionClavier(char c){
    vector<Ennemi> & e = recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi();
    
            
            
    switch (c) {
        
        case 'q':
        dialogue_actuel = "";
            if(joueur.getPositionX() - 32 < 0){
                if (recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Gauche_Number() == -1 ){
                    /*Zone z(recupereNiveau(niveau_actuel).genZone());
                    z.set_Zone_Droite_Number(zone_actuel);
                    recupereNiveau(niveau_actuel).getZone(zone_actuel).set_Zone_Gauche_Number(recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level());
                    recupereNiveau(niveau_actuel).addZone(z);
                    zone_actuel = recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level() - 1;
                */}
                else if ((recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Gauche_Number() != -2 )) {
                    zone_actuel = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Gauche_Number();
                    joueur.setPositionX(joueur.getPositionX() + 800 - 32);
                    if (zone_actuel == 10){
                    dans_village = false;    
                    }
                }
                //joueur.setPositionX(joueur.getPositionX() + 800 - 32);
            }
            //setPositionX(getPositionX()-32);
            else { 
                    char cas = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((joueur.getPositionY() ) / 32 ,( joueur.getPositionX() - 32 )/ 32);
                    if( cas != '-' && cas != 'M' ){
                    
                    joueur.gauche();
                    joueur.SetOrientation(c);                  
                    }
            }
                   
            break;
        case 'd':
            dialogue_actuel = "";
            if(joueur.getPositionX() + 32  > 800 - 32){
                if (recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Droite_Number() == -1 ){
                    /*Zone z(recupereNiveau(niveau_actuel).genZone());
                    z.set_Zone_Gauche_Number(zone_actuel);
                    recupereNiveau(niveau_actuel).getZone(zone_actuel).set_Zone_Droite_Number(recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level());
                    recupereNiveau(niveau_actuel).addZone(z);
                    zone_actuel = recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level() - 1;
                */}
                else if ((recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Droite_Number() != -2 )) {
                    zone_actuel = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Droite_Number();
                    joueur.setPositionX(joueur.getPositionX() - 800 + 32);
                    if (zone_actuel == 10){
                    dans_village = false;    
                    }
                }
                //joueur.setPositionX(joueur.getPositionX() - 800 + 32);
            }
            //setPositionX(getPositionX()-32);
            else {
                    char cas = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((joueur.getPositionY() ) / 32 , ( joueur.getPositionX() + 32 ) / 32) ; 
                    
                    if(cas != '-' && cas != 'M'){
                    
                    joueur.droite();
                    joueur.SetOrientation(c);
                    }
                                   
            }
            break;
        case 'z':
            dialogue_actuel = "";
            if(joueur.getPositionY() - 32 < 0){
                if (recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Haute_Number() == -1 ){
                   /*Zone z(recupereNiveau(niveau_actuel).genZone());
                    z.set_Zone_Basse_Number(zone_actuel);
                    recupereNiveau(niveau_actuel).getZone(zone_actuel).set_Zone_Haute_Number(recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level());
                    recupereNiveau(niveau_actuel).addZone(z);
                    zone_actuel = recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level() - 1;
                */}
                else if ((recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Haute_Number() != -2 )) {
                    zone_actuel = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Haute_Number();
                    joueur.setPositionY(joueur.getPositionY() + 640 - 32);
                    if (zone_actuel == 10){
                    dans_village = false;    
                    }
                }
                //joueur.setPositionY(joueur.getPositionY() + 640 - 32);
            }
            //setPositionX(getPositionX()-32);
            else {  
                    char cas = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((joueur.getPositionY() - 32) / 32 , joueur.getPositionX() / 32) ; 
                    if(cas != '-' && cas != 'M' ){
                    
                    joueur.haut();
                    joueur.SetOrientation(c);
                    }              
            }
            break;
        case 's':
            dialogue_actuel = "";
            if(joueur.getPositionY() + 32 > 640 - 32){
                
                if (recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Basse_Number() == -1 ){
                    /*Zone z(recupereNiveau(niveau_actuel).genZone());
                    z.set_Zone_Haute_Number(zone_actuel);
                    recupereNiveau(niveau_actuel).getZone(zone_actuel).set_Zone_Basse_Number(recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level());
                    recupereNiveau(niveau_actuel).addZone(z);
                    zone_actuel = recupereNiveau(niveau_actuel).get_number_of_zone_in_the_level() - 1;
                */}
                else if ((recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Basse_Number() != -2 )) {
                    zone_actuel = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_Zone_Basse_Number();
                    joueur.setPositionY(joueur.getPositionY() - 640 + 32);
                    if (zone_actuel == 10){
                    dans_village = false;    
                    }
            
                }
                //joueur.setPositionY(joueur.getPositionY() - 640 + 32);
            }
            //setPositionX(getPositionX()-32);
            else {
                    char cas = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse((joueur.getPositionY() + 32) / 32 , joueur.getPositionX() / 32) ;
                    if( cas != '-' && cas != 'M'){
                    joueur.bas();
                    joueur.SetOrientation(c);
                    }
            }
            
        break;
        case 'e':
        {
            bool att_p;
            att_p = true;
            int com;
            com = 0;
            vector<Pnj>& l_pnj = niveau[niveau_actuel].getZone(zone_actuel).getPNJ();
            vector<Ennemi>& enemZone = recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi();
            /*
            int cibleX = joueur.getPositionX();
            int cibleY = joueur.getPositionY();
            switch (joueur.GetOrientation()) {
                case 'z': cibleY -= 32; break;
                case 's': cibleY += 32; break;
                case 'd': cibleX += 32; break;
                case 'q': cibleX -= 32; break;
            }

            bool ennemiDevant = false;
            for (int i = 0; i < static_cast<int>(enemZone.size()); i++) {
                if (enemZone[i].getPositionX() == cibleX && enemZone[i].getPositionY() == cibleY) {
                    ennemiDevant = true;
                    break;
                }
            }

            if (!ennemiDevant) {
            */
            switch (joueur.GetOrientation()) {
                case 'z':
                
                while ((com < size(l_pnj))and((joueur.getPositionX() != l_pnj[com].getPositionX())||(joueur.getPositionY()-32 != l_pnj[com].getPositionY()))){
                    com = com +1 ;
                }
                if (com < size(l_pnj)) att_p = false;
                break;
                case 's':
                while ((com < size(l_pnj))and((joueur.getPositionX() != l_pnj[com].getPositionX())||(joueur.getPositionY()+32 != l_pnj[com].getPositionY()))){
                    com = com +1 ;
                }
                if (com < size(l_pnj)) att_p = false;
                
                break;
                case 'd':
                while ((com < size(l_pnj))and((joueur.getPositionX()+32 != l_pnj[com].getPositionX())||(joueur.getPositionY() != l_pnj[com].getPositionY()))){
                    com = com +1 ;
                }
                if (com < size(l_pnj)) att_p = false;
                
                break;
                case 'q':
                while ((com < size(l_pnj))and((joueur.getPositionX()-32 != l_pnj[com].getPositionX())||(joueur.getPositionY() != l_pnj[com].getPositionY()))){
                    com = com +1 ;
                }
                if (com < size(l_pnj)) att_p = false;
                
                break;

            }
            /*
            }*/
            if(att_p /*|| ennemiDevant*/){
            vector<Attaque> & a = recupereNiveau(niveau_actuel).getZone(zone_actuel).getAttaques();
            switch (joueur.GetOrientation()) {
                case 'z':
                a.insert(a.begin() ,Attaque("aes" , 10 ,joueur.getPositionX() , joueur.getPositionY() - 32));
                break;
                case 's':
                a.insert( a.begin() ,Attaque("aes" , 10 ,joueur.getPositionX() , joueur.getPositionY() + 32));
                break;
                case 'd':
                a.insert(a.begin() ,Attaque("aes" , 10 ,joueur.getPositionX() + 32 , joueur.getPositionY()));
                break;
                case 'q':
                a.insert(a.begin() ,Attaque("aes" , 10 ,joueur.getPositionX() - 32 , joueur.getPositionY()));
                break;

            }
            vector<Ennemi> &enem = recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi();
            bool ennemiTouche = false;

            for (int k = 0 ; k < enem.size() ; k++ ){
                if (a.size() >0) {
                    if (a[0].PosX == enem[k].getPositionX()){
                        if (a[0].PosY == enem[k].getPositionY()) {
                            ennemiTouche = true;
                            joueur.attaquer(enem[k]);
                            if (enem[k].getPv() == 0 ) {
                                if (enem[k].getType() == 'B') {
                                    boss_vaincu = true;
                                }
                                incrementer_Ennemis_Tues();
                                vector<Ennemi> ene_t ;
                                for (int r = 0; r < enem.size() ; r++) {
                                    if (k != r) {
                                    ene_t.push_back(enem[r]);
                                    }
                                }
                                enem.clear();
                                for (int t = 0 ; t < ene_t.size() ; t++){
                                enem.push_back(ene_t[t]);
                                }

                            }
                        }

                    }
                
                }

                
            }
            if (!ennemiTouche) {
                Zone& zone = recupereNiveau(niveau_actuel).getZone(zone_actuel);
                vector<Coffre>& coffres = zone.getCoffres();
                bool coffreTrouve = false;

                for(int i = 0; i < coffres.size(); i++) {
                    if (coffres[i].getPositionX() == a[0].PosX && coffres[i].getPositionY() == a[0].PosY) {
                        coffreTrouve = true;
                        if(!coffres[i].estOuvert()) {
                            coffres[i].ouvrir();
                        
                        vector<Objet>& objets = coffres[i].getContenu();

                        if(objets.empty()) {
                            cout << "Le coffre est vide." << endl;
                        } else {
                            cout << "Le coffre est ouvert : " << endl;
                            for(int j = 0; j < objets.size(); j++) {
                                inventaire.ajouterObjet(objets[j]);
                                incrementer_Objets_Recuperes();

                                cout << "Objet trouve : " << objets[j].getNom() << endl;
                                if(j < objets.size() - 1) {
                                    cout << ", ";
                                }
                            }
                            cout << endl;
                        }

                        objets.clear(); // On vide le coffre après avoir récupéré les objets
                        zone.Terrain[coffres[i].getPositionY() / 32][coffres[i].getPositionX() / 32] = 'O';
                    } else {
                        cout << "Ce coffre est deja ouvert." << endl;
                    }
                }

            }
        }
            
            }
            else {
                if (com >= 0 && com < static_cast<int>(l_pnj.size())) {
                    l_pnj[com].charger_dialogue(1);
                    dialogue_actuel = l_pnj[com].getDialogue();
                } else {
                    dialogue_actuel = "";
                }
            
            calculerScore();

            break;   
         
        }    
    }
    //si le joueur se déplace vers une case occupée par un caractère "-", on annule le déplacement et si la case est occupée par un ennemi, le joueur s'arrête à une case de distance de l'ennemi 

    //si le joueur se déplace vers une case occupée par un ennemi, le joueur s'arrête 
    //vector<Ennemi> & e = recupereNiveau(niveau_actuel).getZone(zone_actuel).getEnnemi();
    /*char caseJoueur = recupereNiveau(niveau_actuel).getZone(zone_actuel).get_terrain_casse(joueur.getPositionY() / 32 , joueur.getPositionX() / 32);
    
    if(caseJoueur == '-' || caseJoueur == 'M') 
    {
        switch (joueur.GetOrientation()) {
        case 'z':
        joueur.setPositionY(joueur.getPositionY() + 32);
        break;
        case 's':
        joueur.setPositionY(joueur.getPositionY() - 32);
        break;
        case 'd':
        joueur.setPositionX(joueur.getPositionX() - 32);
        break;
        case 'q':
        joueur.setPositionX(joueur.getPositionX() + 32);
        break;
        }
    }
    */
    
    
}
for(int k = 0 ; k < e.size() ; k++ ){
                //cout << "test";
                if (e[k].getPositionX() == joueur.getPositionX() && e[k].getPositionY() == joueur.getPositionY()) {
                    //cout << "collision énnemi joueur avant orientation";
                    switch (joueur.GetOrientation()) {
                        case 'z':
                            joueur.setPositionY(joueur.getPositionY() + 32);
                            cout << "collision énnemi joueur haut" << endl;
                            break;
                        case 's':
                            joueur.setPositionY(joueur.getPositionY() - 32);
                            cout << "collision énnemi joueur bas" << endl;
                            break;
                        case 'd':
                            joueur.setPositionX(joueur.getPositionX() - 32);
                            cout << "collision énnemi joueur droite" << endl;
                            break;
                        case 'q':
                            joueur.setPositionX(joueur.getPositionX() + 32);
                            cout << "collision énnemi joueur gauche" << endl;
                            break;
                        }
        }
            }
}
int ContexteJeu::get_Zone_Number(){
    return zone_actuel;
}

void ContexteJeu::set_Zone_Number(int n){
    zone_actuel = n;
}

int ContexteJeu::get_Actual_Level(){
    return niveau_actuel;

}
string ContexteJeu::get_dialoge(){
    return dialogue_actuel;
}
void ContexteJeu::set_Actual_Level(int n){
    niveau_actuel = n;
}

int ContexteJeu::get_Nombre_Ennemis_Tues() const {
    return ennemis_tues;
}

void ContexteJeu::incrementer_Ennemis_Tues() {
    ennemis_tues += 1;
}

bool ContexteJeu::verifPV(Entite& cible) {

    return cible.getPv() == 0;

}



void ContexteJeu::sauvegarderPartie(const string& nom_fichier) {
    sauvegarde.sauvegarderPartie(nom_fichier, *this);
}   

void ContexteJeu::chargerPartie(const string& nom_fichier) {
    sauvegarde.chargerPartie(nom_fichier, *this);
}

int ContexteJeu::get_Objets_Recuperes() const {
    return objets_recuperes;
}

void ContexteJeu::incrementer_Objets_Recuperes() {
    objets_recuperes += 1;
}

int ContexteJeu::getScore() const {
    return score;
}

void ContexteJeu::calculerScore() {
    score = ennemis_tues * 10 + objets_recuperes * 5;
}

vector<Objet> ContexteJeu::getInventaire() {
    return inventaire.getObjets();
}

bool ContexteJeu::estDansVillage()const{
    return dans_village;
}

void ContexteJeu::setScore(int s) {
    score = s;
}

bool ContexteJeu::estBossVaincu() const {
    return boss_vaincu;
}