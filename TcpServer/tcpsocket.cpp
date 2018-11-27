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

#include "tcpsocket.h"

TcpSocket::TcpSocket(qintptr ID, QObject *parent) : QObject (parent)
{
    moveToThread(&thread);
    sktdescriptor = ID;
    iduser = -1;
    datasclient = "";
    buffer.clear();
    sizedata = 0;
    a = 0;

    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(ID))
    {
        emit error(socket->error());
        return;
    }

    connect(&thread,    SIGNAL(finished()),                             socket, SLOT(deleteLater()));
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.
    connect(socket,     SIGNAL(readyRead()),                            this,   SLOT(TraiteDonneesRecues()), Qt::DirectConnection);
    connect(socket,     SIGNAL(disconnected()),                         this,   SLOT(Deconnexion()));
    connect(socket,     SIGNAL(error(QAbstractSocket::SocketError)),    this,   SLOT(erreurSocket(QAbstractSocket::SocketError)));
    thread.start();
}

TcpSocket::~TcpSocket()
{
    thread.wait();
}

void            TcpSocket::setIdUser(int id)                        {iduser = id;}
int             TcpSocket::idUser()                                 {return iduser;}
void            TcpSocket::setData(QString datas)                   {datasclient = datas;}
QString         TcpSocket::getData()                                {return datasclient;}

QAbstractSocket::SocketState TcpSocket::state()
{
    return socket->state();
}

void TcpSocket::TraiteDonneesRecues()
{
    while (socket->bytesAvailable() > 0)
    {
        buffer.append(socket->readAll());
        while ((sizedata == 0 && buffer.size() >= 4) || (sizedata > 0 && buffer.size() >= sizedata)) // on n'a toujours pas la teille du message ou on n'a pas le message complet
        {
            if (sizedata == 0 && buffer.size() >= 4)                // on a les 4 premiers caractères => on a la taille du message
            {
                sizedata = Utils::ArrayToInt(buffer.mid(0, 4));
                buffer.remove(0, 4);
            }
            if (sizedata > 0 && buffer.size() >= sizedata)          // le message est complet
            {
                QByteArray data = buffer.mid(0, sizedata);
                buffer.clear();                                     // on remet à 0 buffer et sizedata
                sizedata = 0;
                QString msg = QString::fromUtf8(data);              // traitement du message
                //qDebug() << socketDescriptor << " Data in: " << buffer << " message = " << msg;
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
    //qDebug() << "message = envoyé par le serveur " + msg + " - destinataire = " + socket->peerAddress().toString();
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(Utils::IntToArray(paquet.size()));
        socket->write(paquet);
        socket->waitForBytesWritten();
    }
}

void TcpSocket::Deconnexion()
{
    emit deconnexion(sktdescriptor, this);
    thread.quit();
}

void TcpSocket::erreurSocket(QAbstractSocket::SocketError erreur)
{
    //qDebug() << "le cient ne répond plus - " << erreur << " - " << socket->errorString();

    /*
        le cient ne répond plus
        QAbstractSocket::RemoteHostClosedError
        "The remote host closed the connection"
    */
}
