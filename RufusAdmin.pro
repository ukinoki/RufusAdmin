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

QT       += sql core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

OSX {
TARGET = /Applications/RufusAdmin
}
LINUX {
TARGET = /home/serge/RufusAdmin
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
    dlg_paramconnexion.cpp \
    widgetbuttonframe.cpp \
    importdocsexternesthread.cpp \
    functormajpremierelettre.cpp \
    functormessage.cpp \
    functordatauser.cpp \
    dlg_message.cpp \
    dlg_banque.cpp \
    dlg_gestioncomptes.cpp \
    dlg_gestionlieux.cpp \
    dlg_gestionusers.cpp \
    dlg_motifs.cpp \
    utils.cpp \
    database.cpp \
    gbl_datas.cpp

HEADERS += \
        rufusadmin.h \
    macros.h \
    dlg_paramconnexion.h \
    widgetbuttonframe.h \
    importdocsexternesthread.h \
    functormajpremierelettre.h \
    functormessage.h \
    functordatauser.h \
    dlg_banque.h \
    dlg_message.h \
    dlg_gestioncomptes.h \
    dlg_gestionlieux.h \
    dlg_gestionusers.h \
    dlg_motifs.h \
    utils.h \
    database.h \
    gbl_datas.h \
    log.h \
    icons.h \
    tcpserver.h

FORMS += \
        rufusadmin.ui \
    dlg_paramconnexion.ui \
    dlg_banque.ui \
    dlg_gestioncomptes.ui \
    dlg_gestionusers.ui \
    dlg_motifs.ui

RESOURCES += \
    assets/Images/images.qrc \
    assets/Sons/sons.qrc \
    assets/Fichiers/Fichiers.qrc \

#--------------------
# INCLUDE
#--------------------
include(Items/items.pri)
include(Components/components.pri)
include(TcpServer/tcpserver.pri)

ICON = Sunglasses.icns

OSX {
INCLUDEPATH += /usr/local/include/poppler/qt5
LIBS += -L/usr/local/lib/ -lpoppler-qt5
# la ligne qui suit impose un fichier info.plist préconfiguré dans le package
# en inserant les 2 lignes qui suivent dans ce fichier, l'application est réduite à une icone dans la barre des taches
#   elle ne figure pas dans le dock et le menu est supprimé
#	<key>LSUIElement</key>
#	<string>1</string>
QMAKE_INFO_PLIST = info.plist
}
LINUX {
INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/local/lib/ -lpoppler-qt5
}

DISTFILES += \
    COPYING


