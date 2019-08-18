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


#include <math.h>
#include "gestiondata.h"
#include <qapplication.h>

gestionData::gestionData(int * progressThread) : AT(progressThread) {
    threadCalculer.setup(&AT);
    clipboard = QApplication::clipboard();
}


QString gestionData::getClipboardDatas() {


    QStringList texte = clipboard->text().replace(',', '.').split("\n");

    QStringList item;

    double value;
    bool isDouble;
    int errorNumber = 0;

    mSouris.clear();// on vide la liste actuelle
    maxTaille = 0;

    // lecture du clipboard et creation du tableau
    int id = 0;
    for (QString ligne : texte) {
        item = ligne.split("\t");
        if(item.length() == 2) {
            value = item[1].toDouble(&isDouble);
            if (isDouble && value >= 0) {//TODO Nombres positifs uniquement ?
                mSouris.push_back({item[0], id, value});
                id++;
                maxTaille = std::max(maxTaille, value);
            } else {
                errorNumber++;
                mSouris.push_back({item[0], -1});
            }
        } else if (item.length() > 2) {
            errorNumber = -1;
            break;
        }
    }
    QString resultMessage;
    if(errorNumber == 0 && mSouris.size() > 0) { // donnees valides
        nbSouris = mSouris.size();
        calculerParametresSerie();
        resultMessage = QString("<html><head/><body><p><span style=\" color:#4e9a06;\">Lecture de %1 lignes réussie</span></p></body></html>").arg(nbSouris);
    } else {
        if(errorNumber > 0) {
            resultMessage = QString("<html><head/><body><p><span style=\" color:#cc0000;\">%1 erreur(s) dans la lecture des données</span></p></body></html>").arg(errorNumber);
            sort(mSouris.begin(), mSouris.end());
       } else {
            resultMessage = "<html><head/><body><p><span style=\" color:#cc0000;\">Format incorrect (voir tutoriel)</span></p></body></html>";
        }
        return resultMessage;

    }
}

void gestionData::calculerParametresSerie()
{
    sort(mSouris.begin(), mSouris.end());

    //moyenne
    moyenne = 0;
    for(Souris &s : mSouris) {
        moyenne += s.taille;
    }

    moyenne /= nbSouris;
    variance = 0;
    for(Souris &s : mSouris) {
        variance += pow(s.taille - moyenne, 2);
    }
    variance /= nbSouris;
    if(nbSouris % 2 == 0) {
        medianne = (mSouris[nbSouris / 2].taille + mSouris[nbSouris / 2 - 1].taille) / 2.0;
    } else {
        medianne = mSouris[nbSouris / 2].taille;
    }
}

bool gestionData::startCalculs() {
    AT.setData(&mSouris, &taillesGroupes);
    threadCalculer.start(QThread::TimeCriticalPriority);
}

bool gestionData::stopCalculs() {
    AT.stopGenetique();
}
