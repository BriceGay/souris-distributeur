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

#include <QTimer>
#include <qdialog.h>

#include <QCloseEvent>
#include <QMessageBox>

#include "generalinclude.h"

#include "resultdialog.h"







QStandardItemModel *tableauDonnees = new QStandardItemModel(0,2);
QStandardItemModel *tableauGroupes = new QStandardItemModel(0,2);
QChart::AnimationOptions options(1);
QModelIndex q = QModelIndex();
QDialog *tutorielDialog;

QTimer *progressBarUpdater;
int threadProgression;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), GD(&threadProgression){

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



    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timer->start(300);

    connect(ui->getDataButton, SIGNAL(clicked(bool)), this, SLOT(onGetDataButtonClicked()));

    connect(ui->progressBar, SIGNAL(valueChanged(int)), this, SLOT(onProgressBarChanged()));


    tutorielDialog = new QDialog;
    Ui::Dialog ui;
    ui.setupUi(tutorielDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)  // show prompt when user wants to close app
{
    event->ignore();
    if (!calculsEnCours || QMessageBox::Yes == QMessageBox::question(this,  "Fermer ?", "Stoper les calculs et quitter ?", QMessageBox::Yes | QMessageBox::Cancel))
    {
        event->accept();
    }

}

void MainWindow::onGetDataButtonClicked()
{
    ui->getDataButton->setEnabled(false);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(5);
    ui->resultMessage->setText("");
    //initialisation : on supprime tous les resultats precedants
    while (tableauDonnees->rowCount() > 0) {
        tableauDonnees->removeRow(0);
    }
    chartView->chart()->removeAllSeries();
    setPhaseCreationGroupeEnabled(false);
    ui->progressBar->setValue(30);

    QString resultat = GD.getClipboardDatas();
    ui->resultMessage->setText(resultat);
    ui->progressBar->setValue(50);
    if(resultat.contains("réussie")) {
        afficherTableauDatas();
        ui->progressBar->setValue(70);
        createSplineChart();
        ui->progressBar->setValue(90);
        setPhaseCreationGroupeEnabled(true);
        chartView->setVisible(true);

    } else if(resultat.contains("erreur")){//une ou plusieurs données incorrectes
        afficherTableauDatas();
    }
    ui->progressBar->setValue(100);
    ui->progressBar->setVisible(false);
    ui->getDataButton->setEnabled(true);

}

void MainWindow::on_helpButton_clicked() {
    tutorielDialog->show();
}


void MainWindow::setPhaseCreationGroupeEnabled(bool enabled) {
    ui->VisualisationGroupBox->setEnabled(enabled);
    ui->DefinirGrpGroupBox->setEnabled(enabled);
    ui->TailleGroupesGroupBox->setEnabled(false);
    ui->calculerButton->setEnabled(false);
}

void MainWindow::afficherTableauDatas()
{
    int row;
    for (Souris &s : GD.mSouris) {
        row = tableauDonnees->rowCount();
        tableauDonnees->insertRow(row, {});
        tableauDonnees->setData(tableauDonnees->index(row,0, q), s.name);

        if (s.taille > 0) {
            tableauDonnees->setData(tableauDonnees->index(row,1,q), s.taille);
        } else {
            tableauDonnees->setData(tableauDonnees->index(row,1,q), "ERREUR");
        }
    }
}

void MainWindow::createSplineChart() {
  //  qDebug("Start creating SplineChart");
   int nbSourisCetteTaille[20];

    QChart *chart = chartView->chart();
    chart->setTitle(QString( "<html><head/><body>"
                    "<p align=\"center\"><span style=\" font-weight:600;\">Données statistiques :</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Nombre d'éléments : %1</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Moyenne : %2</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Ecart-type : %3</span></p>"
                    "<p align=\"center\"><span style=\" font-style:italic;\">Médianne : %4</span></p>"
                    "</body></html>"
                    "<p align=\"center\"><span style=\" font-weight:600;\">Allure du la série :</span></p>")
                    .arg(GD.nbSouris).arg(GD.moyenne).arg(sqrt(GD.variance)).arg(GD.medianne));

    int nbPts = 10;

    int maxY = 0;
    int index;
    for (int iPt = 0; iPt <= nbPts; ++iPt) {
        nbSourisCetteTaille[iPt] = 0;
//        qDebug("%d", nbSourisCetteTaille[iPt]);
    }
    for (const Souris &s : GD.mSouris) {
        index = s.taille * nbPts / GD.maxTaille;
        nbSourisCetteTaille[index]++;
        maxY = std::max(maxY, nbSourisCetteTaille[index]);
      //  qDebug("index=%d  value=%d   MaxY=%d", index, nbSourisCetteTaille[index], maxY);
    }

    QSplineSeries *series = new QSplineSeries();
    for (int i = 0; i <= nbPts; i++) {
        qDebug("x=%d   y=%d",i*GD.maxTaille/nbPts, nbSourisCetteTaille[i]);
        series ->append(i*GD.maxTaille/nbPts, nbSourisCetteTaille[i]);
    }
    chart->addSeries(series);
    chart->legend()->setVisible(false);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, GD.maxTaille);

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
    GD.nbGroupes = ui->spinBox->value();
    if(GD.nbGroupes > GD.nbSouris || GD.nbGroupes == 0) {
        ui->nbGroupeErrorLabel->setText("Nombre de groupes invalide");
        ui->TailleGroupesGroupBox->setEnabled(false);
        return;
    }
    ui->nbGroupeErrorLabel->setText("");

    int defautlGroupeSize = GD.nbSouris / GD.nbGroupes;
    for (int iGrp = 0; iGrp < GD.nbGroupes; ++iGrp) {
        tableauGroupes->insertRow(0);
        tableauGroupes->setData(tableauGroupes->index(0,0,q), QString("Groupe %1").arg(GD.nbGroupes - iGrp));
        tableauGroupes->setData(tableauGroupes->index(0,1,q), defautlGroupeSize);
    }
    ui->TailleGroupesGroupBox->setEnabled(true);
}

