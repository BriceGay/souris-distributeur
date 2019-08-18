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
#include <math.h>


vector<Souris> * algorithmeTraitement::sendResult() {
    return &mSouris;
}

vector<unsigned int> *algorithmeTraitement::sendConsigne()
{
    return &taillesGroupes;
}


algorithmeTraitement::algorithmeTraitement(int *refProgression)
{
    threadProgression = refProgression;
}

void algorithmeTraitement::startGenetique()
{
    stopThread = false;
    qDebug("startGenetique");
    bestBestScore = 1000*1000.0;

   *threadProgression = 0;

    srand (time(NULL));

    init();
    long long t = time(NULL);
    int iGeneration = 0;
    while (!stopThread && iGeneration < MAX_NB_GENERATION) {
        qDebug("Gen %d - Time %d", iGeneration, time(NULL) - t);
        *threadProgression = iGeneration * 99 / MAX_NB_GENERATION;
        evaluation();
        selection();
        iGen = !iGen;
        croisement();
        mutation();

        iGeneration++;
    }
    *threadProgression = 100;

    qDebug("Gen %d - Time %d", iGeneration, time(NULL) - t);
    qDebug() << "Best=" << bestBestScore;
}

void algorithmeTraitement::stopGenetique()
{
    qDebug("stopGenetique");
    stopThread = true;
    //TODO
}

void algorithmeTraitement::setData(vector<Souris> *listeSouris, vector<int>* listeGroupes)
{
    restartVariablesLocales();
    mSouris = *listeSouris;
    taillesGroupes = *listeGroupes;
}


void algorithmeTraitement::init()
{
  //  qDebug("init");

    //tri des tailles de grp par ordre croissant
    sort(taillesGroupes.begin(), taillesGroupes.end());

    //calcul GlobMoy
    globMoy = globMoyC = 0;
    for (Souris s : mSouris) {
        globMoy += s.taille;
    }
    globMoy /= static_cast<double>(mSouris.size());

    //definition de nbSourisUtilisees
    nbSourisUtilisees = 0;
    for(unsigned int t : taillesGroupes)
        nbSourisUtilisees += t;

    //elimination des souris en trop
    nbSourisAEcarter = mSouris.size() - nbSourisUtilisees;
    bMin = 0;
    bMax = mSouris.size() - 1;
    for (unsigned int i = 0; i < nbSourisAEcarter; ++i) {
        if(abs(mSouris[bMin].taille - globMoy) < abs(mSouris[bMax].taille - globMoy)) {
            bMax--;
        } else {
            bMin++;
        }
    }

    //comptage grp impaires
    nbGroupesImpairs = 0;
    for(unsigned int t : taillesGroupes) {
        nbGroupesImpairs += t % 2;
    }


    //MAJ globMoy et globMoyC
    globMoy = globMoyC = 0;
    for (iSouris = bMin; iSouris <= bMax; ++iSouris) {
        globMoy += mSouris[iSouris].taille;
        globMoyC += pow(mSouris[iSouris].taille, 2);
    }
    globMoy /= static_cast<double>(nbSourisUtilisees);
    globMoyC /= static_cast<double>(nbSourisUtilisees);


    //separation en 2 grp autour de la medianne
    while (bMax - bMin + 1 > nbGroupesImpairs) {
        sourisDispo[0].push_back(mSouris[bMin].taille);
        sourisDispo[1].push_back(mSouris[bMax].taille);
        bMin++;
        bMax--;
    }

    //creation des groupes
    for(unsigned int taille : taillesGroupes) {
        groupes.push_back({taille, 0});
        if(taille % 2 == 1) {
            groupes.back().valAddi = mSouris[bMin].taille;
            bMin++;
        }
    }


    //init population =
    tailleChromoTheorique = sourisDispo[0].size();
    tailleChromoReel = tailleChromoTheorique - (taillesGroupes.back() / 2); // car arondi inferieur sur la division

    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
        for (int iSerie = 0; iSerie < 2; ++iSerie) {

            for (iSouris = 0; iSouris < tailleChromoTheorique; ++iSouris) {
                tablePermutation[iSouris].update(iSouris);
            }

            sort(tablePermutation, tablePermutation + tailleChromoTheorique);

            for (iSouris = 0; iSouris < tailleChromoReel; ++iSouris) {
                pop[iGen][iIndiv][iSerie][iSouris] = tablePermutation[iSouris].id;
            }
        }
    }
    for (int iSerie = 0; iSerie < 2; ++iSerie) {
        for (unsigned int i = 0; i < tailleChromoReel; ++i) {
            nbDejaVu[iSerie][i] = curTest;// pas besoin de reinitialiser
        }
    }
    lastGrpId = taillesGroupes.size() - 1;
}


