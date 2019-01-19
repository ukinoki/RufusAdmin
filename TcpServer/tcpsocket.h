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

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "utils.h"
#include "log.h"

/*les règles non écrites
 * pas de classe héritée de thread
 * pas de slot appelé dans un thread depuis l'extérieur autrement que par un signal

 */

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit        TcpSocket(qintptr ID, QObject *parent = Q_NULLPTR);
    ~TcpSocket();
    QThread         thread;
    qintptr         sktdescriptor;
    void            envoyerMessage(QString msg);
    QAbstractSocket::SocketState state();
    void            setIdUser(int id);
    int             idUser();
    void            setData(QString datas);
    QString         getData();

signals:
    void            error(QTcpSocket::SocketError socketerror);
    void            emitmsg(qintptr sktdescriptor, QString msg);
    void            deconnexion(qintptr sktdescriptor, TcpSocket * skt);

public slots:
    void            TraiteDonneesRecues();
    void            Deconnexion();

private:
    int a;
    QTcpSocket      *socket;
    QByteArray      buffer;                                                 // le buffer stocke les data jusqu'à ce que tout le bloc soit reçu
    qint32          sizedata;                                               // le stockage de la taille permet de savoir si le bloc a été reçu
    int             iduser;                                                 // stocke l'id correspondant au user correspondant à la connexion - utilisé pour la messagerie
    QString         datasclient;                                            // stocke l'adresse IP et l'adresse MAC du client

private slots:
    void            erreurSocket(QAbstractSocket::SocketError);
};

#endif // TCPSOCKET_H
