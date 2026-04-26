#ifndef NIVEAU_H
#define NIVEAU_H

#include "Zone.h"

#include<string>
#include <vector>

using namespace std;

class Niveau {

    private:

        //int possibilités_non_explorée;
        string nom;
        vector<Zone> zones;
        int difficultés;
        

        //Zone zones[5];
    public:
        int id ;
        Niveau();
        void charger();
        void terminer();

        int getId();
        string getNom() const;
        string genZone();
        string genPorte(int n);
        Zone& getZone(int n );
        void addZone(Zone & z);
        void clearZones();
        const Zone& getZone(int n ) const;
        int get_number_of_zone_in_the_level();
        void setId(int i);
        void setNom( string n);

        
};

#endif
