#* (C) 2018 LAINE SERGE

#    This file is part of RufusAdmin.

#    RufusAdmin is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.

#    RufusAdmin is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with RufusAdmin.  If not, see <http://www.gnu.org/licenses/>.

QT       += sql core gui network multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

OSX {
TARGET = /Applications/RufusAdmin
}
LINUX {
TARGET = $(HOME)/RufusAdminApp
}

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

SOURCES += \
    main.cpp \
    rufusadmin.cpp \
    importdocsexternesthread.cpp \
    utils.cpp \
    gbl_datas.cpp \
    flags.cpp

HEADERS += \
    ostask.h \
    rufusadmin.h \
    macros.h \
    importdocsexternesthread.h \
    timerthread.h \
    utils.h \
    gbl_datas.h \
    log.h \
    icons.h \
    flags.h

FORMS += \
    rufusadmin.ui \

RESOURCES += \
    assets/Images/images.qrc \
    assets/Sons/sons.qrc \
    assets/Fichiers/Fichiers.qrc \

#--------------------
# INCLUDE
#--------------------
include(Items/items.pri)
include(Database/database.pri)
include(ItemsLists/itemslists.pri)
include(Components/components.pri)
include(TcpServer/tcpserver.pri)
include(Dialogs/dialogs.pri)
include(Widgets/widgets.pri)
LINUX {
include(SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication # cette instruction doit être incluse APRES la ligne précédente
}

TRANSLATIONS    = rufusadmin_en.ts

ICON = Sunglasses.icns

OSX {
INCLUDEPATH += /usr/local/include/poppler/qt5
LIBS += -L/usr/local/lib/ -lpoppler-qt5
# la ligne qui suit impose un fichier info.plist préconfiguré dans le package
# en inserant les 2 lignes qui suivent dans ce fichier, l'application est réduite à une icone dans la barre des taches
#   elle ne figure pas dans le dock et le menu est supprimé
#	<key>LSUIElement</key>
#	<string>1</string>
#QMAKE_INFO_PLIST = info.plist
}
LINUX {
INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/local/lib/ -lpoppler-qt5
}

DISTFILES += \
    _Diagrams/ImpressionsRufus.vpp \
    _Diagrams/readme.txt \
    _Diagrams/Gestion des documents sous Rufus.pdf \
    _Diagrams/Recuperation_CCAM-howto.txt \
    _Diagrams/Creer_des_procedures_mysql_avec_Qt.rtf \
    assets/A_faire.odt \
    COPYING


