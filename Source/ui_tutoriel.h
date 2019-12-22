/********************************************************************************
** Form generated from reading UI file 'tutoriel.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TUTORIEL_H
#define UI_TUTORIEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QLabel *label;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(528, 426);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(Dialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 494, 1017));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setTextFormat(Qt::AutoText);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog", "<html><head/><body><p><span style=\" font-size:20pt; font-weight:600;\">Tutoriel :</span></p><p>Souris-Distributeur est un logiciel libre qui permet de separer un groupe de donn\303\251es en plusieurs sous groupes ayant les m\303\252mes caract\303\251ristiques (moyenne, \303\251cart-type et m\303\251dianne).</p><p>Ce logiciel fonctionne uniquement avec les nombres positifs.</p><p><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">1- R\303\251cup\303\251rer vos donn\303\251es</span></p><p>Dans un <span style=\" font-weight:600;\">tableur</span> mettez vos donn\303\251es dans <span style=\" font-weight:600;\">2 colonnes</span> (le nom de la souris \303\240 droite et la taille de la tumeur \303\240 droite)</p><p><span style=\" font-weight:600;\">Selectionner</span> ces donn\303\251es (sans les titres de colonne, s\303\251lectionnez bien <span style=\" font-weight:600;\">seulement 2 colonnes</span>) </p><p><img src=\":/screenTableur.png\"/></p><p><span style=\" font-weight:600;\">Copiez</span> ce"
                        "s donn\303\251es (Ctrl + C, Cmd + C, Clique droit &gt; Copier)</p><p>Dans Souris-Distributeur<span style=\" font-weight:600;\"> cliquez</span> ensuite sur &quot;Charger les donn\303\251es&quot;</p><p>Le logiciel affiche vos donn\303\251es, <span style=\" font-weight:600;\">v\303\251rifiez</span> que tout va bien puis passez \303\240 la suite.</p><p>Au centre un graphique vous montre la r\303\251partission des donn\303\251es</p><p><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">2- Parametrer vos groupes :</span></p><p>Dans la colonne de droite, <span style=\" font-weight:600;\">indiquer</span> le nombre de groupes puis <span style=\" font-weight:600;\">cliquer</span> sur valider</p><p>Vous pouvez ensuite <span style=\" font-weight:600;\">d\303\251finir la taille</span> de chaque groupe.</p><p>Lorsque tout est pr\303\252t, <span style=\" font-weight:600;\">cliquez</span> sur calculer.</p><p><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">3- R\303\251sultats :</span></p><"
                        "p>Le logiciel copie les r\303\251sultats dans le presse-papier, il vous suffit de les coller dans un tableur<br/></p><p><br/></p><p><span style=\" font-size:20pt; font-weight:600;\">Informations sur le logiciel :</span></p><p>Ce logiciel a \303\251t\303\251 d\303\251vellop\303\251 par Brice Gay (brice.gay1@gmail.com) en 2019 pour l'IAB (INSERM). </p><p><span style=\" font-weight:600;\">Code source :</span><a href=\"https://github.com/BriceGay/souris-distributeur\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/BriceGay/souris-distributeur</span></a></p><p><span style=\" font-weight:600;\">Licence :</span></p><p>Copyright Brice Gay (brice.gay1@gmail.com) 2019 </p><p><span style=\" font-family:'monospace';\">This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the ho"
                        "pe that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see &lt;https://www.gnu.org/licenses/&gt;.</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TUTORIEL_H
