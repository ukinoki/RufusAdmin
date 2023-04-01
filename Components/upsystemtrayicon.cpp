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

#include "upsystemtrayicon.h"

UpSystemTrayIcon* UpSystemTrayIcon::instance =  Q_NULLPTR;
UpSystemTrayIcon* UpSystemTrayIcon::I()
{
    if( !instance )
        instance = new UpSystemTrayIcon(Icons::icAppIcon());
    return instance;
}

void UpSystemTrayIcon::showMessage(QString title, QString msg, int duree, const QIcon &icon)
{
    struct Message message = {
        title,
        msg,
        duree,
        icon.isNull() ? Icons::icAppIcon() : icon
    };

    I();
    instance->list_messages << message;
    if (!instance->isVisible())
        instance->showListMessages();
}

void UpSystemTrayIcon::showMessages(QString title, QStringList listmsg, int duree, QIcon icon)
{
    if(icon.isNull())
        icon = Icons::icAppIcon();
    for (int i=0; i<listmsg.size(); ++i)
        showMessage(title, listmsg.at(i), duree, icon);
}

void UpSystemTrayIcon::showMessage(QString msg, int duree, const QIcon &icon)
{
    showMessage(tr("Messages"), msg, duree, icon);
}

void UpSystemTrayIcon::showMessages(QStringList listmsg, int duree, const QIcon &icon)
{
   showMessages(tr("Messages"), listmsg, duree, icon);
}

void UpSystemTrayIcon::showListMessages()
{
    if (list_messages.isEmpty())
    {
        hide();
        return;
    }
    if (!isVisible())
        show();
    const struct Message &message = list_messages.constFirst();
    QSystemTrayIcon::showMessage(message.title, message.text, message.icon, message.duration);
    list_messages.removeAt(0);
    QTimer::singleShot(message.duration, this, &UpSystemTrayIcon::showListMessages);
}
