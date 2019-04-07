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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

struct Souris {
    QString name;
    double tailleOriginale;
    int taille;
    bool operator < (Souris & s) {
        return taille < s.taille;
    }
};

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;

public slots:
    void onGetDataButtonClicked();


private slots:
    void on_validerButton_clicked(bool checked);
    void updateGroupes();

    void on_calculerButton_clicked();

private:
    void createSplineChart();
    void afficherTableauDatas();
    int getClipboardDatas();
    void setPhase2Enabled(bool enabled);
    void lancerCalculs();
    void stopCalculs();
    void calculerParametresSerie();

private:
    //souris
    int nbSouris;
    vector<Souris> mSouris;
    double coefMultiplicateurDouble = 1000;
    int maxTaille = -1;
    int nbGroupes;
    double moyenne;
    double variance;
    double medianne;

    bool calculsEnCours = false;

    int m_valueMax;
    int m_valueCount;
    QList<QChartView *> m_charts;
    DataTable m_dataTable;

    // elements graphiques
    QChartView *chartView;
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
