/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *DonneesGroupBox;
    QVBoxLayout *verticalLayout_4;
    QPushButton *getDataButton;
    QPushButton *helpButton;
    QLabel *resultMessage;
    QTableView *tableView_1;
    QGroupBox *VisualisationGroupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *DefinirGrpGroupBox;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *NbGroupesGroupBox;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spinBox;
    QPushButton *validerButton;
    QLabel *nbGroupeErrorLabel;
    QGroupBox *TailleGroupesGroupBox;
    QVBoxLayout *verticalLayout_6;
    QTableView *tableView_2;
    QLabel *GroupeErrorLabel;
    QPushButton *calculerButton;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(844, 495);
        MainWindow->setBaseSize(QSize(1000, 500));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/logoVert.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setIconSize(QSize(200, 200));
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        DonneesGroupBox = new QGroupBox(centralWidget);
        DonneesGroupBox->setObjectName(QString::fromUtf8("DonneesGroupBox"));
        DonneesGroupBox->setMaximumSize(QSize(300, 16777215));
        verticalLayout_4 = new QVBoxLayout(DonneesGroupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        getDataButton = new QPushButton(DonneesGroupBox);
        getDataButton->setObjectName(QString::fromUtf8("getDataButton"));

        verticalLayout_4->addWidget(getDataButton);

        helpButton = new QPushButton(DonneesGroupBox);
        helpButton->setObjectName(QString::fromUtf8("helpButton"));

        verticalLayout_4->addWidget(helpButton);

        resultMessage = new QLabel(DonneesGroupBox);
        resultMessage->setObjectName(QString::fromUtf8("resultMessage"));
        resultMessage->setTextFormat(Qt::AutoText);
        resultMessage->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout_4->addWidget(resultMessage);

        tableView_1 = new QTableView(DonneesGroupBox);
        tableView_1->setObjectName(QString::fromUtf8("tableView_1"));
        tableView_1->setMinimumSize(QSize(220, 0));
        tableView_1->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        tableView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_1->setAlternatingRowColors(true);
        tableView_1->setSelectionMode(QAbstractItemView::NoSelection);
        tableView_1->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        tableView_1->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableView_1->setShowGrid(true);
        tableView_1->setGridStyle(Qt::DashLine);
        tableView_1->setSortingEnabled(true);
        tableView_1->setWordWrap(false);
        tableView_1->setCornerButtonEnabled(false);
        tableView_1->horizontalHeader()->setVisible(false);
        tableView_1->horizontalHeader()->setCascadingSectionResizes(true);
        tableView_1->horizontalHeader()->setHighlightSections(false);
        tableView_1->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(tableView_1);


        horizontalLayout->addWidget(DonneesGroupBox);

        VisualisationGroupBox = new QGroupBox(centralWidget);
        VisualisationGroupBox->setObjectName(QString::fromUtf8("VisualisationGroupBox"));
        VisualisationGroupBox->setEnabled(false);
        VisualisationGroupBox->setMinimumSize(QSize(300, 400));
        verticalLayout_3 = new QVBoxLayout(VisualisationGroupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout->addWidget(VisualisationGroupBox);

        DefinirGrpGroupBox = new QGroupBox(centralWidget);
        DefinirGrpGroupBox->setObjectName(QString::fromUtf8("DefinirGrpGroupBox"));
        DefinirGrpGroupBox->setEnabled(false);
        DefinirGrpGroupBox->setMaximumSize(QSize(300, 16777215));
        verticalLayout_5 = new QVBoxLayout(DefinirGrpGroupBox);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        NbGroupesGroupBox = new QGroupBox(DefinirGrpGroupBox);
        NbGroupesGroupBox->setObjectName(QString::fromUtf8("NbGroupesGroupBox"));
        horizontalLayout_2 = new QHBoxLayout(NbGroupesGroupBox);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spinBox = new QSpinBox(NbGroupesGroupBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout_2->addWidget(spinBox);

        validerButton = new QPushButton(NbGroupesGroupBox);
        validerButton->setObjectName(QString::fromUtf8("validerButton"));

        horizontalLayout_2->addWidget(validerButton);


        verticalLayout_5->addWidget(NbGroupesGroupBox);

        nbGroupeErrorLabel = new QLabel(DefinirGrpGroupBox);
        nbGroupeErrorLabel->setObjectName(QString::fromUtf8("nbGroupeErrorLabel"));

        verticalLayout_5->addWidget(nbGroupeErrorLabel);

        TailleGroupesGroupBox = new QGroupBox(DefinirGrpGroupBox);
        TailleGroupesGroupBox->setObjectName(QString::fromUtf8("TailleGroupesGroupBox"));
        verticalLayout_6 = new QVBoxLayout(TailleGroupesGroupBox);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        tableView_2 = new QTableView(TailleGroupesGroupBox);
        tableView_2->setObjectName(QString::fromUtf8("tableView_2"));
        tableView_2->setMinimumSize(QSize(220, 0));
        tableView_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableView_2->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);

        verticalLayout_6->addWidget(tableView_2);


        verticalLayout_5->addWidget(TailleGroupesGroupBox);

        GroupeErrorLabel = new QLabel(DefinirGrpGroupBox);
        GroupeErrorLabel->setObjectName(QString::fromUtf8("GroupeErrorLabel"));

        verticalLayout_5->addWidget(GroupeErrorLabel);

        calculerButton = new QPushButton(DefinirGrpGroupBox);
        calculerButton->setObjectName(QString::fromUtf8("calculerButton"));

        verticalLayout_5->addWidget(calculerButton);


        horizontalLayout->addWidget(DefinirGrpGroupBox);


        verticalLayout->addLayout(horizontalLayout);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setValue(0);
        progressBar->setTextVisible(true);

        verticalLayout->addWidget(progressBar);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 844, 22));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "R\303\251partisseur de souris", nullptr));
        DonneesGroupBox->setTitle(QApplication::translate("MainWindow", "1 - R\303\251cup\303\251rez vos donn\303\251es", nullptr));
        getDataButton->setText(QApplication::translate("MainWindow", "Charger les donn\303\251es", nullptr));
        helpButton->setText(QApplication::translate("MainWindow", "Tutoriel", nullptr));
        resultMessage->setText(QString());
        VisualisationGroupBox->setTitle(QApplication::translate("MainWindow", "2 - Visualisation", nullptr));
        DefinirGrpGroupBox->setTitle(QApplication::translate("MainWindow", "3 - D\303\251finissez vos groupes", nullptr));
        NbGroupesGroupBox->setTitle(QApplication::translate("MainWindow", "Nombre de groupes", nullptr));
        validerButton->setText(QApplication::translate("MainWindow", "Valider", nullptr));
        nbGroupeErrorLabel->setText(QString());
        TailleGroupesGroupBox->setTitle(QApplication::translate("MainWindow", "Taille des groupes", nullptr));
        GroupeErrorLabel->setText(QString());
        calculerButton->setText(QApplication::translate("MainWindow", "Calculer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