void algorithmeTraitement::evaluation()
{
    //qDebug("evaluation");

    bestScore = 1000*1000;

    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
        curReadId = 0;
        curTest++;

        //calcul pr grp reels
        for (int iGroupe = 0; iGroupe < lastGrpId; ++iGroupe) {
            startReadId = curReadId;
            curReadId += groupes[iGroupe].taille / 2;

            moyGrp[iGroupe] = groupes[iGroupe].valAddi;
            moyGrpC[iGroupe] = pow(groupes[iGroupe].valAddi, 2);

            for (int iSerie = 0; iSerie < 2; ++iSerie) {
                for (iSouris = startReadId; iSouris < curReadId; ++iSouris) {
                    moyGrp[iGroupe] += sourisDispo[iSerie].at(pop[iGen][iIndiv][iSerie][iSouris]);
                    moyGrpC[iGroupe] += pow(sourisDispo[iSerie].at(pop[iGen][iIndiv][iSerie][iSouris]) , 2);
                    nbDejaVu[iSerie][ pop[iGen][iIndiv][iSerie][iSouris] ] = curTest;
                }
            }
        }

        //calcul pr dernier grp
        moyGrp[lastGrpId] = groupes[lastGrpId].valAddi;
        moyGrpC[lastGrpId] = pow(groupes[lastGrpId].valAddi, 2);
        for (int iSerie = 0; iSerie < 2; ++iSerie) {
            for (iSouris = 0; iSouris < tailleChromoReel; ++iSouris) {
                if(nbDejaVu[iSerie][iSouris] != curTest) {
                    nbDejaVu[iSerie][iSouris] = curTest; // pour eviter les bug
                    moyGrp[lastGrpId] += sourisDispo[iSerie][iSouris];
                    moyGrpC[lastGrpId] += pow(sourisDispo[iSerie][iSouris], 2);
                }
            }
        }

        //calcul score
        scoreIndiv[iIndiv] = 0;
        for (int iGroupe = 0; iGroupe < taillesGroupes.size(); ++iGroupe) {
            scoreIndiv[iIndiv] += pow(globMoy - moyGrp[iGroupe], 2);
            scoreIndiv[iIndiv] += abs(globMoyC - moyGrpC[iGroupe]);
        }
        if(bestScore > scoreIndiv[iIndiv]) {
            bestScore = scoreIndiv[iIndiv];
            idBestScore = iIndiv;
        }
    }

    bestBestScore = min(bestScore, bestBestScore);
    qDebug("bestScore=%lf", bestScore);

    double tot;
    tot = 0;
    for (int var = 0; var < 5; ++var) {
        qDebug() << sourisDispo[0].at(pop[iGen][idBestScore][0][var]);
        tot += sourisDispo[0].at(pop[iGen][idBestScore][0][var]);
        qDebug() << sourisDispo[1].at(pop[iGen][idBestScore][1][var]);
        tot += sourisDispo[1].at(pop[iGen][idBestScore][1][var]);
    }
    qDebug() << "(" << tot << ")";
    qDebug() << " --- ";
    tot = 0;
    for (int var = 5; var < 10; ++var) {
        qDebug() << sourisDispo[0].at(pop[iGen][idBestScore][0][var]);
        tot += sourisDispo[0].at(pop[iGen][idBestScore][0][var]);
        qDebug() << sourisDispo[1].at(pop[iGen][idBestScore][1][var]);
        tot += sourisDispo[1].at(pop[iGen][idBestScore][1][var]);
    }
    qDebug() << "(" << tot << ")";

}


void algorithmeTraitement::selection() {

  //  qDebug("selection");

    for (int iIndiv = 0; iIndiv < NB_INDIVIDUS_PARENTS; ++iIndiv) {
        iConcurrent[0] = rand() % TAILLE_POP; //TODO une methode qui fait passer un duel à tous les individus ?
        iConcurrent[1] = rand() % TAILLE_POP;
        iSelec = (scoreIndiv[iConcurrent[1]] < scoreIndiv[iConcurrent[0]]) xor (rand() % DIVISEUR_POURCENTAGES > PRESSIONS_SELECTION);

        //copie de l'individu selectionne vers la generation suivante
        for (int i = 0; i < 2; ++i) {
            for (int iSouris = 0; iSouris < tailleChromoTheorique; ++iSouris) {
                pop[!iGen][iIndiv][i][iSouris] = pop[iGen][iConcurrent[iSelec]][i][iSouris];
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
                Pop[iGen][iIndiv][iSouris][iChrom] = Pop[iGen][iParents[0]][iSouris][iChrom];
                nbDejaVu[Pop[iGen][iIndiv][iSouris][iChrom]] = curTest;
            }
            for (int iSouris = iCoupure; iSouris < tailleChromo; ++iSouris) {
                if (nbDejaVu[Pop[iGen][iParents[1]][iSouris][iChrom]] == curTest) {
                    Pop[iGen][iIndiv][iSouris][iChrom] = Pop[iGen][iParents[1]][iSouris][iChrom];
                    nbDejaVu[Pop[iGen][iIndiv][iSouris][iChrom]] = curTest;
                } else {
                    Pop[iGen][iIndiv][iSouris][iChrom] = -1;
                }
            }
            iSouris = -1;
            iSouris2 = -1;
            while(iSouris < tailleChromo) {
                do {
                    iSouris++;
                } while(iSouris < tailleChromo && Pop[iGen][iIndiv][iSouris][iChrom] != -1);
                if(iSouris < tailleChromo) {
                    do {
                        iSouris2++;
                    } while(nbDejaVu[Pop[iGen][iParents[1]][iSouris2][iChrom]] == curTest);
                    Pop[iGen][iIndiv][iSouris][iChrom] = Pop[iGen][iParents[1]][iSouris2][iChrom];
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
            swapNb = Pop[iGen][iIndiv][i1][iChromo];
            Pop[iGen][iIndiv][i1][iChromo] = Pop[iGen][iIndiv][i2][iChromo];
            Pop[iGen][iIndiv][i2][iChromo] = swapNb;
        }
    }
}


void algorithmeTraitement::restartVariablesLocales()
{
    sourisDispo[0].clear();
    sourisDispo[1].clear();
    groupes.clear();
}

