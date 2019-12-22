/********************************************************************************
** Form generated from reading UI file 'resultdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTDIALOG_H
#define UI_RESULTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ResultDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *copieDataButton;

    void setupUi(QDialog *ResultDialog)
    {
        if (ResultDialog->objectName().isEmpty())
            ResultDialog->setObjectName(QString::fromUtf8("ResultDialog"));
        ResultDialog->resize(500, 500);
        ResultDialog->setMinimumSize(QSize(500, 500));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/logoVert.png"), QSize(), QIcon::Normal, QIcon::Off);
        ResultDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ResultDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);

        copieDataButton = new QPushButton(ResultDialog);
        copieDataButton->setObjectName(QString::fromUtf8("copieDataButton"));

        verticalLayout->addWidget(copieDataButton);


        retranslateUi(ResultDialog);

        QMetaObject::connectSlotsByName(ResultDialog);
    } // setupUi

    void retranslateUi(QDialog *ResultDialog)
    {
        ResultDialog->setWindowTitle(QApplication::translate("ResultDialog", "R\303\251sultats", nullptr));
        copieDataButton->setText(QApplication::translate("ResultDialog", "Copier les donn\303\251es", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResultDialog: public Ui_ResultDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTDIALOG_H
