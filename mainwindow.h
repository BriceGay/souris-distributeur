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



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <qchart.h>
#include <qchartview.h>
#include <qclipboard.h>
#include <qsplineseries.h>
#include <qstandarditemmodel.h>
#include <qvalueaxis.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <QThread>


using namespace std;


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

struct Souris {
    QString name;
    double tailleOriginale;
    int taille;
    bool operator < (Souris & s) {
        return taille < s.taille;
    }
};


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



namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onGetDataButtonClicked();

    void on_validerButton_clicked(bool checked);
    void updateGroupes();
    void updateProgressBar();
    void on_calculerButton_clicked();

    void on_helpButton_clicked();

public:
    void createSplineChart();
    void afficherTableauDatas();
    int getClipboardDatas();
    void setPhase2Enabled(bool enabled);
    void calculerParametresSerie();




    void startGenetique();
    void stopGenetique();
    void init();
    void evaluation();
    void selection();
    void croisement();
    void mutation();

public:
    //souris
    int nbSouris;
    vector<Souris> mSouris;
    double coefMultiplicateurDouble = 1000;
    int maxTaille = -1;
    int nbGroupes;
    double moyenne;
    double variance;
    double medianne;
    int nbSourisUtilisees;
    vector<int> taillesGroupes;

    bool calculsEnCours = false;

    int m_valueMax;
    int m_valueCount;
    QList<QChartView *> m_charts;
    DataTable m_dataTable;

    // elements graphiques
    QChartView *chartView;
    Ui::MainWindow *ui;




    #define NB_MAX_SOURIS 300
    #define MAX_NB_GRP 50
    #define TAILLE_POP 100
    #define PRESSIONS_SELECTION 70
    #define NB_INDIVIDUS_PARENTS 30
    #define MAX_NB_GENERATION 10000
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

};

;


class ThreadCalculer : public QThread {
    public:
        MainWindow * mW;
        void setup(MainWindow * mainW) {
            mW = mainW;
        }
        void run() {
            mW->startGenetique();
        }
};

#endif // MAINWINDOW_H
