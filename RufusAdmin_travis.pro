#* (C) 2016 LAINE SERGE

#    This file is part of RufusAdmin.

#    RufusAdmin is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    RufusAdmin is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with RufusAdmin.  If not, see <http://www.gnu.org/licenses/>.

QT       += sql core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = /home/travis/build/Ukinoki/RufusAdmin

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

linux-g++ {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    target.path = /home/travis/build/Ukinoki/RufusAdmin.AppDir
    desktop.path = /home/travis/build/Ukinoki/RufusAdmin.AppDir
    desktop.files += ./RufusAdmin.AppDir/rufusadmin.desktop
    shortcutfiles.path = $$PREFIX/share/applications
    icon.path = /home/travis/build/Ukinoki/RufusAdmin.AppDir
    icon.files += ./RufusAdmin.AppDir/rufusadmin.png

    INSTALLS += target
    INSTALLS += desktop
    INSTALLS += shortcutfiles
    INSTALLS += icon
}

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
include(ItemsLists/itemslists.pri)
include(Database/database.pri)
include(Components/components.pri)
include(TcpServer/tcpserver.pri)
include(Dialogs/dialogs.pri)
include(Widgets/widgets.pri)

ICON += \
    Sunglasses.icns

INCLUDEPATH += /usr/include/poppler/qt5

LIBS += -Llibs/poppler -lpoppler-qt5

QTPLUGIN += QSQLMYSQL

DISTFILES += \
    RufusAdmin.AppDir/rufusadmin.desktop \
    RufusAdmin.AppDir/rufusadmin.png \
    _Diagrams/ImpressionsRufus.vpp \
    _Diagrams/readme.txt \
    _Diagrams/Gestion des documents sous Rufus.pdf \
    _Diagrams/Recuperation_CCAM-howto.txt \
    _Diagrams/Creer_des_procedures_mysql_avec_Qt.rtf \
    assets/A_faire.odt
