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

#include "algorithme.h"
#include "generalinclude.h"

struct Groupe {
    int tailleTotale;
    double valeurAdditionnelle;
};


struct RandTable {
    short id;
    int random;
    void update(short i) {
        id = i;
        random = rand();
    }
    bool operator < (RandTable &r) {
        return  random < r.random;
    }
};


#define NB_MAX_SOURIS 300
#define MAX_NB_GRP 50
#define TAILLE_POP 100
#define PRESSIONS_SELECTION 70
#define NB_INDIVIDUS_PARENTS 30
#define MAX_NB_GENERATION 100000
#define PROBA_MUTATION 1
#define DIVISEUR_POURCENTAGES 100

int nbGroupesImpairs,  nbSourisAEcarter;
int bMin, bMax;
double newMoy, newVariance;


bool iGen = 0;

vector<Groupe> groupes;
vector<double> sourisDispo[2];
int tailleChromo;

short pop[2][TAILLE_POP][NB_MAX_SOURIS/2][2];
long double scoreIndiv[TAILLE_POP];
int iSouris;

short curTest = 0;
short nbDejaVu[NB_MAX_SOURIS/2];
RandTable tablePermutation[NB_MAX_SOURIS / 2];



double moyGrp[MAX_NB_GRP], variGrp[MAX_NB_GRP];
double moyMoy, moyVari;
int curReadId;
int startReadId;
double bestScore;
int idBestScore;


int iConcurrent[2];
bool iSelec;


int iParents[2];
int iCoupure;
int iSouris2;
int i1, i2, iChromo, swapNb;

vector<Souris> * algorithmeTraitement::sendResult() {
    return &mSouris;
}

vector<int> *algorithmeTraitement::sendConsigne()
{
    return &tailleGroupes;
}


double bestBestScore = 1000000;

algorithmeTraitement::algorithmeTraitement(int *refProgression)
{
    threadProgression = refProgression;
}

void algorithmeTraitement::startGenetique()
{
    stopThread = false;
    qDebug("startGenetique");
    bestBestScore = 1000000.0;

   *threadProgression = 0;

    srand (time(NULL));
//    init();
    long long t = time(NULL);
    int iGeneration = 0;
    while (!stopThread && iGeneration < MAX_NB_GENERATION) {
 //       qDebug("Gen %d - Time %d", iGeneration, time(NULL) - t);
        *threadProgression = iGeneration * 99 / MAX_NB_GENERATION;
//        evaluation();
//        selection();
//        iGen = !iGen;
//        croisement();
    //    mutation();

        iGeneration++;
    }
    *threadProgression = 100;

    qDebug("Gen %d - Time %d", iGeneration, time(NULL) - t);
    qDebug() << "Best" << bestBestScore;
}

void algorithmeTraitement::stopGenetique()
{
    qDebug("stopGenetique");
    stopThread = true;
    //TODO
}

