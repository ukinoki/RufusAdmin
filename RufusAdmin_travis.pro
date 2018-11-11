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
    upcheckbox.cpp \
    upcombobox.cpp \
    updialog.cpp \
    uplabel.cpp \
    uppushbutton.cpp \
    upsmallbutton.cpp \
    uplineedit.cpp \
    uptablewidget.cpp \
    dlg_paramconnexion.cpp \
    upmessagebox.cpp \
    widgetbuttonframe.cpp \
    importdocsexternesthread.cpp \
    functormajpremierelettre.cpp \
    functormessage.cpp \
    uptextedit.cpp \
    functordatauser.cpp \
    upradiobutton.cpp \
    dlg_message.cpp \
    dlg_banque.cpp \
    dlg_gestioncomptes.cpp \
    dlg_gestionlieux.cpp \
    dlg_gestionusers.cpp \
    dlg_motifs.cpp \
    upgroupbox.cpp \
    gestiontcpserver.cpp \
    utils.cpp \
    database.cpp \
    gbl_datas.cpp \
    upheaderview.cpp

HEADERS += \
        rufusadmin.h \
    upcheckbox.h \
    upcombobox.h \
    macros.h \
    updialog.h \
    uplabel.h \
    uppushbutton.h \
    upsmallbutton.h \
    uplineedit.h \
    uptablewidget.h \
    dlg_paramconnexion.h \
    upmessagebox.h \
    widgetbuttonframe.h \
    importdocsexternesthread.h \
    functormajpremierelettre.h \
    functormessage.h \
    uptextedit.h \
    functordatauser.h \
    dlg_banque.h \
    upradiobutton.h \
    dlg_message.h \
    dlg_gestioncomptes.h \
    dlg_gestionlieux.h \
    dlg_gestionusers.h \
    dlg_motifs.h \
    upgroupbox.h \
    gestiontcpserver.h \
    utils.h \
    database.h \
    gbl_datas.h \
    log.h \
    icons.h \
    upheaderview.h

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

ICON += \
    Sunglasses.icns

INCLUDEPATH += /usr/include/poppler/qt5

LIBS += -Llibs/poppler -lpoppler-qt5

QTPLUGIN += QSQLMYSQL

DISTFILES += \
    RufusAdmin.AppDir/rufusadmin.desktop \
    RufusAdmin.AppDir/rufusadmin.png
