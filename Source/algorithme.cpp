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
    result.clear();
    int curReadId = 0;
    curTest++;
    for(int iGrp = 0; iGrp < groupes.size() - 1; iGrp++) {

        if(groupes[iGrp].taille % 2 == 1) {
            result.push_back(mSouris[groupes[iGrp].valAddiId]);
        }
        for (iSouris = 0; iSouris < groupes[iGrp].taille / 2; ++iSouris) {
            for (int iSerie = 0; iSerie < 2; ++iSerie) {
                result.push_back(mSouris.at( sourisDispoId[iSerie][ memoryBestBestScore[iSerie][curReadId] ] ) );
                nbDejaVu[iSerie][ memoryBestBestScore[iSerie][curReadId] ] = curTest + iSerie;

            }
            curReadId++;
        }
    }
    if(groupes.back().taille % 2 == 1) {
        result.push_back(mSouris[groupes.back().valAddiId]);
    }
    for (int iSerie = 0; iSerie < 2; ++iSerie) {
        for (iSouris = 0; iSouris < tailleChromoAvecDernier; ++iSouris) {
            if (nbDejaVu[iSerie][iSouris] != curTest + iSerie) {
                result.push_back(mSouris.at( sourisDispoId[iSerie][iSouris] ) );
                nbDejaVu[iSerie][iSouris] = curTest + iSerie;
            }
        }
    }
    curTest += 2;
    return &result;

}


vector<int> *algorithmeTraitement::sendConsigne()
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
//    qDebug("startGenetique");

    restartVariablesLocales();

    *threadProgression = 0;

    srand (time(NULL));

    init();
    long long t = time(NULL);
    int iGeneration = 0;
    while (!stopThread && iGeneration < MAX_NB_GENERATION) {
        qDebug("Gen %d - Time %d - Best %d - BestBest %d", iGeneration, time(NULL) - t, bestScore, bestBestScore);
        *threadProgression = iGeneration * 99 / MAX_NB_GENERATION;
        evaluation();
        selection();
        iGen = !iGen;
        croisement();
        mutation();

        iGeneration++;
    }
    *threadProgression = 100;

    qDebug("Gen %d - Time %d - Best %d - BestBest %d", iGeneration, time(NULL) - t, bestScore, bestBestScore);
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
    qDebug("init");

    //tri des tailles de grp par ordre croissant
    sort(taillesGroupes.begin(), taillesGroupes.end());

    //calcul GlobMoy
    globMoy = globMoyC = 0;
    for (Souris s : mSouris) {
        globMoy += s.taille;
    }
    globMoy /= static_cast<long double>(mSouris.size());

    //definition de nbSourisUtilisees
    nbSourisUtilisees = 0;
    for(int t : taillesGroupes)
        nbSourisUtilisees += t;

    //elimination des souris en trop
    nbSourisAEcarter = mSouris.size() - nbSourisUtilisees;
    bMin = 0;
    bMax = mSouris.size() - 1;
    for (int i = 0; i < nbSourisAEcarter; ++i) {
        if(abs(mSouris[bMin].taille - globMoy) < abs(mSouris[bMax].taille - globMoy)) {
            bMax--;
        } else {
            bMin++;
        }
    }

    //comptage grp impaires
    nbGroupesImpairs = 0;
    for(int t : taillesGroupes) {
        nbGroupesImpairs += t % 2;
    }


    //MAJ globMoy et globMoyC
    globMoy = globMoyC = 0;
    for (iSouris = bMin; iSouris <= bMax; ++iSouris) {
        globMoy += mSouris[iSouris].taille;
        globMoyC += pow(mSouris[iSouris].taille, 2);
    }
    globMoy /= static_cast<long double>(nbSourisUtilisees);
    globMoyC /= static_cast<long double>(nbSourisUtilisees);


    //separation en 2 grp autour de la medianne
    while (bMax - bMin + 1 > nbGroupesImpairs) {
        sourisDispo[0].push_back(mSouris[bMin].taille);
        sourisDispoId[0].push_back(bMin);
        sourisDispo[1].push_back(mSouris[bMax].taille);
        sourisDispoId[1].push_back(bMax);
        bMin++;
        bMax--;
    }

    //creation des groupes
    for(int taille : taillesGroupes) {
        groupes.push_back({taille, 0, 0});
        if(taille % 2 == 1) {
            groupes.back().valAddi = mSouris[bMin].taille;
            groupes.back().valAddiId = bMin;
            bMin++;
        }
    }


    //init population =
    tailleChromoAvecDernier = sourisDispo[0].size();
    tailleChromoSansDernier = tailleChromoAvecDernier - (taillesGroupes.back() / 2); // car arondi inferieur sur la division

    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
        for (int iSerie = 0; iSerie < 2; ++iSerie) {

            for (iSouris = 0; iSouris < tailleChromoAvecDernier; ++iSouris) {
                tablePermutation[iSouris].update(iSouris);
            }

            sort(tablePermutation, tablePermutation + tailleChromoAvecDernier);

            for (iSouris = 0; iSouris < tailleChromoSansDernier; ++iSouris) {
                pop[iGen][iIndiv][iSerie][iSouris] = tablePermutation[iSouris].id;
            }
        }
    }
    for (int iSerie = 0; iSerie < 2; ++iSerie) {
        for (int i = 0; i < tailleChromoSansDernier; ++i) {
            nbDejaVu[iSerie][i] = curTest;// pas besoin de reinitialiser
        }
    }
    lastGrpId = taillesGroupes.size() - 1;
}


