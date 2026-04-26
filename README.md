# Castle Maze

## Description

```

castle_maze est un jeu ou on arrive dans un village. 
Le but est de tuer le boss d'un donjon qui se trouve dans un portail magique.
On doit donc réussir a récupérer des objet pour pouvoir être assez fort pour battre le boss.

On commence devant l’écran titre ou on peut sélectionner jouer ,
l'objectif est de maximiser son score dans chaque partie .
Pour cela , on peut choisir un personnage qui a une classe et des statiques. 
Après ce choix on atterrit dans un écran village ou on peut discuter avec des pnj . 
Puis on peut sortir et se balader dans plusieurs écrans de batailles .
Dans ces écrans on peut affronter un certain nombre d'ennemis qui ont des statiques propre.
Affronter un ennemi consiste a esquivé ces attaques et d'utiliser l'outil de sa classe pour infliger des dégâts .
Il est possible de trouver un boss dans ces écrans.
Si on tue le boss la partie est terminé.
Dans tous les écrans il est possible de trouver des coffres 
qui contienne des pièces 
qui permette d'augmenter son score ou d'autre objet permettant d'augmenter les statistiques.  


-l'affichage

-entitée
        -PV
            -attaquant (DGT, Type d'attaque, Boost ,Malus)
                -personnage
                -boss
                -mob
            
            -pnj 
                -villageos
                    -en danger(bool)
-drop
    -provenant de :
        -coffre
        -mob
        -boss

-objet
    -changement de statistques( Pour les attaquants)



-combat( permet de generer les attaques entres les entitées)

-ecran (SDL)

-map 
    -écran différents

-mission (conditions a remplir)

-score (monstres tuées, objet recupéré, pnj sauvé)


```
### Organisation du projet :    
Le projet est organisé en plusieurs dossiers :
- `src/` : contient les fichiers sources du projet
- `bin/` : contient les exécutables générés après compilation
- `obj/` : contient les fichiers objets générés après compilation
- `font/` : contient les polices utilisées dans le projet
- `img/` : contient les images utilisées dans le projet
- `data/` : contient les fichiers de données (niveaux, personnages, ennemis, etc.)
- `doc/` : contient la documentation générée par Doxygen
- `README.md` : ce fichier de documentation


## Installation 
Pour compiler le projet, il suffit d'exécuter la commande `make` dans le terminal à la racine du projet.

Cela va créer les dossiers nécessaires, compiler les fichiers sources et générer les exécutables  : 
- `bin/CastleMaze_Text` pour la version textuelle
- `bin/CastleMaze_SDL` pour la version graphique.

## Utilisation
- Pour jouer à la version textuelle, exécutez la commande `make text`
- Pour jouer à la version graphique, exécutez la commande `make SDL`

## Nettoyage
Pour nettoyer les fichiers objets et les exécutables, exécutez la commande `make clean`
# GAMEPLAY

## Commandes :
**Déplacement :**
   - `Z` ou `↑`= Haut 
   - `S` ou `↓`= Bas
   - `Q` ou `←`= Gauche
   - `D` ou `→`= Droite

**Attaques :**
- `Espace` ou `1` : Attaque de base
- `J` ou `2`: Attaque spéciale (si disponible)
- `K` ou `3`: Utiliser un objet (si disponible)



Yohan : SDL // affichage des objets , le score , le boss, 
Mathieu : generateur des zones, les pnj
Chaouki : objets, utilisation des objets dans inventaire, recuperation des objets dan zone, score, boss//
    
Mathieu : Fonction d'affichage en mode Text faire liens entre les classes

Mathieu : son en sdl , rajouter pnj , objets , dialogues , (modifier les menus de base utiliser asset du pack)