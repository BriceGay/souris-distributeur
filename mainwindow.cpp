 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <algorithm>

#include <QtCore/QRandomGenerator>

#include <QTimer>

using namespace std;

QStandardItemModel *tableauDonnees = new QStandardItemModel(0,2);
QStandardItemModel *tableauGroupes = new QStandardItemModel(0,2);
QClipboard *clipboard = QApplication::clipboard();
QChart::AnimationOptions options(1);
int nbSourisCetteTaille[20];
QModelIndex q = QModelIndex();

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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGroupes()));
    timer->start(300);

    connect(ui->getDataButton, SIGNAL(clicked(bool)), this, SLOT(onGetDataButtonClicked()));
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
    int nbGrpsPlusGros = nbSouris % nbGroupes;
    for (int iGrp = 0; iGrp < nbGrpsPlusGros; ++iGrp) {
        tableauGroupes->insertRow(0);
        tableauGroupes->setData(tableauGroupes->index(0,0,q), QString("Groupe %1").arg(nbGroupes - iGrp));
        tableauGroupes->setData(tableauGroupes->index(0,1,q), defautlGroupeSize+1);
    }
    for (int iGrp = nbGrpsPlusGros; iGrp < nbGroupes; ++iGrp) {
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
    ui->calculerButton->setEnabled(total==nbSouris);
}


void MainWindow::on_calculerButton_clicked()
{
    if (calculsEnCours) {
        ui->DonneesGroupBox->setEnabled(true);
        ui->VisualisationGroupBox->setEnabled(true);
        ui->NbGroupesGroupBox->setEnabled(true);
        ui->TailleGroupesGroupBox->setEnabled(true);
        ui->calculerButton->setText("Calculer");
        calculsEnCours = false;
        stopCalculs();
    } else {
        int total = 0;
        for (int iGrp = 0; iGrp < nbGroupes; ++iGrp) {
            total += tableauGroupes->data(tableauGroupes->index(iGrp,1,q)).toInt();
        }
        if(total != nbSouris)
            return;
        calculsEnCours = true;
        ui->DonneesGroupBox->setEnabled(false);
        ui->VisualisationGroupBox->setEnabled(false);
        ui->NbGroupesGroupBox->setEnabled(false);
        ui->TailleGroupesGroupBox->setEnabled(false);
        ui->calculerButton->setText("Annuler");
        lancerCalculs();
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





void MainWindow::lancerCalculs() {
    qDebug("On calcule !!!");
}
void MainWindow::stopCalculs() {
    qDebug("On ne calcule plus !!!");
}