void algorithmeTraitement::evaluation()
{
//    qDebug("evaluation");

    bestScore = 1000*1000*1000;
//    qDebug("%llf", bestScore);
    idBestScore = 0;
    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
//        qDebug() << "Evaluate indiv " << iIndiv << " from Gen " << iGen << "\n";
//        for (int iSerie = 0; iSerie < 2; ++iSerie) {
//            for (iSouris = 0; iSouris < 11; ++iSouris) {
//                qDebug() << pop[iGen][iIndiv][iSerie][iSouris] << " ";
//            }
//            qDebug() << "---";
//        }
//        qDebug() << "\n";

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
            for (iSouris = 0; iSouris < tailleChromoSansDernier; ++iSouris) {
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

    if (bestScore < bestBestScore) {
        bestBestScore = bestScore;
        idBestBestScore = idBestScore;
        for (int iSerie = 0; iSerie < 2; ++iSerie) {
            memoryBestBestScore[iSerie].clear();
            for (iSouris = 0; iSouris < tailleChromoSansDernier; ++iSouris) {
                memoryBestBestScore[iSerie].push_back(pop[iGen][idBestScore][iSerie][iSouris]);
            }
        }
    }
//    qDebug("bestScore=%d", bestScore);
//    qDebug("   bestBestScore=%d\n", bestBestScore);

//    long double tot;
//    tot = 0;
//    for (int var = 0; var < tailleChromoSansDernier; ++var) {

//        qDebug() << sourisDispo[0].at(pop[iGen][idBestScore][0][var]);
//        tot += sourisDispo[0].at(pop[iGen][idBestScore][0][var]);
//        qDebug() << sourisDispo[1].at(pop[iGen][idBestScore][1][var]);
//        tot += sourisDispo[1].at(pop[iGen][idBestScore][1][var]);
//    }
//    qDebug() << "(" << (double)tot << ")";
//    qDebug() << " --- ";

}


void algorithmeTraitement::selection() {

//    qDebug("selection");

    for (int iIndiv = 0; iIndiv < NB_INDIVIDUS_PARENTS; ++iIndiv) {
        iConcurrent[0] = rand() % TAILLE_POP; //TODO une methode qui fait passer un duel à tous les individus ?
        iConcurrent[1] = rand() % TAILLE_POP;
        iSelec = (scoreIndiv[iConcurrent[1]] < scoreIndiv[iConcurrent[0]]) xor (rand() % DIVISEUR_POURCENTAGES > PRESSIONS_SELECTION);

        //copie de l'individu selectionne vers la generation suivante
        for (int i = 0; i < 2; ++i) {
            for (int iSouris = 0; iSouris < tailleChromoAvecDernier; ++iSouris) {
                pop[!iGen][iIndiv][i][iSouris] = pop[iGen][iConcurrent[iSelec]][i][iSouris];
            }
        }
    }
}


void algorithmeTraitement::croisement()
{
//    qDebug("croisement");

    for (int iIndiv = NB_INDIVIDUS_PARENTS; iIndiv < TAILLE_POP; iIndiv ++) {
        iParents[0] = rand() % NB_INDIVIDUS_PARENTS;
        iParents[1] = rand() % NB_INDIVIDUS_PARENTS;

        iCoupure = rand() % (tailleChromoSansDernier + 1);
        for (int iChrom = 0; iChrom < 2; ++iChrom) {

            curTest++;
            for (int iSouris = 0; iSouris < iCoupure; ++iSouris) {
                pop[iGen][iIndiv][iChrom][iSouris] = pop[iGen][ iParents[0] ][iChrom][iSouris];
                nbDejaVu[iChrom][ pop[iGen][iIndiv][iChrom][iSouris] ] = curTest;
            }
            for (int iSouris = iCoupure; iSouris < tailleChromoSansDernier; ++iSouris) {
                if (nbDejaVu[iChrom][ pop[iGen][ iParents[1] ][iChrom][iSouris] ] != curTest) {
                    pop[iGen][iIndiv][iChrom][iSouris] = pop[iGen][iParents[1]][iChrom][iSouris];
                    nbDejaVu[iChrom][ pop[iGen][iIndiv][iChrom][iSouris] ] = curTest;
                } else {
                    pop[iGen][iIndiv][iChrom][iSouris] = -1;//WARNING negatif dans short
                }
            }

            pas = (rand() % 2 - 1) * 2 + 1; // pas = +1 ou -1
            iSouris2 = rand() % tailleChromoAvecDernier;

            for (int iSouris = 0; iSouris < tailleChromoSansDernier; ++iSouris) {
                if(pop[iGen][iIndiv][iChrom][iSouris] == -1) {

                    while (nbDejaVu[iChrom][iSouris2] == curTest) {
                        iSouris2 += pas;
                        if(iSouris2 == -1)
                            iSouris2 = tailleChromoAvecDernier - 1;
                        else if(iSouris2 == tailleChromoAvecDernier)
                            iSouris2 = 0;
                    }
                    nbDejaVu[iChrom][iSouris2] = curTest;
                    pop[iGen][iIndiv][iChrom][iSouris] = iSouris2;

                }
            }



        }
//        qDebug() << "After Croisement of " << iIndiv << " from Gen " << iGen << "\n";
//        for (int iSerie = 0; iSerie < 2; ++iSerie) {
//            for (iSouris = 0; iSouris < 5; ++iSouris) {
//                qDebug() << pop[iGen][iIndiv][iSerie][iSouris] << " ";
//                if(pop[iGen][iIndiv][iSerie][iSouris] == -1)
//                    qErrnoWarning("ERROR");
//            }
//            qDebug() << "---";
//        }
//        qDebug() << "\n";


//        swapNb = iParents[0];
//        iParents[0] = iParents[1];
//        iParents[1] = swapNb;

    }
}

void algorithmeTraitement::mutation()
{

//    qDebug("mutation");

    for (int iIndiv = 0; iIndiv < TAILLE_POP; ++iIndiv) {
        if(rand() % DIVISEUR_POURCENTAGES < PROBA_MUTATION) {
            i1 = rand() % tailleChromoAvecDernier;
            i2 = rand() % tailleChromoAvecDernier;
            iChromo = rand() % 2;
            for (iSouris = 0; iSouris < tailleChromoSansDernier; ++iSouris) {
                if(pop[iGen][iIndiv][iChromo][iSouris] == i1) {
                    pop[iGen][iIndiv][iChromo][iSouris] = i2;
                } else if (pop[iGen][iIndiv][iChromo][iSouris] == i2) {
                    pop[iGen][iIndiv][iChromo][iSouris] = i1;
                }
            }
        }
    }
}


void algorithmeTraitement::restartVariablesLocales()
{
    sourisDispo[0].clear();
    sourisDispo[1].clear();
    sourisDispoId[0].clear();
    sourisDispoId[1].clear();
    groupes.clear();
    bestBestScore = 1000*1000*1000;
    idBestBestScore = 0;
}

