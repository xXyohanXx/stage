#ifndef ENTITE_H
#define ENTITE_H

#include<string>
#include <vector>
using namespace std;

class Entite {

    private:

        
        int positionX;
        int positionY;
        int pv; //les points de vie
        int defense;

    public:

        string nom;
        Entite();
        void update();

        //accesseurs et mutateurs pour positionX
        void setPositionX(int x);
        int getPositionX() const;
        //accesseurs et mutateurs pour positionY
        void setPositionY(int y);
        int getPositionY() const;
        //accesseurs et mutateurs pour pv
        void setPv(int pv);
        int getPv() const;
        
        void setDefense(int d);
        int getDefense() const;

        void gauche();
        void droite();
        void haut();
        void bas();

};

#endif