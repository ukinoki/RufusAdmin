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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "upmessagebox.h"
#include "dlg_message.h"
#include "utils.h"
#include <QTime>
#include "gbl_datas.h"
#include "flags.h"
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static TcpServer*       I();
    bool                    start();
    QString                 listestringidPostesConnectes();                                     /*! la liste des stringid des postes connectes, sous forme de QString, les string id sont séparés par TCPMSG_Separator et le QString se termine par TCPMSG_ListeSockets */
    void                    envoyerBALMsgA(int iduser, QString msg);                            /*! envoi d'un message sur un QTcpSocket en particulier */
    void                    envoyerATous(QString msg, qintptr emetteurorigin = 0);              /*! envoi de message commun sur tous les sockets - emetteurorigin sera exclu de la liste des destinataires */
    void                    envoieListeSockets(qintptr descriptor = -1);                        /*! envoie la liste des sockets sur tous les sockets */

private:
    static TcpServer*               instance;
    TcpServer();
    QMap<qintptr, TcpSocket*>       map_socketdescriptors = QMap<qintptr, TcpSocket*>();    // le map des sockets à partir des socketdescriptor

    void                            AfficheListeSockets(QString fonction = "");             /* utilisé pour le debugging seulement */
    bool                            OKControleIterator(QMap<qintptr, TcpSocket*>::iterator &itr);
    TcpSocket*                      SocketFromDescriptor(qintptr descriptor);
    void                            TraiteMessageRecu(qintptr descriptor, QString msg);     /* traitement des messages reçus*/
    void                            Deconnexion(qintptr descriptor);                        /* deconnexion d'un socket */
    QString                         datadescriptor(qintptr descriptor);                     /* renvoie les datas (poste, user) correspondant au descriptor d'un socket */

signals:
    void                            ModifListeSockets(QString listidpostesconnectes);
    void                            deconnexionposte(QString poststringid);

protected:
    void                            incomingConnection(qintptr descriptor);
};

#endif // TCPSERVER_H
