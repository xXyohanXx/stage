#//no-pie pour éviter les erreurs de compilation pour SDL

# -------------------------------------------------Makefile pour compiler le projet Castle Maze---------------------------------------------------------- #


all: obj bin data/sauvegarde/ documentation bin/CastleMaze_Text bin/CastleMaze_SDL

# Création des dossiers nécessaires pour les fichiers objets, les exécutables, les polices et les images
obj bin data/sauvegarde/ :
	mkdir -p obj bin data/sauvegarde/

# Génération des fichiers objets obj (.o) à partir des fichiers sources (.cpp et .h):

obj/AffichageSDL.o : src/AffichageSDL.cpp src/AffichageSDL.h src/AffichageSDLUtils.h | obj
	g++ -std=c++17 -c src/AffichageSDL.cpp -o obj/AffichageSDL.o

obj/AffichageSDLUtils.o : src/AffichageSDLUtils.cpp src/AffichageSDLUtils.h | obj
	g++ -std=c++17 -c src/AffichageSDLUtils.cpp -o obj/AffichageSDLUtils.o

obj/HUDSDL.o : src/HUDSDL.cpp src/AffichageSDL.h src/AffichageSDLUtils.h | obj
	g++ -std=c++17 -c src/HUDSDL.cpp -o obj/HUDSDL.o

obj/JeuSDL.o : src/JeuSDL.cpp src/AffichageSDL.h src/AffichageSDLUtils.h src/ContexteJeu.h | obj
	g++ -std=c++17 -c src/JeuSDL.cpp -o obj/JeuSDL.o

obj/JoueurSDL.o : src/JoueurSDL.cpp src/AffichageSDL.h src/AffichageSDLUtils.h | obj
	g++ -std=c++17 -c src/JoueurSDL.cpp -o obj/JoueurSDL.o

obj/Attaque.o : src/attaque.cpp src/attaque.h | obj
	g++ -std=c++17 -c src/attaque.cpp -o obj/Attaque.o
	
obj/ContexteJeu.o : src/ContexteJeu.cpp src/ContexteJeu.h | obj
	g++ -std=c++17 -c src/ContexteJeu.cpp -o obj/ContexteJeu.o

obj/DeadSDL.o : src/DeadSDL.cpp src/AffichageSDL.h | obj
	g++ -std=c++17 -c src/DeadSDL.cpp -o obj/DeadSDL.o

obj/Ennemi.o : src/Ennemi.cpp src/Ennemi.h | obj
	g++ -std=c++17 -c src/Ennemi.cpp -o obj/Ennemi.o

obj/EnnemiSDL.o : src/EnnemiSDL.cpp src/AffichageSDL.h src/Ennemi.h | obj
	g++ -std=c++17 -c src/EnnemiSDL.cpp -o obj/EnnemiSDL.o

obj/PnjSDL.o : src/PnjSDL.cpp src/AffichageSDL.h src/Pnj.h | obj
	g++ -std=c++17 -c src/PnjSDL.cpp -o obj/PnjSDL.o

obj/CoffreSDL.o : src/CoffreSDL.cpp src/AffichageSDL.h src/Coffre.h | obj
	g++ -std=c++17 -c src/CoffreSDL.cpp -o obj/CoffreSDL.o

obj/DialogueSDL.o : src/DialogueSDL.cpp src/AffichageSDL.h src/AffichageSDLUtils.h | obj
	g++ -std=c++17 -c src/DialogueSDL.cpp -o obj/DialogueSDL.o

obj/Entite.o : src/Entite.cpp src/Entite.h | obj
	g++ -std=c++17 -c src/Entite.cpp -o obj/Entite.o

obj/Jeu_text.o : src/Jeu_text.cpp src/Jeu_text.h | obj
	g++ -std=c++17 -c src/Jeu_text.cpp -o obj/Jeu_text.o

obj/joueur.o : src/joueur.cpp src/joueur.h | obj
	g++ -std=c++17 -c src/joueur.cpp -o obj/joueur.o

obj/main_text.o : src/main_text.cpp src/Jeu_text.h src/ContexteJeu.h | obj
	g++ -std=c++17 -c src/main_text.cpp -o obj/main_text.o

obj/mainSDL.o : src/mainSDL.cpp src/AffichageSDL.h src/ContexteJeu.h | obj
	g++ -std=c++17 -c src/mainSDL.cpp -o obj/mainSDL.o

obj/MenuSDL.o : src/MenuSDL.cpp src/AffichageSDL.h src/AffichageSDLUtils.h | obj
	g++ -std=c++17 -c src/MenuSDL.cpp -o obj/MenuSDL.o


obj/Niveau.o : src/Niveau.cpp src/Niveau.h | obj
	g++ -std=c++17 -c src/Niveau.cpp -o obj/Niveau.o