void MainWindow::updateTimer()
{

    if(calculsEnCours) {
        ui->progressBar->setValue(threadProgression);
    } else if (ui->TailleGroupesGroupBox->isEnabled()) {
        int total = 0;
        for (int iGrp = 0; iGrp < GD.nbGroupes; ++iGrp) {
            total += tableauGroupes->data(tableauGroupes->index(iGrp,1,q)).toInt();
        }
        ui->GroupeErrorLabel->setText(QString("Total : %1/%2 souris").arg(total).arg(GD.nbSouris));
        ui->calculerButton->setEnabled(total <= GD.nbSouris);
    }


}

void MainWindow::on_calculerButton_clicked()
{
    ui->calculerButton->setEnabled(false);

    if (calculsEnCours) {
        if(QMessageBox::Yes == QMessageBox::question(this,  "Confirmation", "Abandonner le calcul en cours ?", QMessageBox::Yes | QMessageBox::Cancel)) {
            setWindowEnabled(true);
            GD.stopCalculs();
            calculsEnCours = false;
        }


    } else {
        while (!GD.taillesGroupes.empty()) {
            GD.taillesGroupes.pop_back();
        }
        GD.nbSourisUtilisees = 0;
        for (int iGrp = 0; iGrp < GD.nbGroupes; ++iGrp) {
            GD.taillesGroupes.push_back(tableauGroupes->data(tableauGroupes->index(iGrp,1,q)).toInt());
            GD.nbSourisUtilisees += GD.taillesGroupes.back();
        }
        if(GD.nbSourisUtilisees > GD.nbSouris)
            return;
        calculsEnCours = true;
        setWindowEnabled(false);
        GD.startCalculs();
    }

    ui->calculerButton->setEnabled(true);
}

void MainWindow::setWindowEnabled(bool e) {
    ui->DonneesGroupBox->setEnabled(e);
    ui->VisualisationGroupBox->setEnabled(e);
    ui->NbGroupesGroupBox->setEnabled(e);
    ui->TailleGroupesGroupBox->setEnabled(e);
    ui->progressBar->setVisible(!e);
    ui->progressBar->setValue(0);

    if (e) {
        ui->calculerButton->setText("Calculer");
    } else {
        ui->calculerButton->setText("Annuler");
    }
}

void MainWindow::onProgressBarChanged()
{
    if(calculsEnCours && ui->progressBar->value() == 100) {
        calculsEnCours = false;
        qDebug("BIIIP");
        setWindowEnabled(true);
        ResultDialog r(this, &(GD.AT) );
        r.exec();
    }
}
