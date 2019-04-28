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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ui_tutoriel.h>
#include <thread>

#include <QtCore/QRandomGenerator>
#include <QTimer>
#include <qdialog.h>

#include <qdebug.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


ThreadCalculer threadCalculer;

QStandardItemModel *tableauDonnees = new QStandardItemModel(0,2);
QStandardItemModel *tableauGroupes = new QStandardItemModel(0,2);
QClipboard *clipboard = QApplication::clipboard();
QChart::AnimationOptions options(1);
int nbSourisCetteTaille[20];
QModelIndex q = QModelIndex();
QDialog *tutorielDialog;

QTimer *progressBarUpdater;
int threadProgression;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->chart()->setAnimationOptions(options);
    QChart *chart = new QChart();
    chartView->setChart(chart);

    ui->verticalLayout_2->addWidget(chartView);

    ui->tableView_1->setModel(tableauDonnees);
    ui->tableView_2->setModel(tableauGroupes);

    threadCalculer.setup(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGroupes()));
    timer->start(300);

    progressBarUpdater = new QTimer(this);
    connect(progressBarUpdater, SIGNAL(timeout()), this, SLOT(updateProgressBar()));


    connect(ui->getDataButton, SIGNAL(clicked(bool)), this, SLOT(onGetDataButtonClicked()));
    tutorielDialog = new QDialog;
    Ui::Dialog ui;
    ui.setupUi(tutorielDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGetDataButtonClicked()
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(5);
    ui->resultMessage->setText("");
    //initialisation : on supprime tous les resultats precedants
    while (tableauDonnees->rowCount() > 0) {
        tableauDonnees->removeRow(0);
    }
    maxTaille = 0;
    mSouris.clear();
    chartView->chart()->removeAllSeries();
    setPhase2Enabled(false);
    ui->progressBar->setValue(30);

    int resultat = getClipboardDatas();
    ui->progressBar->setValue(50);
    if(resultat == 0) {
        afficherTableauDatas();
        calculerParametresSerie();
        ui->progressBar->setValue(70);
        createSplineChart();
        ui->progressBar->setValue(90);
        setPhase2Enabled(true);
        chartView->setVisible(true);

    } else if(resultat == 1){
        afficherTableauDatas();
    }
    ui->progressBar->setValue(100);
    ui->progressBar->setVisible(false);
 }

int MainWindow::getClipboardDatas() {
    QStringList texte = clipboard->text().replace(',', '.').split("\n");

    QStringList item;

    double doubleValue;
    bool isDouble;
    int errorNumber = 0;
    int intValue;

    // lecture du clipboard et creation du tableau
    for (QString ligne : texte) {
        item = ligne.split("\t");
        if(item.length()== 2) {
            doubleValue =  item[1].toDouble(&isDouble);
            if (isDouble && doubleValue >= 0) {
                intValue = static_cast<int>(doubleValue*coefMultiplicateurDouble);
                mSouris.push_back({item[0], doubleValue, intValue});
                maxTaille = std::max(maxTaille, intValue);
            } else {
                errorNumber++;
                mSouris.push_back({item[0], -1, -1});
            }
        } else if (item.length() > 2) {
            errorNumber = -1;
            break;
        }
    }

    if(errorNumber == 0 && mSouris.size() > 0) { // donnees valides
        nbSouris = mSouris.size();
        ui->resultMessage->setText(QString("<html><head/><body><p><span style=\" color:#4e9a06;\">Lecture de %1 lignes réussie</span></p></body></html>").arg(nbSouris));
        return 0;
    } else {
        if(errorNumber > 0) {
             ui->resultMessage->setText(QString("<html><head/><body><p><span style=\" color:#cc0000;\">%1 erreur(s) dans la lecture des données</span></p></body></html>").arg(errorNumber));
             sort(mSouris.begin(), mSouris.end());
             return 1;
        } else {
            ui->resultMessage->setText("<html><head/><body><p><span style=\" color:#cc0000;\">Format incorrect (voir tutoriel)</span></p></body></html>");
        }
        return -1;
    }
}

