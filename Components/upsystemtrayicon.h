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

#ifndef UPSYSTEMTRAYICON_H
#define UPSYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QTimer>
#include <QTime>
#include <QStandardItem>
#include "icons.h"

class UpSystemTrayIcon : public QSystemTrayIcon
{
    struct Message
    {
        QString title;
        QString text;
        int     duration;
        QIcon   icon;
    };
    Q_OBJECT
public:
    static      UpSystemTrayIcon* I();
    static void showMessage(QString title, QString msg, int duree, const QIcon &icon = QIcon());
    static void showMessages(QString title, QStringList msg, int duree, QIcon icon = QIcon());
    static void showMessage(QString msg, int duree, const QIcon &icon = QIcon());
    static void showMessages(QStringList msg, int duree, const QIcon &icon = QIcon());

private:
    static UpSystemTrayIcon *instance;
    UpSystemTrayIcon(const QIcon icon, QObject *parent = Q_NULLPTR) : QSystemTrayIcon(icon, parent) {}
    QList<struct Message>           list_messages;
    void                            showListMessages();
};

#endif // UPSYSTEMTRAYICON_H
