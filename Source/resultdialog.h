#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <algorithme.h>
#include "generalinclude.h"
#include <qclipboard.h>
#include <QtCharts/QChartGlobal>
#include <qchart.h>
#include <qchartview.h>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE
namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent, algorithmeTraitement * algo);
    ~ResultDialog();

    void createSplineChart();
    QChartView *chartView;
public slots:
    void onCopieDataButtonClicked();

private:
    Ui::ResultDialog *ui;
    algorithmeTraitement * AT;
    vector<Souris> listSouris;
    vector<int> listGrp;
    QString result;
    QClipboard *clipboard;


};

#endif // RESULTDIALOG_H