void MainWindow::setPhase2Enabled(bool enabled) {
    ui->VisualisationGroupBox->setEnabled(enabled);
    ui->DefinirGrpGroupBox->setEnabled(enabled);
    ui->TailleGroupesGroupBox->setEnabled(false);
    ui->calculerButton->setEnabled(false);
}

void MainWindow::afficherTableauDatas()
{

    int row;
    for (Souris &s : mSouris) {
        row = tableauDonnees->rowCount();
        tableauDonnees->insertRow(row, {});
        tableauDonnees->setData(tableauDonnees->index(row,0, q), s.name);

        if (s.taille > 0) {
            tableauDonnees->setData(tableauDonnees->index(row,1,q), static_cast<double>(s.taille) / coefMultiplicateurDouble);
        } else {
            tableauDonnees->setData(tableauDonnees->index(row,1,q), "ERREUR");
        }
    }
}

void MainWindow::createSplineChart() {
  //  qDebug("Start creating SplineChart");

    QChart *chart = chartView->chart();
    chart->setTitle(QString( "<html><head/><body>"
                    "<p align=\"center\"><span style=\" font-weight:600;\">Données statistiques :</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Nombre d'éléments : %1</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Moyenne : %2</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Ecart-type : %3</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Médianne : %4</span></p>"
                    "</body></html>"
                    "<p align=\"center\"><span style=\" font-weight:600;\">Allure du la série :</span></p>")
                    .arg(nbSouris).arg(moyenne).arg(sqrt(variance)).arg(medianne));

    int nbPts = 10;

    int maxY = 0;
    int index;
    for (int iPt = 0; iPt <= nbPts; ++iPt) {
        nbSourisCetteTaille[iPt] = 0;
//        qDebug("%d", nbSourisCetteTaille[iPt]);
    }
    for (const Souris &s : mSouris) {
        index = s.taille * nbPts / maxTaille;
        nbSourisCetteTaille[index]++;
        maxY = std::max(maxY, nbSourisCetteTaille[index]);
      //  qDebug("index=%d  value=%d   MaxY=%d", index, nbSourisCetteTaille[index], maxY);
    }

    QSplineSeries *series = new QSplineSeries();
    for (int i = 0; i <= nbPts; i++) {
        qDebug("x=%d   y=%d",i*maxTaille/nbPts/coefMultiplicateurDouble, nbSourisCetteTaille[i]);
        series ->append(i*maxTaille/nbPts/coefMultiplicateurDouble, nbSourisCetteTaille[i]);
    }
    chart->addSeries(series);
    chart->legend()->setVisible(false);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, maxTaille / coefMultiplicateurDouble);

    chart->axes(Qt::Vertical).first()->setRange(-1, maxY+1);

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
   // axisY->setLabelFormat("%.1f  ");

    chartView->chart()->setAnimationOptions(options);

}

void MainWindow::on_validerButton_clicked(bool checked) {
    while(tableauGroupes->rowCount() > 0)
        tableauGroupes->removeRow(0);
    nbGroupes = ui->spinBox->value();
    if(nbGroupes > nbSouris || nbGroupes == 0) {
        ui->nbGroupeErrorLabel->setText("Nombre de groupes invalide");
        ui->TailleGroupesGroupBox->setEnabled(false);
        return;
    }
    ui->nbGroupeErrorLabel->setText("");

    int defautlGroupeSize = nbSouris / nbGroupes;
    for (int iGrp = 0; iGrp < nbGroupes; ++iGrp) {
        tableauGroupes->insertRow(0);
        tableauGroupes->setData(tableauGroupes->index(0,0,q), QString("Groupe %1").arg(nbGroupes - iGrp));
        tableauGroupes->setData(tableauGroupes->index(0,1,q), defautlGroupeSize);
    }
    ui->TailleGroupesGroupBox->setEnabled(true);
}