void algorithmeTraitement::setData(vector<Souris> *listeSouris, vector<int>* listeGroupes)
{
    mSouris = *listeSouris;
    tailleGroupes = *listeGroupes;
}
/*

void algorithmeTraitement::init()
{
  //  qDebug("init");

    //init Groupes

    nbGroupesImpairs = 0;
    for(int t : taillesGroupes) {
        nbGroupesImpairs += t % 2;
    }

    nbSourisAEcarter = nbSouris - nbSourisUtilisees;
    bMin = 0;
    bMax = mSouris.size() - 1;
    for (int i = 0; i < nbSourisAEcarter; ++i) {
        if(abs(mSouris[bMin].taille - moyenne) < abs(mSouris[bMax].taille - moyenne)) {
            bMax--;
        } else {
            bMin++;
        }
    }

    newMoy = 0.0;
    newVariance = 0;
    for (int iSouris = bMin; iSouris <= bMax; ++iSouris) {
        newMoy += mSouris[iSouris].tailleOriginale;
    }
    newMoy /= bMax - bMin + 1;
    for (int iSouris = bMin; iSouris <= bMax; ++iSouris) {
        newVariance += pow(newMoy - mSouris[iSouris].tailleOriginale, 2);
    }
    newVariance /= bMax - bMin + 1;

    while (bMax - bMin + 1 > nbGroupesImpairs){
        sourisDispo[0].push_back(mSouris.at(bMin).tailleOriginale);
        sourisDispo[1].push_back(mSouris.at(bMax).tailleOriginale);
        bMin++;
        bMax--;
    }

    for(int taille : taillesGroupes) {
   //     if(taillesGroupes)
        groupes.push_back({taille, -1});
    }

    for (int iSouris = bMin; iSouris <= bMax; ++iSouris) {
        groupes[iSouris - bMin].valeurAdditionnelle = mSouris.at(iSouris).tailleOriginale;
    }



    //init population

    tailleChromo = (nbSourisUtilisees - nbGroupesImpairs) / 2;

    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {

        for (int iSerie = 0; iSerie < 2; ++iSerie) {

            for (short iSouris = 0; iSouris < tailleChromo; ++iSouris) {
                tablePermutation[iSouris].update(iSouris);
            }

            sort(tablePermutation, tablePermutation + tailleChromo);

            for (int iSouris = 0; iSouris < tailleChromo; ++iSouris) {
                pop[iGen][iIndiv][iSouris][iSerie] = tablePermutation[iSouris].id;
            }
        }
    }

    for (int i = 0; i < tailleChromo; ++i) {
        nbDejaVu[i] = curTest;
    }
}


void algorithmeTraitement::evaluation()
{
    //qDebug("evaluation");
    bestScore = 1000*1000;
    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
        curReadId = 0;
        for (int iGroupe = 0; iGroupe < nbGroupes; ++iGroupe) {
            startReadId = curReadId;
            curReadId += groupes[iGroupe].tailleTotale / 2;
            if(groupes[iGroupe].valeurAdditionnelle > 0) {
                moyGrp[iGroupe] = groupes[iGroupe].valeurAdditionnelle;
            } else {
                moyGrp[iGroupe] = 0;
            }
            for (iSouris =  startReadId; iSouris < curReadId; ++iSouris) {
                for (int iSerie = 0; iSerie < 2; ++iSerie) {
                    moyGrp[iGroupe] += sourisDispo[iSerie].at(pop[iGen][iIndiv][iSouris][iSerie]);
                }
            }
            moyGrp[iGroupe] /= groupes[iGroupe].tailleTotale;

            if(groupes[iGroupe].valeurAdditionnelle > 0) {
                variGrp[iGroupe] = pow(groupes[iGroupe].valeurAdditionnelle-moyGrp[iGroupe], 2);
            } else {
                variGrp[iGroupe] = 0;
            }
            for (iSouris =  startReadId; iSouris < curReadId; ++iSouris) {
                for (int iSerie = 0; iSerie < 2; ++iSerie) {
                    variGrp[iGroupe] += pow(sourisDispo[iSerie].at(pop[iGen][iIndiv][iSouris][iSerie])-moyGrp[iGroupe], 2);;
                }
            }
            variGrp[iGroupe] /= groupes[iGroupe].tailleTotale;
        }
        moyMoy = 0;
        moyVari = 0;
        for (int iGroupe = 0; iGroupe < nbGroupes; ++iGroupe) {
            moyMoy += moyGrp[iGroupe];
            moyVari += variGrp[iGroupe];
        }
        moyMoy /= nbGroupes;
        moyVari /= nbGroupes;
        for (int iGroupe = 0; iGroupe < nbGroupes; ++iGroupe) {
            moyMoy = moyGrp[iGroupe];
            moyVari = variGrp[iGroupe];
        }
        scoreIndiv[iIndiv] = 0;
        for (int iGroupe = 0; iGroupe < nbGroupes; ++iGroupe) {
            scoreIndiv[iIndiv] += pow(moyMoy - moyGrp[iGroupe], 2);
            scoreIndiv[iIndiv] += abs(moyVari - variGrp[iGroupe]);
        }
        if(bestScore > scoreIndiv[iIndiv]) {
            bestScore = scoreIndiv[iIndiv];
            idBestScore = iIndiv;
        }
    }
    QDebug debug = qDebug();

    qDebug("bestScore=%lf", bestScore);
    double tot;
    tot = 0;
    for (int var = 0; var < 5; ++var) {
        debug << sourisDispo[0].at(pop[iGen][idBestScore][var][0]);
        tot += sourisDispo[0].at(pop[iGen][idBestScore][var][0]);
        debug << sourisDispo[1].at(pop[iGen][idBestScore][var][1]);
        tot += sourisDispo[1].at(pop[iGen][idBestScore][var][1]);
    }
    debug << "(" << tot << ")";
    debug << " --- ";
    tot = 0;
    for (int var = 5; var < 10; ++var) {
        debug << sourisDispo[0].at(pop[iGen][idBestScore][var][0]);
        tot += sourisDispo[0].at(pop[iGen][idBestScore][var][0]);
        debug << sourisDispo[1].at(pop[iGen][idBestScore][var][1]);
        tot += sourisDispo[1].at(pop[iGen][idBestScore][var][1]);
    }
    debug << "(" << tot << ")";
    bestBestScore = min(bestScore, bestBestScore);
}


void algorithmeTraitement::selection() {

  //  qDebug("selection");

    for (int iIndiv = 0; iIndiv < NB_INDIVIDUS_PARENTS; ++iIndiv) {
        iConcurrent[0] = rand() % TAILLE_POP;
        iConcurrent[1] = rand() % TAILLE_POP;
        iSelec = (scoreIndiv[iConcurrent[1]] < scoreIndiv[iConcurrent[0]]) ^ (rand() % DIVISEUR_POURCENTAGES > PRESSIONS_SELECTION);

        //copie de l'individu selectionne vers la generation suivante
        for (int iSouris = 0; iSouris < tailleChromo; ++iSouris) {
            for (int i = 0; i < 2; ++i) {
                pop[!iGen][iIndiv][iSouris][i] = pop[iGen][iConcurrent[iSelec]][iSouris][i];
            }
        }
    }
}


void algorithmeTraitement::croisement()
{
   // qDebug("croisement");

    for (int iIndiv = NB_INDIVIDUS_PARENTS; iIndiv < TAILLE_POP; iIndiv += 2) {
        iParents[0] = rand() % NB_INDIVIDUS_PARENTS;
        iParents[1] = rand() % NB_INDIVIDUS_PARENTS;

        iCoupure = rand() % tailleChromo;
        for (int iChrom = 0; iChrom < 2; ++iChrom) {

            curTest++;
            for (int iSouris = 0; iSouris < iCoupure; ++iSouris) {
                pop[iGen][iIndiv][iSouris][iChrom] = pop[iGen][iParents[0]][iSouris][iChrom];
                nbDejaVu[pop[iGen][iIndiv][iSouris][iChrom]] = curTest;
            }
            for (int iSouris = iCoupure; iSouris < tailleChromo; ++iSouris) {
                if (nbDejaVu[pop[iGen][iParents[1]][iSouris][iChrom]] == curTest) {
                    pop[iGen][iIndiv][iSouris][iChrom] = pop[iGen][iParents[1]][iSouris][iChrom];
                    nbDejaVu[pop[iGen][iIndiv][iSouris][iChrom]] = curTest;
                } else {
                    pop[iGen][iIndiv][iSouris][iChrom] = -1;
                }
            }
            iSouris = -1;
            iSouris2 = -1;
            while(iSouris < tailleChromo) {
                do {
                    iSouris++;
                } while(iSouris < tailleChromo && pop[iGen][iIndiv][iSouris][iChrom] != -1);
                if(iSouris < tailleChromo) {
                    do {
                        iSouris2++;
                    } while(nbDejaVu[pop[iGen][iParents[1]][iSouris2][iChrom]] == curTest);
                    pop[iGen][iIndiv][iSouris][iChrom] = pop[iGen][iParents[1]][iSouris2][iChrom];
                }
            }
        }


//        swapNb = iParents[0];
//        iParents[0] = iParents[1];
//        iParents[1] = swapNb;

    }
}

void algorithmeTraitement::mutation()
{

  //  qDebug("mutation");

    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
        if(rand() % DIVISEUR_POURCENTAGES < PROBA_MUTATION) {
            i1 = rand() % tailleChromo;
            i2 = rand() % tailleChromo;
            iChromo = rand() % 2;
            swapNb = pop[iGen][iIndiv][i1][iChromo];
            pop[iGen][iIndiv][i1][iChromo] = pop[iGen][iIndiv][i2][iChromo];
            pop[iGen][iIndiv][i2][iChromo] = swapNb;
        }
    }
}*/


void algorithmeTraitement::restartVariablesLocales()
{

}

