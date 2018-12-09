/* (C) 2018 LAINE SERGE
This file is part of RufusAdmin.

RufusAdmin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License,
or any later version.

RufusAdmin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RufusAdmin.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTILS_H
#define UTILS_H

#include <QRegExp>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include "uplineedit.h"
#include "uplabel.h"
#include "upmessagebox.h"
#include <QInputDialog>

class Utils
{
public:
    static QRegExp const rgx_rx;
    static QRegExp const rgx_AlphaNumeric;
    static QRegExp const rgx_AlphaNumeric_3_15;
    static QRegExp const rgx_AlphaNumeric_5_15;

    static QRegExp const rgx_IPV4;
    static QRegExp const rgx_IPV4_mask;

    static QRegExp const rgx_mail;

    static QRegExp const rgx_adresse;
    static QRegExp const rgx_CP;
    static QRegExp const rgx_ville;
    static QRegExp const rgx_telephone;

    static QRegExp const rgx_tabac;
    static QRegExp const rgx_cotation;

    static QRegExp const rgx_recherche;


    static void Pause(int msec = 1000);
    static QString convertHTML(QString text);

    static QString trim(QString text, bool end=true);
    static QString capitilize(QString text);
    static qint32  ArrayToInt(QByteArray source);
    static QByteArray IntToArray(int source);
    static QString getIpAdress();
    static QString getMACAdress();
    static QString CorrigeApostrophe(QString RechAp);
    static bool VerifMDP(QString MDP, QString Msg, bool mdpverified=false);
};


#endif // UTILS_H
