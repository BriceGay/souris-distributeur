#-------------------------------------------------
#
#    Copyright 2019 Brice Gay (brice.gay1@gmail.com)
#
#    This file is part of souris-distributeur.
#
#    souris-distributeur is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#
#    souris-distributeur is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with souris-distributeur.  If not, see <https://www.gnu.org/licenses/>.
#
#
#
#    Project created by QtCreator 2019-04-03T16:31:47
#
#-------------------------------------------------

QT += core gui core widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = souris-distrubuteur
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui \
        tutoriel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    logoVert.png \
    logoVert.png \
    logo100.png

RESOURCES += \
    logo.qrc
