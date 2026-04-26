
#include "Jeu_text.h"
#include "Ennemi.h"
#include "Pnj.h"
#include "Zone.h"
#include "attaque.h"
#include "joueur.h"
#include "Sauvegarde.h"

#include <algorithm>
#include <cstring>
#include <ostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include "winTxt.h"

#include <iostream>
#include <iterator>
#include <string>
#include <cstring>
#include <vector>

void Jeu_text::commencer(){
    string nom ;
	string save ; 
    cout << "Bienvenue dans castle_maze" << endl;
    cout << "Quel est votre nom jeune aventurier " << endl;
    cin >> nom  ;
	
	cout << "Nom saisi : " << nom << endl;
    
	contexte.modifierJoueur(nom);
    cout << "Dans le village de Teburtz , Le Maire Antoine vous propose de faire partie de la brigade anti-monstre de la ville . " << endl;
    cout << "Pour vous aidez dans votre tache , il vous donne accès au portail a l'orée de la ville " << endl;
    cout << "Pour vous déplacez vous pouvez utilisez  z , q ,s d et e pour attaquer " << endl;
    cout << "Bonne chance jeune aventurier" << endl;

	//charger une partie sauvegardée avec un numéro de sauvegarde choisi par le joueur
	
	cout << "choissez le numéro de sauvergade a charger " << endl;
	cin >> save ;
	if (save != "0") {
		Sauvegarde savegame;
		string fichier_save = "save" + save + ".txt";
		
		// Création des references 

		string& fichier_save_ref = fichier_save; 
		ContexteJeu& contexte_ref = contexte;
		savegame.chargerPartie(fichier_save_ref, contexte_ref);
	}
    //afficher_zone(0);

}
void Jeu_text::afficher_zone(int n){
    vector<vector<int>> posit = contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(n).getPosition();
    vector<int> pos_a_verif ;
	for(int i = 0 ; i <= 640 ; i = i + 32 )  {
		for (int j = 0; j <= 800 ; j = j + 32) {

            pos_a_verif = {i  , j };
            for (int k = 0 ; k < size(posit) ; k++ ){
            if (pos_a_verif == posit[k] ) {
                cout << "E";
            
            }
            else {
                cout << "*";
            }
            
            
        }
        }
        cout<< endl;


    }
    
    cout << contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(n).getPosition()[0][0] <<endl;
    cout << contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(n).getPosition()[0][1] <<endl;
    
}


void Jeu_text::txtAff(WinTXT &win )
{
	const int terrainHeight = 20;
	const int terrainWidth = 25;

    //char * ter[20][26];
    //strcpy(ter, contexte.recupereNiveau().getZone(n).Terrain );
	Zone &ter = contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()) ;
	Joueur &joueur = contexte.recupereJoueur();
	//Joueur pac = contexte.recupereJoueur();
	//const Fantome &fan = jeu.getConstFantome();
	
	win.clear();
	termClear();
	if (!contexte.verifPV(contexte.recupereJoueur())){
	string hudJoueur = "J: " + joueur.nom + " PV: " + to_string(joueur.getPv());
	for (int i = 0; i < static_cast<int>(hudJoueur.size()); ++i)
		win.print(i, 0, hudJoueur[i]);

	string hudEnnemi = "E: aucun";
	if (!ter.getEnnemi().empty()) {
		hudEnnemi = "E PV: " + to_string(ter.getEnnemi()[0].getPv());
	}
	for (int i = 0; i < static_cast<int>(hudEnnemi.size()); ++i)
		win.print(i, 1, hudEnnemi[i]);

	// Affichage des murs et des pastilles
	for (int y = 0; y < terrainHeight; ++y)
		for (int x = 0; x < terrainWidth; ++x)
			win.print(x, y + 2, ter.get_terrain_casse(y ,x  ));

	//affichage des dialogue
	for (int j= 0 ;j < contexte.get_dialoge().size() ; j++ ) {
		win.print(j , 22 , contexte.get_dialoge()[j]);
	
	}
	// Affichage de Pacman
	win.print(joueur.getPositionX()/32, joueur.getPositionY()/32 + 2, 'P');
	contexte.deplacement_ennemis();
	// afichage des ennemsi
	for (int i = 0 ; i < contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getPosition().size() ; i++){
		Ennemi & ennemi_boucle = contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getEnnemi()[i];
		win.print( ennemi_boucle.getPositionX()/32,  ennemi_boucle.getPositionY()/32 + 2, ennemi_boucle.getType());

	}
	for (int i = 0 ; i < contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getPNJ().size() ; i++){
		Pnj & ennemi_boucle = contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getPNJ()[i];
		win.print( ennemi_boucle.getPositionX()/32,  ennemi_boucle.getPositionY()/32 + 2, 'V');

	}
	// afichage des attaques
	for (int j = 0 ; j < contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getAttaques().size() ; j++){
		vector<Attaque> & a = contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getAttaques();
		if (a[j].time <= 8){
		win.print(a[j].PosX/32,  a[j].PosY/32 + 2, '~');
		a[j].time +=1;
		}
		else {

			//a.erase(find(a.begin() , a.end() , a[j]));
			a.pop_back();
		}
	}
	}
	else {
		string p = "Vous avez perdu";
		for (int t = 0; t < static_cast<int>(p.size()); ++t){
		win.print(t, 0, p[t]);}

	}
	// Affichage du Fantome
	//win.print(fan.getX(), fan.getY(), 'F');

	win.draw();
}

void Jeu_text::txtBoucle()
{
	
	// Creation d'une nouvelle fenetre en mode texte
	// => fenetre de dimension et position (WIDTH,HEIGHT,STARTX,STARTY)
	WinTXT win(25, 2 + 20 + 1 );

	bool ok = true;
	int c;

	do
	{
		txtAff(win );
		//contexte.recupereNiveau(contexte.get_Actual_Level()).id  2;
#ifdef _WIN32
		Sleep(100);
#else
		usleep(100000);
#endif // WIN32
		

		c = win.getCh();
		switch (c)
		{
		case 'z':
			contexte.actionClavier('z');
			//contexte.recupereJoueur().haut();
			break;
		case 'q':
			contexte.actionClavier('q');
			//contexte.recupereJoueur().gauche();
			break;
		case 's':
			contexte.actionClavier('s');
			//contexte.recupereJoueur().bas();
			break;
		case 'd':
			//contexte.recupereJoueur().droite();
			contexte.actionClavier('d');
			break;
		case 'e':
			contexte.actionClavier('e');
			/*
			if (contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getAttaques().size() < 1){
			contexte.actionClavier('e');
			}*/
			break;
		case 't':
			//contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).move_ennemi_l(0);
			//contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getEnnemi()[0].setPositionX(128);
			//cout << contexte.recupereNiveau(contexte.get_Actual_Level()).getZone(contexte.get_Zone_Number()).getEnnemi()[0].getPositionX();
			//cout << contexte.recupereJoueur().getPositionX();
			//cout << contexte.recupereNiveau(contexte.get_Actual_Level()).getId();
			break;
		case 'a':
			ok = false;
			break;
		}

	} while (ok);
}

string Jeu_text::afficher_inventaire() {
    string texte = "Inventaire : ";

	vector<Objet> objetzone = contexte.getInventaire();

    if (objetzone.empty()) {
        return texte + "vide";
    }

    for (int i = 0; i < objetzone.size(); i++) {
        texte += objetzone[i].getNom();

        if (i < objetzone.size() - 1) {
            texte += ", ";
        }
    }

    return texte;
}


