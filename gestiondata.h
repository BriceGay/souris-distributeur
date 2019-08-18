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




#ifndef GESTIONDATA_H
#define GESTIONDATA_H

#include "algorithme.h"
#include "generalinclude.h"
#include <qclipboard.h>

#include <qthread.h>

class ThreadCalculer : public QThread {
public:
    algorithmeTraitement * at;
    void setup(algorithmeTraitement * algT) {
        at = algT;
    }
    void run() {
        at->startGenetique();
    }
};






class gestionData
{
public:
    ThreadCalculer threadCalculer;
    gestionData(int *progressThread);

    QString getClipboardDatas();
    void calculerParametresSerie();

    algorithmeTraitement AT;


    vector<Souris> mSouris;
    int nbSouris;
    double maxTaille = -1;
    int nbGroupes;
    double moyenne;
    double variance;
    double medianne;
    int nbSourisUtilisees;
    vector<int> taillesGroupes;



    bool startCalculs();
    bool stopCalculs();

    QClipboard *clipboard;
};

#endif // GESTIONDATA_H
