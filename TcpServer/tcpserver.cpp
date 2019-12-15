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

bool TcpServer::start()
{
    Logs::LogSktMessage("void TcpServer::start()");
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
    Logs::LogSktMessage("void TcpServer::incomingConnection(qintptr socketDescriptor) - " + QString::number(descriptor));
    TcpSocket *socket = new TcpSocket(descriptor);

    connect(socket,     &TcpSocket::receiveTCPmsg,              this,   &TcpServer::TraiteMessageRecu);
    connect(socket,     &TcpSocket::deconnexion,          this,   &TcpServer::Deconnexion);

    map_socketdescriptors  .insert(descriptor, socket);
}

void TcpServer::Deconnexion(qintptr descriptor)
{
    Logs::LogSktMessage("void TcpServer::Deconnexion(qintptr sktdescriptor)- skt descriptor " + QString::number(descriptor));

    TcpSocket *skt = SocketFromDescriptor(descriptor);
    if ( skt == Q_NULLPTR)
        return;
    PosteConnecte * post = skt->posteconnecte();
    if (post == Q_NULLPTR)
        return;
    QString adress  = post->nomposte();
    User *usr = Datas::I()->users->getById(post->id());
    QString login = ( usr != Q_NULLPTR? usr->login() : "" );
    UpSystemTrayIcon::I()->showMessage(tr("Messages"), login + " " +  tr("vient de se déconnecter sur") + " " + adress, Icons::icSunglasses(), 3000);

    map_socketdescriptors   .remove(descriptor);
    envoyerATous(post->stringid() + TCPMSG_DeconnexionPoste);
    emit deconnexionposte(post->stringid());
    skt->deleteLater();
    Logs::LogSktMessage("void TcpServer::Deconnexion(qintptr sktdescriptor) - skt retrouvé -> deconnexion adress " + adress + " - " + login);
}

void TcpServer::TraiteMessageRecu(qintptr descriptor, QString msg)
{
    Logs::LogSktMessage("void TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg) - skt descriptor " + QString::number(descriptor) + " msg " + msg);
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
        QList<int> listdestfin = QList<int>();
        QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
        while (itskt.hasNext())
        {
            PosteConnecte * post = itskt.value()->posteconnecte();
            if (post == Q_NULLPTR)
                continue;
            if (listid.contains(QString::number(post->id())) && !listdestfin.contains(post->id())) {
                listdestfin << post->id();
                envoyerA(post->id(), nbmsg + TCPMSG_MsgBAL);
            }
        }
        Flags::I()->MAJflagMessages();
    }
    else if (msg.contains(TCPMSG_StringidPoste))         // le stringid du poste qui vient de se connecter
    {
        msg.remove(TCPMSG_StringidPoste);
        TcpSocket *skt = SocketFromDescriptor(descriptor);
        if (skt == Q_NULLPTR)
            return;
        PosteConnecte * post = Datas::I()->postesconnectes->getByStringId(msg);
        if (post == Q_NULLPTR)
            return;
        skt->setposteconnecte(post);
        //qDebug() << "TCPMSG_DataSocket" << msg << " - sktdescriptor" << sktdescriptor;
        Logs::LogSktMessage("void TcpServer::TraiteMessageRecu() - msg.contains(TCPMSG_StringidPoste) - data = " + msg);
        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Datas::I()->users->getById(post->id())->login() + " " +  tr("vient de se connecter sur") + " " + post->nomposte(), Icons::icSunglasses(), 3000);
        envoieListeSockets();
        AfficheListeSockets(TCPMSG_StringidPoste);
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
    Logs::LogSktMessage("void TcpServer::envoieListeSockets(qintptr sktdescriptor)");
    QString listskt = listeidPostesConnectes();
    if (descriptor == -1)
        envoyerATous(listskt);
    else
    {
        if (SocketFromDescriptor(descriptor) != Q_NULLPTR)
            SocketFromDescriptor(descriptor)->envoyerMessage(listskt);
    }
    emit ModifListeSockets();
}

QList<PosteConnecte*> TcpServer::listePostesConnectes()
{
    QList<PosteConnecte*> listpost = QList<PosteConnecte*>();
    PosteConnecte *post = Datas::I()->postesconnectes->admin();
    if (post != Q_NULLPTR)
        listpost << post;
    foreach (TcpSocket* skt, map_socketdescriptors.values())
    {
        PosteConnecte * post = skt->posteconnecte();
        if (post == Q_NULLPTR)
            continue;
        listpost << post;
    }
    return listpost;
}

QString TcpServer::listeidPostesConnectes()
{
    QString liststringidpost = "";
    PosteConnecte *post = Datas::I()->postesconnectes->admin();
    if (post != Q_NULLPTR)
        liststringidpost += post->stringid();
    foreach (TcpSocket* skt, map_socketdescriptors.values())
    {
        PosteConnecte * post = skt->posteconnecte();
        if (post == Q_NULLPTR)
            continue;
        liststringidpost += TCPMSG_Separator + post->stringid();
    }
    liststringidpost += TCPMSG_ListeSockets;
    return liststringidpost;
}

TcpSocket* TcpServer::SocketFromDescriptor(qintptr descriptor)
{
    QString msg = "void TcpServer::SocketFromDescriptor(qintptr sktdescriptor) - sktdescriptor " + QString::number(descriptor);
    auto itskt = map_socketdescriptors.find(descriptor);
    if (itskt != map_socketdescriptors.end()) {
        Logs::LogSktMessage(msg + " - trouvé");
        return itskt.value();
    }
    else {
        Logs::LogSktMessage(msg + " = QNULLPTR");
        return Q_NULLPTR;
    }
}

void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin)
{
    Logs::LogSktMessage("void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin) - skt descriptor " + QString::number(emetteurorigin) + " msg " + msg);
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    while (itskt.hasNext())
    {
        itskt.next();
        if (itskt.key() != emetteurorigin)
        {
            Logs::LogSktMessage("void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin) - sktdescriptor destinataire " + QString::number(itskt.key()) + " msg " + msg);
            itskt.value()->envoyerMessage(msg);
        }
    }
}

void TcpServer::envoyerA(int iduser, QString msg)
{
    Logs::LogSktMessage("void TcpServer::envoyerA(int iduser, QString msg)");
    QMapIterator<qintptr, TcpSocket*> itskt(map_socketdescriptors);
    QList<int> listdestfin = QList<int>();
    foreach (TcpSocket* skt, map_socketdescriptors.values())
    {
        PosteConnecte * post = skt->posteconnecte();
        if (post == Q_NULLPTR)
            continue;
        if (iduser == post->id() && !listdestfin.contains(post->id())) {
            listdestfin << post->id();
            itskt.value()->envoyerMessage(msg);
        }
    }
}

void TcpServer::AfficheListeSockets(QString fonction)
{
    qDebug() << "TcpServer::AfficheListeSockets(QString fonction) - " + fonction;
    foreach (TcpSocket* skt, map_socketdescriptors.values())
    {
        PosteConnecte * post = skt->posteconnecte();
        if (post == Q_NULLPTR)
            continue;
        qDebug() << post->ipadress() << post->macadress() << post->nomposte() << post->id();
    }
}
