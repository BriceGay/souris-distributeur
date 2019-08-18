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

class algorithmeTraitement
{
public:
    algorithmeTraitement(int * refProgression);
    void startGenetique();
    void stopGenetique();
    void setData(vector<Souris> * listeSouris, vector<int> * listeGroupes);
    vector<Souris> * sendResult();
    vector<int> * sendConsigne();
private:
    void init();
    void evaluation();
    void selection();
    void croisement();
    void mutation();
    void restartVariablesLocales();

    bool stopThread;
    vector<Souris> mSouris;
    vector<int> tailleGroupes;
    int * threadProgression;

};



#endif // ALGORITHME_H
