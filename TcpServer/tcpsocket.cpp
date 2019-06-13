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
    socket = new QTcpSocket();
}

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

    connect(&thread,    &QThread::finished,                                                     socket, &TcpSocket::deleteLater);
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.
    connect(socket,     &QTcpSocket::readyRead,                                                 this,   &TcpSocket::TraiteDonneesRecues, Qt::DirectConnection);
    connect(socket,     &QTcpSocket::disconnected,                                              this,   &TcpSocket::Deconnexion);
    connect(socket,     QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),   this,   &TcpSocket::erreurSocket);
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
    if (socket== Q_NULLPTR)
    {
        Logs::MSGSOCKET("unknown socket");
        return;
    }
    Logs::MSGSOCKET("void TcpSocket::envoyerMessage(QString msg) - msg " + msg);
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
    Logs::MSGSOCKET(msg2 + " - destinataire = " + login);
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
    emit deconnexion(sktdescriptor);
    thread.quit();
}

void TcpSocket::erreurSocket(QAbstractSocket::SocketError erreur)
{
    qDebug() << "le cient ne répond plus - " << erreur << " - " << socket->errorString();

    /*
        le cient ne répond plus
        QAbstractSocket::RemoteHostClosedError
        "The remote host closed the connection"
    */
}

bool TcpSocket::TcpConnectToServer(QString ipadrserver)
{
    if (ipadrserver == "")
        ipadrserver    = DataBase::I()->parametres()->adresseserveurtcp();
    if (ipadrserver == "")
        return false;
    QString port        = NOM_PORT_TCPSERVEUR;
    PortTCPServer       = port.toUShort();
    /*!
     * The main difference between close() and disconnectFromHost() is that the first actually closes the OS socket, while the second does not.
     * The problem is, after a socket was closed, you cannot use it to create a new connection.
     * Thus, if you want to reuse the socket, use disconnectFromHost() otherwise close()
    */
    socket->disconnect();
    if (state() == QAbstractSocket::ConnectedState || state() == QAbstractSocket::ConnectingState)
        socket->disconnectFromHost();
    connect(socket,     &QTcpSocket::hostFound, this,   [=] {
                                                            qDebug() << "Serveur trouvé";
                                                            Logs::MSGSOCKET("Serveur trouvé");
                                                          });
    socket->connectToHost(ipadrserver,PortTCPServer);     // On se connecte au serveur
    if (socket->waitForConnected(30000))
    {
        connect(socket,                 QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,   &TcpSocket::erreurSocket);
        return true;
    }
    else
    {
        socket->disconnect();
        socket->disconnectFromHost();
        instance = Q_NULLPTR;
        return false;
    }
}

