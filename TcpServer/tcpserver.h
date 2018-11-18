/* (C) 2018 LAINE SERGE
This file is part of RufusAdmin.

RufusAdmin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RufusAdmin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RufusAdmin. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "upmessagebox.h"
#include "dlg_message.h"
#include "utils.h"
#include <QTime>
#include "gbl_datas.h"
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static      TcpServer* getInstance();
    bool        start();
    void        envoyerATous(QString msg, qintptr emetteurorigin = 0);                      /* envoi de message commun sur tous les sockets - emetteurorigin sera exclu de la liste des destinataires */
    QString     ListeSockets();
    void        setId(int id);
    void        envoyerA(int iduser, QString msg);                                          /* envoi d'un message sur un QTcpSocket en particulier */
    void        Deconnexion(int iduser, QString MACAdress);                                 /* deconnexion d'un socket */

private:
    static TcpServer*        instance;
    TcpServer();

    QMap<qintptr, TcpSocket*>       socketdescriptors;                                      // le mapping des sockets à partir des scoketdescriptor

    void                            AfficheListeSockets(QString fonction = "");             /* utilisé pour le debugging seulement */
    void                            envoieListeSockets(qintptr sktdescriptor = 0);          /* envoie la liste des sockets à tous les socketdescriptors */
    QString                         gListeSockets;
    int                             idAdmin;
    TcpSocket*                      SocketFromDescriptor(qintptr socketdescriptor);

signals:
    void                            ModifListeSockets();

private slots:
    void                            TraiteMessageRecu(qintptr sktdescriptor, QString msg);  /* traitement des messages reçus*/
    void                            Deconnexion(qintptr sktdescriptor, TcpSocket* skt);     /* deconnexion d'un socket */

protected:
    void                            incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
