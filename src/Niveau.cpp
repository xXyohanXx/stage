#include "Niveau.h"
#include "Ennemi.h"
#include "Pnj.h"
#include "Zone.h"

#include <cassert>
#include <cstdlib>
#include <iterator>
#include <random>


Niveau::Niveau() {
    //generation village
    srand (time(NULL));
    Zone z1("1");
    Pnj m("1");
    z1.getPNJ().push_back(m);
    z1.getPNJ()[0].setPositionX(128);
    z1.getPNJ()[0].setPositionY(128);
    z1.set_Zone_Haute_Number(1);
    z1.set_Zone_Basse_Number(2);
    z1.set_Zone_Droite_Number(3);
    z1.set_Zone_Gauche_Number(4);  

    Zone z2("2");
    //z1.set_Zone_Haute_Number(1);
    z2.set_Zone_Basse_Number(0);
    z2.set_Zone_Droite_Number(5);
    z2.set_Zone_Gauche_Number(6);        
    Zone z3("3");
    z3.set_Zone_Haute_Number(0);
    //z1.set_Zone_Basse_Number(2);
    z3.set_Zone_Droite_Number(7);
    z3.set_Zone_Gauche_Number(8);        
    
    Zone z4("4");
    z4.set_Zone_Haute_Number(5);
    z4.set_Zone_Basse_Number(7);
    z4.set_Zone_Droite_Number(9);
    z4.set_Zone_Gauche_Number(0);        
    Zone z5("5");
    z5.set_Zone_Haute_Number(6);
    z5.set_Zone_Basse_Number(8);
    z5.set_Zone_Droite_Number(0);
    //z1.set_Zone_Gauche_Number(4);        
    Zone z6("6");
    //z1.set_Zone_Haute_Number(1);
    z6.set_Zone_Basse_Number(3);
    //z1.set_Zone_Droite_Number(3);
    z6.set_Zone_Gauche_Number(1);        
    Zone z7("7");
    //z1.set_Zone_Haute_Number(1);
    z7.set_Zone_Basse_Number(4);
    z7.set_Zone_Droite_Number(1);
    //z1.set_Zone_Gauche_Number(4);        
    
    Zone z8("8");
    //z1.set_Zone_Haute_Number(3);
    z8.set_Zone_Haute_Number(3);
    //z1.set_Zone_Droite_Number(3);
    z8.set_Zone_Gauche_Number(2);        
    
    Zone z9("9");
    z9.set_Zone_Haute_Number(4);
    //z1.set_Zone_Basse_Number(2);
    //z1.set_Zone_Droite_Number(3);
    z9.set_Zone_Droite_Number(2);        
    Zone z10("10");
    //z1.set_Zone_Haute_Number(1);
    //z1.set_Zone_Basse_Number(2);
    z10.set_Zone_Droite_Number(10);
    z10.set_Zone_Gauche_Number(3);        
    zones.push_back(z1);
    zones.push_back(z2);
    zones.push_back(z3);
    zones.push_back(z4);
    zones.push_back(z5);
    zones.push_back(z6);
    zones.push_back(z7);
    zones.push_back(z8);
    zones.push_back(z9);
    zones.push_back(z10);



    //generation donjon
    Zone ze("4" ,"1" , "_g");
    //ze.set_z_max(1);
    ze.set_nb_changement_de_zone_possible(4);
    zones.push_back(ze);
    difficultés = 9; //c'est le nb de zone -1 car la zone de base n'est pas considerer 
    int c = 10;
    int possibilités_non_explorée; 
    //zones[0].set_z_max( difficultés - possibilités_non_explorée + 1 );
    possibilités_non_explorée = ze.get_nb_changement_de_zone_possible();
    //zones[0].set_z_max( difficultés - possibilités_non_explorée + 1 );
    zones[c].set_z_max( difficultés - possibilités_non_explorée + 1 );
    if (difficultés >= 4){
        while (c < size(zones)) {
        
        
            while(zones[c].get_nb_changement_de_zone_possible() > 0){
                possibilités_non_explorée = zones[c].get_nb_changement_de_zone_possible();
                
                
                if (zones[c].get_Zone_Haute_Number() == -1){
                    int n;
                    int e;
                    int a ;
                    n = zones[c].get_z_max();
                    
                    if (n != 0){
                        if(possibilités_non_explorée == 1 ){
                            a = zones[c].get_z_max()  ;
                        }
                        else {    
                            a = rand() % n +1;
                            
                        }
                        if (a > 4) n = 4;
                        else n = a;

                        if ( a > 1){
                        e = rand() % (n-1) +2;
                        }
                        else {
                            e = rand() % n + 1;
                            
                        }
                    }
                    else {
                        e = 1;
                        a = 1;
                    }
                    zones[c].set_z_max(zones[c].get_z_max() - a +1 );
                    Zone z( genPorte(e) ,genZone(), "_h");
                    z.set_z_max(a-e+1);
                    

                    if (rand() % 2 == 0) {
                        Coffre coffre(7 * 32, 7 * 32);

                        Objet obj;
                        int numObjet = rand() % 4 + 1;
                        obj.charger_objet(to_string(numObjet));

                        coffre.ajouterObjet(obj);
                        z.getCoffres().push_back(coffre);
                        z.Terrain[7][7] = 'C';
                    }

                    zones[c].set_Zone_Haute_Number(zones.size());
                    z.set_Zone_Basse_Number(c);
                    z.set_nb_changement_de_zone_possible(e-1);
                    zones.push_back(z);
                }
                else if (zones[c].get_Zone_Basse_Number() == -1) {
                    int n;
                    int e;
                    int a ;
                    n = zones[c].get_z_max();
                    
                    if (n != 0){
                        if(possibilités_non_explorée == 1 ){
                            a = zones[c].get_z_max()  ;
                        }
                        else {    
                            a = rand() % n +1;
                            
                        }
                        if (a > 4) n = 4;
                        else n = a;
                        if (a > 1){
                        e = rand() % (n-1) +2;
                        }
                        else {
                            e = rand() % n + 1;
                            
                        }
                    }
                    else {
                        e = 1;
                        a = 1;
                    }
                    zones[c].set_z_max(zones[c].get_z_max() - a +1 );
                    Zone z( genPorte(e) ,genZone() , "_b");
                    if(possibilités_non_explorée == 1 ){
                    z.set_z_max(zones[c].get_z_max()-e+1);
                    }
                    else {
                    z.set_z_max(a-e+1);
                    }

                    if (rand() % 2 == 0) {
                        Coffre coffre(7 * 32, 7 * 32);

                        Objet obj;
                        int numObjet = rand() % 4 + 1;
                        obj.charger_objet(to_string(numObjet));

                        coffre.ajouterObjet(obj);
                        z.getCoffres().push_back(coffre);
                        z.Terrain[7][7] = 'C';
                    } 

                    zones[c].set_Zone_Basse_Number(zones.size());
                    z.set_Zone_Haute_Number(c);
                    z.set_nb_changement_de_zone_possible(e-1);
                    zones.push_back(z);
                }
                else if (zones[c].get_Zone_Droite_Number() == -1){
                    int n;
                    int e;
                    int a ;
                    n = zones[c].get_z_max();
                    
                    if (n != 0){
                        if(possibilités_non_explorée == 1 ){
                            a = zones[c].get_z_max() ;
                        }
                        else {    
                            a = rand() % n +1;
                            }
                        if (a > 4) n = 4;
                        else n = a;
                        if ( a > 1){
                        e = rand() % (n-1) +2;
                        }
                        else {
                            e = rand() % n + 1;
                            
                        }
                    }
                    else {
                        e = 1;
                        a = 1;
                    }
                    Zone z( genPorte(e) ,genZone() , "_d");
                    if(possibilités_non_explorée == 1 ){
                    z.set_z_max(zones[c].get_z_max()-e+1);
                    }
                    else {
                    z.set_z_max(a-e+1);
                    }

                    if (rand() % 2 == 0) {
                        Coffre coffre(7 * 32, 7 * 32);

                        Objet obj;
                        int numObjet = rand() % 4 + 1;
                        obj.charger_objet(to_string(numObjet));

                        coffre.ajouterObjet(obj);
                        z.getCoffres().push_back(coffre);
                        z.Terrain[7][7] = 'C';
                    }

                    zones[c].set_z_max(zones[c].get_z_max() - a +1);
                    zones[c].set_Zone_Droite_Number(zones.size());
                    z.set_Zone_Gauche_Number(c);if (n > 4) n = 4;
                    z.set_nb_changement_de_zone_possible(e-1);
                    zones.push_back(z);
                }
            
                else if (zones[c].get_Zone_Gauche_Number() == -1){
                    int n;
                    int e;
                    int a ;
                    n = zones[c].get_z_max();
                    
                    if (n != 0){
                        if(possibilités_non_explorée == 1 ){
                            a = zones[c].get_z_max() ;
                        }
                        else {    
                            a = rand() % n +1;
                        }
                        if (a > 4) n = 4;
                        else n = a;
                        if ( a > 1){
                        e = rand() % (n-1) +2;
                        }
                        else {
                            e = rand() % n + 1;
                        
                        }
                    }
                    else {
                        e = 1;
                        a = 1;
                    }
                    
                    zones[c].set_z_max(zones[c].get_z_max() - a +1 );
                    Zone z( genPorte(e),genZone(),"_g");
                    if(possibilités_non_explorée == 1 ){
                    z.set_z_max(zones[c].get_z_max()-e+1);
                    }
                    else {
                    z.set_z_max(a-e+1);
                    }
                    if (rand() % 2 == 0) {
                        Coffre coffre(7 * 32, 7 * 32);

                        Objet obj;
                        int numObjet = rand() % 4 + 1;
                        obj.charger_objet(to_string(numObjet));

                        coffre.ajouterObjet(obj);
                        z.getCoffres().push_back(coffre);
                        z.Terrain[7][7] = 'C';
                    }
                    
                    zones[c].set_Zone_Gauche_Number(zones.size());
                    
                    z.set_Zone_Droite_Number(c);
                    z.set_nb_changement_de_zone_possible(e-1);
                    zones.push_back(z);}
                    
                zones[c].set_nb_changement_de_zone_possible(possibilités_non_explorée -1);
                difficultés = difficultés - 1;
                //generation boss 
                
                //c = c+1 ;
            }
        c += 1;
        if (c == size(zones)){
                    Ennemi e;
                    e.setType('B');
                    zones[c- 1].getEnnemi().push_back(e);
                }
        }
    }

    /*while (possibilités_non_explorée > 0){
        while (zones[c].get_nb_changement_de_zone_possible() > 0){
            zones[c].set_z_max( difficultés - possibilités_non_explorée + 1 );
            Zone z( genPorte(zones[c].get_z_max()),genZone());
            zones.push_back(z);

        }


    }*/
    //zones[0] = z;
    id = 0;
    nom = "niveauName";

}

void Niveau::charger(){


}

void Niveau::terminer(){


}

void Niveau::addZone(Zone & z){
    zones.push_back(z);
}

void Niveau::clearZones() {
    zones.clear();
}

string Niveau::genZone(){
    int n;
    n = rand() % 2 + 1;
    
    switch (n) {
    case 1:
        return "1";
        break;
    case 2 :
        return "2";
        break;
    }
    return  "-1";
}
string Niveau::genPorte(int e){
    switch (e) {
    case 1:
        return "1";
        break;
    case 2 :
        return "2";
        break;
    case 3:
        return "3";
        break;
    case 4:
        return "4";
        break;

    
    }
    return "-1";
    

}
int Niveau::get_number_of_zone_in_the_level(){
    return zones.size();
}
int Niveau::getId() {
    return id;
}

string Niveau::getNom() const {
    return nom;
}

void Niveau::setId(int i) {
    id = i;
}

void Niveau::setNom(string n) {
    nom = n;
}
Zone & Niveau::getZone(int n){

    //assert(n >= 0 && n < 5);
    return  zones[n];
}

const Zone& Niveau::getZone(int n) const {
    //assert(n >= 0 && n < 5);
    return zones[n];
}