obj/Pnj.o : src/Pnj.cpp src/Pnj.h | obj
	g++ -std=c++17 -c src/Pnj.cpp -o obj/Pnj.o

obj/Zone.o : src/Zone.cpp src/Zone.h | obj
	g++ -std=c++17 -c src/Zone.cpp -o obj/Zone.o

obj/Boss.o : src/Boss.cpp src/Boss.h | obj
	g++ -std=c++17 -c src/Boss.cpp -o obj/Boss.o

obj/Objet.o : src/Objet.cpp src/Objet.h | obj
	g++ -std=c++17 -c src/Objet.cpp -o obj/Objet.o

obj/Coffre.o : src/Coffre.cpp src/Coffre.h | obj
	g++ -std=c++17 -c src/Coffre.cpp -o obj/Coffre.o

obj/Inventaire.o : src/Inventaire.cpp src/Inventaire.h | obj
	g++ -std=c++17 -c src/Inventaire.cpp -o obj/Inventaire.o

obj/Score.o : src/Score.cpp src/Score.h | obj
	g++ -std=c++17 -c src/Score.cpp -o obj/Score.o

obj/TerrainSDL.o : src/TerrainSDL.cpp src/AffichageSDL.h | obj
	g++ -std=c++17 -c src/TerrainSDL.cpp -o obj/TerrainSDL.o

obj/Sauvegarde.o : src/Sauvegarde.cpp src/Sauvegarde.h | obj
	g++ -std=c++17 -c src/Sauvegarde.cpp -o obj/Sauvegarde.o

obj/winTxt.o : src/winTxt.cpp src/winTxt.h | obj
	g++ -std=c++17 -c src/winTxt.cpp -o obj/winTxt.o

# Compilation de l'exécutable à partir des fichiers objets
bin/CastleMaze_Text :  obj/Attaque.o obj/ContexteJeu.o obj/Ennemi.o obj/Entite.o obj/Jeu_text.o obj/joueur.o obj/Niveau.o obj/main_text.o obj/Pnj.o obj/Zone.o obj/Boss.o obj/Objet.o obj/Coffre.o obj/Inventaire.o obj/Score.o obj/Sauvegarde.o obj/winTxt.o | bin
	g++ -no-pie -std=c++17 obj/Attaque.o obj/ContexteJeu.o obj/Ennemi.o obj/Entite.o obj/Jeu_text.o obj/joueur.o obj/Niveau.o obj/main_text.o obj/Pnj.o obj/Zone.o obj/Boss.o obj/Objet.o obj/Coffre.o obj/Inventaire.o obj/Score.o obj/Sauvegarde.o obj/winTxt.o -o bin/CastleMaze_Text

bin/CastleMaze_SDL : obj/AffichageSDL.o obj/AffichageSDLUtils.o obj/Attaque.o obj/ContexteJeu.o obj/DeadSDL.o obj/Ennemi.o obj/EnnemiSDL.o obj/Entite.o obj/HUDSDL.o obj/JeuSDL.o obj/JoueurSDL.o obj/joueur.o obj/Niveau.o obj/mainSDL.o obj/MenuSDL.o obj/Pnj.o obj/PnjSDL.o obj/TerrainSDL.o obj/CoffreSDL.o obj/DialogueSDL.o obj/Zone.o obj/Boss.o obj/Objet.o obj/Coffre.o obj/Inventaire.o obj/Score.o obj/Sauvegarde.o | bin
	g++ -no-pie -std=c++17 obj/AffichageSDL.o obj/AffichageSDLUtils.o obj/Attaque.o obj/ContexteJeu.o obj/DeadSDL.o obj/Ennemi.o obj/EnnemiSDL.o obj/Entite.o obj/HUDSDL.o obj/JeuSDL.o obj/JoueurSDL.o obj/joueur.o obj/Niveau.o obj/mainSDL.o obj/MenuSDL.o obj/Pnj.o obj/PnjSDL.o obj/TerrainSDL.o obj/CoffreSDL.o obj/DialogueSDL.o obj/Zone.o obj/Boss.o obj/Objet.o obj/Coffre.o obj/Inventaire.o obj/Score.o obj/Sauvegarde.o -o bin/CastleMaze_SDL -lSDL2 -lSDL2_image -lSDL2_ttf
# Génération de la documentation :
documentation : doc/doxyfile
	doxygen doc/doxyfile && printf "\nDocumentation générée\n\n"

text : bin/CastleMaze_Text
	./bin/CastleMaze_Text

SDL : bin/CastleMaze_SDL
	./bin/CastleMaze_SDL
# Nettoyage des fichiers objets et de l'exécutable
clean :
	rm -rf obj/* bin/* doc/html && printf " \n\n Dossiers obj, bin et doc nettoyés \n\n"
# Nettoyage des sauvegardes 
make clean_saves :
	rm -rf data/sauvegarde/* && printf " \n\n Dossier de sauvegarde nettoyé \n\n"




