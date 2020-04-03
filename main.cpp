/* (C) 2020 LAINE SERGE
This file is part of RufusAdmin or Rufus.

RufusAdmin and Rufus are free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License,
or any later version.

RufusAdmin and Rufus are distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RufusAdmin and Rufus.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Première initisialization de la classe Icons
*/
#include "icons.h"
QMap<QString,QPixmap> Icons::m_mapPixmap = QMap<QString,QPixmap>();
QMap<QString,QIcon> Icons::m_mapIcon = QMap<QString,QIcon>();

#include "rufusadmin.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    qRegisterMetaType<qintptr>("qintptr");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    QApplication a(argc, argv);

    QPixmap pixmap("://rufusadmin.jpg");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    Utils::Pause(1500);
    splash->close();
    delete splash;
    RufusAdmin w;
    w.show();

    return a.exec();
}
