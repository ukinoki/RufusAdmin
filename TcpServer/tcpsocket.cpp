/* (C) 2018 LAINE SERGE
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

#include "tcpsocket.h"

TcpSocket* TcpSocket::instance = Q_NULLPTR;

TcpSocket* TcpSocket::I()
{
    if (instance == Q_NULLPTR)
        instance = new TcpSocket();
    return instance;
}

TcpSocket::TcpSocket()
{
    buffer.clear();
    sizedata = 0;
}

TcpSocket::TcpSocket(qintptr ID, QObject *parent) : QTcpSocket (parent)
{
    sktdescriptor = ID;
    iduser = -1;
    datasclient = "";
    buffer.clear();
    sizedata = 0;
    a = 0;

    if(!setSocketDescriptor(ID))
    {
        emit errorskt(error());
        return;
    }

    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.
    connect(this,     &QTcpSocket::readyRead,                                                 this,   &TcpSocket::TraiteDonneesRecues, Qt::DirectConnection);
    connect(this,     &QTcpSocket::disconnected,                                              this,   &TcpSocket::Deconnexion);
    connect(this,     QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),   this,   &TcpSocket::erreurSocket);
}

TcpSocket::~TcpSocket()
{
}

void            TcpSocket::setIdUser(int id)                        {iduser = id;}
int             TcpSocket::idUser()                                 {return iduser;}
void            TcpSocket::setData(QString datas)                   {datasclient = datas;}
QString         TcpSocket::getData()                                {return datasclient;}

void TcpSocket::TraiteDonneesRecues()
{
    while (bytesAvailable() > 0)
    {
        buffer.append(readAll());
        while ((sizedata == 0 && buffer.size() >= 4) || (sizedata > 0 && buffer.size() >= sizedata)) // on n'a toujours pas la taille du message ou on n'a pas le message complet
        {
            if (sizedata == 0 && buffer.size() >= 4)                // on a les 4 premiers caractères => on a la taille du message
            {
                sizedata = Utils::ArrayToInt(buffer.mid(0, 4));
                buffer.remove(0, 4);
            }
            if (sizedata > 0 && buffer.size() >= sizedata)          // le message est complet
            {
                QByteArray data = buffer.mid(0, sizedata);
                QString msg = QString::fromUtf8(data);              // traitement du message
                //qDebug() << " Data in: " << buffer << " message = " << msg;
                buffer.clear();                                     // on remet à 0 buffer et sizedata
                sizedata = 0;
                if (msg.contains(TCPMSG_Disconnect))
                    Deconnexion();
                else
                    emit emitmsg(sktdescriptor, msg);
            }
        }
    }
}

void TcpSocket::envoyerMessage(QString msg)
{
    QByteArray paquet = msg.toUtf8();
    QString login = Datas::I()->users->getLoginById(idUser());
    QString msg2("");
    if (msg.contains(TCPMSG_ListeSockets))
        msg2 = TCPMSG_ListeSockets;
    else if (msg.contains(TCPMSG_MAJSalAttente))
        msg2 = TCPMSG_MAJSalAttente;
    else if (msg.contains(TCPMSG_MAJCorrespondants))
        msg2 = TCPMSG_MAJCorrespondants;
    else if (msg.contains(TCPMSG_MAJDocsExternes))
        msg2 = TCPMSG_MAJDocsExternes;
    Logs::MSGSOCKET("void TcpSocket::envoyerMessage(QString msg) - msg " + msg + " - " + msg2 + " - destinataire = " + login);
    //qDebug() << "message = envoyé par le serveur " + msg + " - destinataire = " + socket->peerAddress().toString();
    if(state() == QAbstractSocket::ConnectedState)
    {
        write(Utils::IntToArray(paquet.size()));
        write(paquet);
        waitForBytesWritten();
    }
}

void TcpSocket::Deconnexion()
{
    qDebug() << "deconnexion socket entrant";
    emit deconnexion(sktdescriptor);
}

void TcpSocket::erreurSocket(QAbstractSocket::SocketError erreur)
{
    qDebug() << "le cient ne répond plus - " << erreur << " - " << errorString();

    /*
        le cient ne répond plus
        QAbstractSocket::RemoteHostClosedError
        "The remote host closed the connection"
    */
}

