#ifndef OBJET_H
#define OBJET_H

#include<string>
#include<vector>

class Objet {

    private:

        std::string nom;
        std::string type;
        std::vector<std::string> stats;
        std::vector<int> valeurs;

    public: 

        Objet();
        Objet(const std::string& nom, const std::string& type, const std::vector<std::string>& stats, const std::vector<int>& valeurs);

        std::string getNom() const;
        std::string getType() const;
        std::vector<int> getValeurs() const;
        std::vector<std::string> getStats() const;
        void appliquer();
        void charger_objet(std::string num );

};
#endif