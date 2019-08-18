//-------------------------------------------------
//
//    Copyright 2019 Brice Gay (brice.gay1@gmail.com)
//
//    This file is part of souris-distributeur.
//
//    souris-distributeur is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//
//    souris-distributeur is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with souris-distributeur.  If not, see <https://www.gnu.org/licenses/>.
//
//-------------------------------------------------


#ifndef ALGORITHME_H
#define ALGORITHME_H

#include "generalinclude.h"


struct Groupe {
    unsigned int taille;
    double valAddi;
};


struct RandTable {
    unsigned int id;
    unsigned int random;
    void update(unsigned int i) {
        id = i;
        random = static_cast<unsigned int>(rand());
    }
    bool operator < (RandTable &r) {
        return  random < r.random;
    }
};



class algorithmeTraitement
{
public:
    algorithmeTraitement(int * refProgression);
    void startGenetique();
    void stopGenetique();
    void setData(vector<Souris> * listeSouris, vector<int> * listeGroupes);
    vector<Souris> * sendResult();
    vector<unsigned int> * sendConsigne();

private:
    void init();
    void evaluation();
    void selection();
    void croisement();
    void mutation();
    void restartVariablesLocales();

    int * threadProgression;

    bool stopThread;


    vector<Souris> mSouris;
    vector<unsigned int> taillesGroupes;

    unsigned int nbSourisUtilisees;
    unsigned int nbSourisAEcarter;
    double globMoy, globMoyC;

    unsigned int nbGroupesImpairs;
    unsigned int bMin, bMax;

    vector<Groupe> groupes;
    vector<double> sourisDispo[2];
    unsigned int tailleChromoTheorique, tailleChromoReel;

    #define NB_MAX_SOURIS 300
    #define MAX_NB_GRP 50
    #define TAILLE_POP 100
    #define PRESSIONS_SELECTION 70 //lors de la selection, la proba que le meilleur individu soit selectionné est  PRESSIONS_SELECTION / DIVISEUR_POURCENTAGES
    #define NB_INDIVIDUS_PARENTS 30
    #define MAX_NB_GENERATION 100000
    #define PROBA_MUTATION 1
    #define DIVISEUR_POURCENTAGES 100

    bool iGen = 0;

    unsigned int lastGrpId;


    unsigned short pop[2][TAILLE_POP][2][NB_MAX_SOURIS/2];//WARNING limite le nb de souris
    long double scoreIndiv[TAILLE_POP];
    unsigned int iSouris;//attention à l'utilisation dans des fonctions

    short curTest = 0;
    short nbDejaVu[2][NB_MAX_SOURIS/2];
    RandTable tablePermutation[NB_MAX_SOURIS / 2];


    double bestBestScore = 1000000;


    double moyGrp[MAX_NB_GRP], moyGrpC[MAX_NB_GRP];
    double moyMoy, moyMoyC;
    unsigned int curReadId;
    unsigned int startReadId;
    double bestScore;
    unsigned int idBestScore;


    unsigned short iConcurrent[2];
    bool iSelec;


    int iParents[2];
    int iCoupure;
    int iSouris2;
    int i1, i2, iChromo, swapNb;


};



#endif // ALGORITHME_H
