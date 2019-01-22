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

#include "tcpserver.h"
#include <QTcpSocket>

TcpServer* TcpServer::instance = Q_NULLPTR;

TcpServer* TcpServer::getInstance()
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
    idAdmin = id;
    gListeSockets = Utils::getIpAdress() + TCPMSG_Separator + Utils::getMACAdress() + TCPMSG_Separator + QHostInfo::localHostName() + TCPMSG_Separator + QString::number(idAdmin) + "{}" TCPMSG_ListeSockets;
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

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
//    Logs::MSGSOCKET("void TcpServer::incomingConnection(qintptr socketDescriptor)");
// ++++ l'utilisation de l'instruction Logs::MSGSOCKET dans cette fonction perturvbe le fonctionnement du programme
    TcpSocket *socket = new TcpSocket(socketDescriptor);

    connect(socket,     SIGNAL(emitmsg(qintptr, QString)),              this,   SLOT(TraiteMessageRecu(qintptr, QString)));
    connect(socket,     SIGNAL(deconnexion(qintptr)),                   this,   SLOT(Deconnexion(qintptr)));

    socketdescriptors  .insert(socketDescriptor, socket);
}

void TcpServer::Deconnexion(int iduser, QString MACAdress)
{
    Logs::MSGSOCKET("void TcpServer::Deconnexion(int iduser, QString MACAdress)");
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        if (itthr.value()->idUser() == iduser)
        {
            if (itthr.value()->getData().split(TCPMSG_Separator).size()>0)
                if (itthr.value()->getData().split(TCPMSG_Separator).at(1) == MACAdress)
                {
                    itthr.value()->envoyerMessage(TCPMSG_Disconnect); // la valse des signaux permet de commander l'arrêt du thread depuis l'intérieur - pas trouvé mieux
                    break;
                }
        }
}

void TcpServer::Deconnexion(qintptr sktdescriptor)
{
    Logs::MSGSOCKET("void TcpServer::Deconnexion(qintptr sktdescriptor)- skt descriptor " + QString::number(sktdescriptor));

    TcpSocket *skt = SocketFromDescriptor(sktdescriptor);
    if ( skt == Q_NULLPTR)
        return;
    Logs::MSGSOCKET("void TcpServer::Deconnexion(qintptr sktdescriptor) - skt retrouvé -> deconnexion skt->getData() " + skt->getData());
    QString adress ("");
    if (skt->getData().split(TCPMSG_Separator).size()>2)
        adress = skt->getData().split(TCPMSG_Separator).at(2);
    Logs::MSGSOCKET("void TcpServer::Deconnexion(qintptr sktdescriptor) - skt retrouvé -> deconnexion adress " + adress);
    QString login = Datas::I()->users->getLoginById(skt->idUser());
    Logs::MSGSOCKET("void TcpServer::Deconnexion(qintptr sktdescriptor) - skt retrouvé -> deconnexion login " + login);
    dlg_message(QStringList() << login + " " +  tr("vient de se déconnecter sur") + " " + adress, 3000);
    socketdescriptors   .remove(sktdescriptor);
    skt->deleteLater();
    envoieListeSockets();
    AfficheListeSockets("Deconnexion(qintptr sktdesciptor)");
}

void TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg)
{
    Logs::MSGSOCKET("void TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg) - skt descriptor " + QString::number(sktdescriptor) + " msg " + msg);
    //qDebug() << msg;
    if (SocketFromDescriptor(sktdescriptor) == Q_NULLPTR)
        return;
    if (msg.contains(TCPMSG_MsgBAL))
    {
        /* un message de BAL reçu par le tcpserver a la struture
                * QString contenant la liste des id de destianataires séparés par de virgules
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
        for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        {
            if (listid.contains(QString::number(itthr.value()->idUser())))
                envoyerA(itthr.value()->idUser(), nbmsg + TCPMSG_MsgBAL);
        }
    }
    else if (msg.contains(TCPMSG_idUser))
    {
        msg.remove(TCPMSG_idUser);
        SocketFromDescriptor(sktdescriptor)->setIdUser(msg.toInt());
    }
    else if (msg.contains(TCPMSG_DataSocket))               // les datas  du client qui vient de se connecter reçues par le serveur
    {
        msg.remove(TCPMSG_DataSocket);
        SocketFromDescriptor(sktdescriptor)->setData(msg);
        QString login = Datas::I()->users->getLoginById(SocketFromDescriptor(sktdescriptor)->idUser());
        QString adress(tr("une adresse inconnue"));
        if (msg.split(TCPMSG_Separator).size()>2)
            adress = msg.split(TCPMSG_Separator).at(2);
        dlg_message(QStringList() << login + " " +  tr("vient de se connecter sur") + " " + adress, 3000);
        envoieListeSockets();
        AfficheListeSockets(TCPMSG_DataSocket);
    }

    else if (msg == TCPMSG_EnvoieListSocket)          // un client a demandé la liste mise à jour des sockets
        envoieListeSockets(sktdescriptor);
    else if (msg.contains(TCPMSG_MAJDocsExternes))
        envoyerATous(msg);
    else
        envoyerATous(msg, sktdescriptor);
}

void TcpServer::envoieListeSockets(qintptr sktdescriptor)
{
    Logs::MSGSOCKET("void TcpServer::envoieListeSockets(qintptr sktdescriptor)");
    ListeSockets();
    if (sktdescriptor == -1)
        envoyerATous(gListeSockets);
    else
    {
        if (SocketFromDescriptor(sktdescriptor) != Q_NULLPTR)
            SocketFromDescriptor(sktdescriptor)->envoyerMessage(gListeSockets);
    }
    emit ModifListeSockets();
}

QString TcpServer::ListeSockets()
{
    Logs::MSGSOCKET("void TcpServer::ListeSockets()");
    // le 1er item de gListeSockets est le serveur
    gListeSockets = Utils::getIpAdress();
    gListeSockets += TCPMSG_Separator + Utils::getMACAdress();
    gListeSockets += TCPMSG_Separator + QHostInfo::localHostName();
    gListeSockets += TCPMSG_Separator + QString::number(idAdmin) + "{}";
    // les suivants sont les clients
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        gListeSockets += itthr.value()->getData() + TCPMSG_Separator + QString::number(itthr.value()->idUser()) + "{}";

    gListeSockets += TCPMSG_ListeSockets;
    return gListeSockets;
}

TcpSocket* TcpServer::SocketFromDescriptor(qintptr sktdescriptor)
{
    Logs::MSGSOCKET("void TcpServer::SocketFromDescriptor(qintptr sktdescriptor) - sktdescriptor " + QString::number(sktdescriptor));
    QMap<qintptr, TcpSocket*>::const_iterator itthread = socketdescriptors.find(sktdescriptor);
    if (itthread == socketdescriptors.constEnd())
    {
        Logs::MSGSOCKET("void TcpServer::SocketFromDescriptor(qintptr sktdescriptor) - sktdescriptor QNULLPTR");
        return Q_NULLPTR;
    }
    else
    {
        Logs::MSGSOCKET("void TcpServer::SocketFromDescriptor(qintptr sktdescriptor) - iterator " + QString::number(itthread.key()));
        return itthread.value();
    }
}

void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin)
{
    Logs::MSGSOCKET("void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin) - skt descriptor " + QString::number(emetteurorigin) + " msg " + msg);
    for(QMap<qintptr, TcpSocket*>::const_iterator itthread = socketdescriptors.constBegin(); itthread != socketdescriptors.constEnd(); ++itthread )
    {
        if (itthread.key() != emetteurorigin)
        {
            Logs::MSGSOCKET("void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin) - sktdescriptor destinataire " + QString::number(itthread.key()) + " msg " + msg);
            itthread.value()->envoyerMessage(msg);
        }
    }
}

void TcpServer::envoyerA(int iduser, QString msg)
{
    Logs::MSGSOCKET("void TcpServer::envoyerA(int iduser, QString msg)");
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        if (iduser == itthr.value()->idUser())
            itthr.value()->envoyerMessage(msg);
}

void TcpServer::AfficheListeSockets(QString fonction)
{
    Logs::MSGSOCKET("void TcpServer::AfficheListeSockets(QString fonction)");
    //qDebug() << "liste des connexions " + fonction + " " + QTime::currentTime().toString("hh-mm-ss");
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
    {
        QStringList listdata = itthr.value()->getData().split(TCPMSG_Separator);
        QString msg;
        QStringList::const_iterator itsocket;
        for( itsocket = listdata.constBegin(); itsocket != listdata.constEnd(); ++itsocket )
            if (*itsocket != "")
                msg += *itsocket + " - ";
        //qDebug() << msg;
    }
}
