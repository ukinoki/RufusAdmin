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

#include "tcpserver.h"
#include <QTcpSocket>

TcpServer* TcpServer::instance = Q_NULLPTR;

TcpServer* TcpServer::I()
{
    if( !instance )
        instance = new TcpServer();
    return instance;
}

TcpServer::TcpServer()
{
}

void TcpServer::setId(int id)
{
    Logs::MSGSOCKET("void TcpServer::setId(int id)");
    m_idadmin = id;
    m_listeSockets = Utils::getIpAdress() + TCPMSG_Separator + Utils::getMACAdress() + TCPMSG_Separator + QHostInfo::localHostName() + TCPMSG_Separator + QString::number(m_idadmin) + "{}" TCPMSG_ListeSockets;
}

bool TcpServer::start()
{
    Logs::MSGSOCKET("void TcpServer::start()");
    QString port = NOM_PORT_TCPSERVEUR;
    if (!listen(QHostAddress::Any, port.toUShort())) // Démarrage du serveur sur toutes les IP disponibles et sur le port NOM_PORT_TCPSERVEUR
    {
        UpMessageBox::Watch(Q_NULLPTR, tr("Le serveur n'a pas pu être démarré. Raison :<br />"), errorString());        // le serveur n'a pas été démarré correctement
        return false;
    }
    // le serveur a été démarré correctement
    emit ModifListeSockets();  // déclenche la fonction void RufusAdmin::ResumeTCPSocketStatut() qui recalcule le statut des sockets
    return true;
}

void TcpServer::incomingConnection(qintptr descriptor)
{
    Logs::MSGSOCKET("void TcpServer::incomingConnection(qintptr socketDescriptor) - " + QString::number(descriptor));
    TcpSocket *socket = new TcpSocket(descriptor);

    connect(socket,     &TcpSocket::emitmsg,              this,   &TcpServer::TraiteMessageRecu);
    connect(socket,     &TcpSocket::deconnexion,          this,   &TcpServer::Deconnexion);

    map_socketdescriptors  .insert(descriptor, socket);
}

void TcpServer::Deconnexion(qintptr descriptor)
{
    Logs::MSGSOCKET("void TcpServer::Deconnexion(qintptr sktdescriptor)- skt descriptor " + QString::number(descriptor));

    TcpSocket *skt = SocketFromDescriptor(descriptor);
    if ( skt == Q_NULLPTR)
        return;
    QString adress ("");
    if (skt->datas().split(TCPMSG_Separator).size()>2)
        adress = skt->datas().split(TCPMSG_Separator).at(2);
    QString login = Datas::I()->users->getLoginById(skt->idUser());
    dlg_message(QStringList() << login + " " +  tr("vient de se déconnecter sur") + " " + adress, 3000);
    map_socketdescriptors   .remove(descriptor);
    envoieListeSockets();
    AfficheListeSockets("Deconnexion(qintptr sktdesciptor)");
    skt->deleteLater();

    Logs::MSGSOCKET("void TcpServer::Deconnexion(qintptr sktdescriptor) - skt retrouvé -> deconnexion adress " + adress + " - " + login);
}

