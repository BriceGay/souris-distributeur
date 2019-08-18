#include "resultdialog.h"
#include "ui_resultdialog.h"
#include <QScatterSeries>
#include <QValueAxis>
#include <math.h>

QChart::AnimationOptions optionsBis(1);

ResultDialog::ResultDialog(QWidget *parent, algorithmeTraitement * algo) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    connect(ui->copieDataButton, SIGNAL(clicked(bool)), this, SLOT(onCopieDataButtonClicked()));

    AT = algo;

    listSouris = * (AT->sendResult());
    listGrp = * (AT->sendConsigne());

    qDebug() << listGrp.size();


    //generate clipboard result message et calcule des moyennes / variances
    result = "";
    vector<double> moyennes, ecType;
    double minMoy = 1000*1000*1000;
    double minEcType = 1000*1000*1000;
    double maxMoy = 0;
    double maxEcType = 0;
    int idRead = 0;
    for (int iGrp = 0; iGrp < listGrp.size(); ++iGrp) {
        moyennes.push_back(0);
        ecType.push_back(0);
        result.append(QString("Groupe %1\n").arg(iGrp + 1));
        for (int iSouris = 0; iSouris < listGrp[iGrp]; ++iSouris) {
            moyennes.back() += listSouris[idRead].taille;
            ecType.back() += pow(listSouris[idRead].taille, 2);
            result.append(QString("%1\t%2\n").arg(listSouris[idRead].name).arg(listSouris[idRead].taille));
            idRead ++;
        }
        moyennes.back() /= listGrp[iGrp];
        ecType.back() = sqrt(ecType.back()/listGrp[iGrp] - pow(moyennes.back(), 2));
        minMoy = min(minMoy, moyennes.back());
        maxMoy = max(maxMoy, moyennes.back());
        minEcType = min(minEcType, ecType.back());
        maxEcType = max(maxEcType, ecType.back());
        result.append(QString("MOY\t%1\nET\t%2\n\n").arg(moyennes.back()).arg(ecType.back()));
    }

    qDebug() << "minMoy=" << minMoy << "\nmaxMoy=" << maxMoy << "\nminEcType=" << minEcType << "\nmaxEcType=" << maxEcType;

    clipboard = QApplication::clipboard();
    result.replace('.', ',');
    clipboard->setText(result);
    qDebug() << "Resultats :" << endl << result;


    qDebug("Creating ScatterChart");

    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->chart()->setAnimationOptions(optionsBis);
    QChart *chart = new QChart();
    chartView->setChart(chart);

    ui->horizontalLayout->addWidget(chartView);



     int nbSourisCetteTaille[20];

      chart->setTitle(QString( "<html><head/><body>"
                      "<p align=\"center\"><span style=\" font-weight:600;\">Vos souris ont été réparties dans %1 groupes</span></p>"
                      "<p align=\"center\"><span style=\" font-style:italic;\">Moyenne min : %2</span></p>"
                      "<p align=\"center\"><span style=\" font-style:italic;\">Moyenne max : %3</span></p>"
                      "<p align=\"center\"><span style=\" font-style:italic;\">Ecart-type min: %4</span></p>"
                      "<p align=\"center\"><span style=\" font-style:italic;\">Ecart-type max : %5</span></p>"
                      " </body></head></html>")
                      .arg(listGrp.size()).arg(minMoy).arg(maxMoy).arg(minEcType).arg(maxEcType));


      QScatterSeries *series;
      idRead = 0;
      double maxTailleSouris = 0;
      for (int iGrp = 0; iGrp < listGrp.size(); ++iGrp) {
          series = new QScatterSeries();
          for (int iSouris = 0; iSouris < listGrp[iGrp]; ++iSouris) {
              series ->append(iGrp + 1, listSouris[idRead].taille);
              maxTailleSouris = max(maxTailleSouris, listSouris[idRead].taille);
              idRead++;
          }
          chart->addSeries(series);
      }

      chart->legend()->setVisible(false);
      chart->createDefaultAxes();
      chart->axes(Qt::Horizontal).first()->setRange(0, static_cast<int>(listGrp.size()) + 1);

      chart->axes(Qt::Vertical).first()->setRange(0, maxTailleSouris * 1.01);

      // Add space to label to add space between labels and axis
      QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
      Q_ASSERT(axisY);
     // axisY->setLabelFormat("%.1f  ");

      chartView->chart()->setAnimationOptions(optionsBis);

}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::onCopieDataButtonClicked()
{
    clipboard->setText(result);
}

void ResultDialog::createSplineChart() {

}
