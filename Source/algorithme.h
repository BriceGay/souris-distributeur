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
    int taille;
    double valAddi;
    int valAddiId;
};


struct RandTable {
    int id;
    int random;
    void update(int i) {
        id = i;
        random = static_cast<int>(rand());
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
    void setData(vector<Souris> * listeSouris, vector<int> * listes);
    vector<Souris> * sendResult();
    vector<int> * sendConsigne();

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
    vector<int> taillesGroupes;

    int nbSourisUtilisees;
    int nbSourisAEcarter;
    long double globMoy, globMoyC;

    int nbGroupesImpairs;
    int bMin, bMax;

    vector<Groupe> groupes;
    vector<double> sourisDispo[2];
    vector<int> sourisDispoId[2];
    int tailleChromoAvecDernier, tailleChromoSansDernier;

    #define NB_MAX_SOURIS 300
    #define MAX_NB_GRP 100
    #define TAILLE_POP 300
    #define PRESSIONS_SELECTION 80 //lors de la selection, la proba que le meilleur individu soit selectionné est PRESSIONS_SELECTION / DIVISEUR_POURCENTAGES
    #define NB_INDIVIDUS_PARENTS 50
    #define MAX_NB_GENERATION 10000
    #define PROBA_MUTATION 0
    #define DIVISEUR_POURCENTAGES 100

    bool iGen = 0;

    int lastGrpId;


    short pop[2][TAILLE_POP][2][NB_MAX_SOURIS/2];//WARNING limite le nb de souris
    int scoreIndiv[TAILLE_POP];
    int iSouris;//attention à l'utilisation dans des fonctions

    int curTest = 0;
    int nbDejaVu[2][NB_MAX_SOURIS/2];
    RandTable tablePermutation[NB_MAX_SOURIS / 2];


    int bestBestScore;
    int idBestBestScore = 1000000;
    vector<int> memoryBestBestScore[2];


    long double moyGrp[MAX_NB_GRP], moyGrpC[MAX_NB_GRP];
    long double moyMoy, moyMoyC;
    int curReadId;
    int startReadId;
    int bestScore;
    int idBestScore;


    short iConcurrent[2];
    bool iSelec;


    int iParents[2];
    int iCoupure;
    int iSouris2;
    int i1, i2, iChromo, swapNb;

    short pas;

    vector<Souris> result;

};



#endif // ALGORITHME_H