void MainWindow::updateGroupes()
{

    if(calculsEnCours || !ui->TailleGroupesGroupBox->isEnabled())
        return;
    int total = 0;
    for (int iGrp = 0; iGrp < nbGroupes; ++iGrp) {
        total += tableauGroupes->data(tableauGroupes->index(iGrp,1,q)).toInt();
    }
    ui->GroupeErrorLabel->setText(QString("Total : %1/%2 souris").arg(total).arg(nbSouris));
    ui->calculerButton->setEnabled(total <= nbSouris);
}


void MainWindow::on_calculerButton_clicked()
{
    if (calculsEnCours) {
        ui->DonneesGroupBox->setEnabled(true);
        ui->VisualisationGroupBox->setEnabled(true);
        ui->NbGroupesGroupBox->setEnabled(true);
        ui->TailleGroupesGroupBox->setEnabled(true);
        ui->calculerButton->setText("Calculer");
        stopGenetique();
    } else {
        while (!taillesGroupes.empty()) {
            taillesGroupes.pop_back();
        }
        nbSourisUtilisees = 0;
        for (int iGrp = 0; iGrp < nbGroupes; ++iGrp) {
            taillesGroupes.push_back(tableauGroupes->data(tableauGroupes->index(iGrp,1,q)).toInt());
            nbSourisUtilisees += taillesGroupes.back();
        }
        if(nbSourisUtilisees > nbSouris)
            return;
        calculsEnCours = true;
        ui->DonneesGroupBox->setEnabled(false);
        ui->VisualisationGroupBox->setEnabled(false);
        ui->NbGroupesGroupBox->setEnabled(false);
        ui->TailleGroupesGroupBox->setEnabled(false);
        ui->calculerButton->setText("Annuler");
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(true);
        threadCalculer.start(QThread::TimeCriticalPriority);
        progressBarUpdater->start(100);
    }

}

void MainWindow::calculerParametresSerie()
{
    sort(mSouris.begin(), mSouris.end());

    //moyenne
    moyenne = 0;
    for(Souris &s : mSouris) {
        moyenne += s.tailleOriginale;
    }

    moyenne /= nbSouris;
    variance = 0;
    for(Souris &s : mSouris) {
        variance += pow(s.tailleOriginale - moyenne, 2);
    }
    variance /= nbSouris;
    if(nbSouris % 2 == 0) {
        medianne = (mSouris[nbSouris / 2].tailleOriginale + mSouris[nbSouris / 2 - 1].tailleOriginale) / 2.0;
    } else {
        medianne = mSouris[nbSouris / 2].tailleOriginale;
    }
}

void MainWindow::on_helpButton_clicked() {

    tutorielDialog->show();

}


void MainWindow::updateProgressBar()
{
    ui->progressBar->setValue(threadProgression);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


double bestBestScore = 1000000;
void MainWindow::startGenetique()
{
    qDebug("startGenetique");
    bestBestScore = 1000000.0;
    calculsEnCours = true;

    threadProgression = 0;

    srand (time(NULL));
    init();
    long long t = time(NULL);
    int iGeneration = 0;
    while (calculsEnCours && iGeneration < MAX_NB_GENERATION) {
    //    qDebug("Gen %d - Time %d", iGeneration, time(NULL) - t);
        threadProgression = iGeneration * 100 / MAX_NB_GENERATION;
        evaluation();
        selection();
        iGen = !iGen;
        croisement();
    //    mutation();

        iGeneration++;
    }
    threadProgression = 100;

    qDebug("Gen %d - Time %d", iGeneration, time(NULL) - t);
    qDebug() << "Best" << bestBestScore;
}

void MainWindow::stopGenetique()
{
    qDebug("stopGenetique");
    progressBarUpdater->stop();
    ui->progressBar->setVisible(false);
    calculsEnCours = false;

   //TODO
}


void MainWindow::init()
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



void MainWindow::evaluation()
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


void MainWindow::selection() {

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


void MainWindow::croisement()
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

void MainWindow::mutation()
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
}
