void TcpServer::TraiteMessageRecu(qintptr descriptor, QString msg)
{
    Logs::MSGSOCKET("void TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg) - skt descriptor " + QString::number(descriptor) + " msg " + msg);
    //qDebug() << msg;
    if (SocketFromDescriptor(descriptor) == Q_NULLPTR)
        return;
    if (msg.contains(TCPMSG_MsgBAL))
    {
        /*! un message de BAL reçu par le tcpserver a la struture
                * QString contenant la liste des id de destianataires séparés par des virgules
                * TCPMSG_Separator
                * QString le nombre de messages
                * TCPMSG_Separator
                * TCPMSG_MsgBAL
         * un message est renvoyé sur chaque id de destinataire avec la structure
                * QString le nombre de messages
                * TCPMSG_MsgBAL
        */
        msg.remove(TCPMSG_MsgBAL);
        if (msg.split(TCPMSG_Separator).size()<2)
            return;
        QString listdest = msg.split(TCPMSG_Separator).at(0);
        QString nbmsg = msg.split(TCPMSG_Separator).at(1);
        QStringList listid = listdest.split(",");
        QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
        while (itskt.hasNext())
        {
            if (listid.contains(QString::number(itskt.next().value()->idUser())))
                envoyerA(itskt.value()->idUser(), nbmsg + TCPMSG_MsgBAL);
        }
        Flags::I()->MAJflagMessages();
    }
    else if (msg.contains(TCPMSG_idUser))
    {
        msg.remove(TCPMSG_idUser);
        SocketFromDescriptor(descriptor)->setIdUser(msg.toInt());
    }
    else if (msg.contains(TCPMSG_DataSocket))         // les datas  du client qui vient de se connecter reçues par le serveur
    {
        msg.remove(TCPMSG_DataSocket);
        //qDebug() << "TCPMSG_DataSocket" << msg << " - sktdescriptor" << sktdescriptor;
        SocketFromDescriptor(descriptor)->setData(msg);
        QString login = Datas::I()->users->getLoginById(SocketFromDescriptor(descriptor)->idUser());
        QString adress(tr("une adresse inconnue"));
        if (msg.split(TCPMSG_Separator).size()>2)
            adress = msg.split(TCPMSG_Separator).at(2);
        dlg_message(QStringList() << login + " " +  tr("vient de se connecter sur") + " " + adress, 3000);
        envoieListeSockets();
        AfficheListeSockets(TCPMSG_DataSocket);
    }
    else if (msg == TCPMSG_EnvoieListSocket)          // un client a demandé la liste mise à jour des sockets
        envoieListeSockets(descriptor);
    else if (msg.contains(TCPMSG_MAJDocsExternes))
        envoyerATous(msg);
    else if (msg.contains(TCPMSG_TestConnexion))
    {
        qDebug() << msg + " - " + QTime::currentTime().toString("HH:mm");
    }
    else if (msg.contains(TCPMSG_MAJSalAttente))
    {
        envoyerATous(msg, descriptor);
        Flags::I()->MAJFlagSalleDAttente();
    }
    else if (msg.contains(TCPMSG_MAJCorrespondants))
    {
        envoyerATous(msg, descriptor);
        Flags::I()->MAJflagCorrespondants();
    }
    else if (msg.contains(TCPMSG_MAJPatient))
    {
        envoyerATous(msg, descriptor);
    }
    else
        envoyerATous(msg, descriptor);
}

void TcpServer::envoieListeSockets(qintptr descriptor)
{
    Logs::MSGSOCKET("void TcpServer::envoieListeSockets(qintptr sktdescriptor)");
    ListeSockets();
    if (descriptor == -1)
        envoyerATous(m_listeSockets);
    else
    {
        if (SocketFromDescriptor(descriptor) != Q_NULLPTR)
            SocketFromDescriptor(descriptor)->envoyerMessage(m_listeSockets);
    }
    emit ModifListeSockets();
}

QString TcpServer::ListeSockets()
{
    Logs::MSGSOCKET("void TcpServer::ListeSockets()");
    m_listeSockets = Utils::getIpAdress();
    m_listeSockets += TCPMSG_Separator + Utils::getMACAdress();
    m_listeSockets += TCPMSG_Separator + QHostInfo::localHostName();
    m_listeSockets += TCPMSG_Separator + QString::number(m_idadmin) + "{}";
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    while (itskt.hasNext())
    {
        itskt.next();
        m_listeSockets += itskt.value()->datas() + TCPMSG_Separator + QString::number(itskt.value()->idUser()) + "{}";
    }
    m_listeSockets += TCPMSG_ListeSockets;
    return m_listeSockets;
}

TcpSocket* TcpServer::SocketFromDescriptor(qintptr descriptor)
{
    QString msg = "void TcpServer::SocketFromDescriptor(qintptr sktdescriptor) - sktdescriptor " + QString::number(descriptor);
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    while (itskt.hasNext())
    {
        itskt.next();
        if (itskt.key() == descriptor)
        {
            Logs::MSGSOCKET(msg + " - trouvé");
            return itskt.value();
        }
    }
    Logs::MSGSOCKET(msg + " = QNULLPTR");
    return Q_NULLPTR;
}

void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin)
{
    Logs::MSGSOCKET("void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin) - skt descriptor " + QString::number(emetteurorigin) + " msg " + msg);
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    while (itskt.hasNext())
    {
        itskt.next();
        if (itskt.key() != emetteurorigin)
        {
            Logs::MSGSOCKET("void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin) - sktdescriptor destinataire " + QString::number(itskt.key()) + " msg " + msg);
            itskt.value()->envoyerMessage(msg);
        }
    }
}

void TcpServer::envoyerA(int iduser, QString msg)
{
    Logs::MSGSOCKET("void TcpServer::envoyerA(int iduser, QString msg)");
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    while (itskt.hasNext())
    {
        itskt.next();
        if (iduser == itskt.value()->idUser())
            itskt.value()->envoyerMessage(msg);
    }
}

void TcpServer::AfficheListeSockets(QString fonction)
{
    qDebug() << "TcpServer::AfficheListeSockets(QString fonction) - " + fonction;
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    while (itskt.hasNext())
    {
        QStringList listdata = itskt.next().value()->datas().split(TCPMSG_Separator);
        QStringListIterator itlist(listdata);
        while (itlist.hasNext())
            qDebug() << itlist.next();
    }
}
