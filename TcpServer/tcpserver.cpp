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
    idAdmin = id;
    gListeSockets = Utils::getIpAdress() + TCPMSG_Separator + Utils::getMACAdress() + TCPMSG_Separator + QHostInfo::localHostName() + TCPMSG_Separator + QString::number(idAdmin) + "{}" TCPMSG_ListeSockets;
}

bool TcpServer::start()
{
    QString port = NOM_PORT_TCPSERVEUR;
    if (!listen(QHostAddress::Any, port.toUShort())) // Démarrage du serveur sur toutes les IP disponibles et sur le port NOM_PORT_TCPSERVEUR
    {
        UpMessageBox::Watch(Q_NULLPTR, tr("Le serveur n'a pas pu être démarré. Raison :<br />"), errorString());        // le serveur n'a pas été démarré correctement
        return false;
    }
    // le serveur a été démarré correctement
    emit ModifListeSockets();
    return true;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{

    TcpSocket *socket = new TcpSocket(socketDescriptor);

    connect(socket,     SIGNAL(emitmsg(qintptr, QString)),  this,   SLOT(TraiteMessageRecu(qintptr, QString)));
    connect(socket,     SIGNAL(deconnexion(qintptr)),       this,   SLOT(Deconnexion(qintptr)));

    socketdescriptors  .insert(socketDescriptor, socket);
}

void TcpServer::Deconnexion(qintptr sktdescriptor)
{
    if (SocketFromDescriptor(sktdescriptor) == Q_NULLPTR)
        return;
    QString adress = SocketFromDescriptor(sktdescriptor)->getData().split(TCPMSG_Separator).at(2);
    QString login = Datas::I()->users->getLoginById(SocketFromDescriptor(sktdescriptor)->idUser());
    dlg_message(QStringList() << login + " " +  tr("vient de se déconnecter sur") + " " + adress, 3000);
    socketdescriptors   .remove(sktdescriptor);
    envoieListeSockets();
    AfficheListeSockets("Deconnexion(qintptr sktdesciptor)");
}

void TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg)
{
    qDebug() << msg;
    if (SocketFromDescriptor(sktdescriptor) == Q_NULLPTR)
        return;
    if (msg.contains(TCPMSG_MsgBAL))
    {
        msg.remove(TCPMSG_MsgBAL);
        QString listdest = msg.split(TCPMSG_Separator).at(0);
        QString nbmsg = msg.split(TCPMSG_Separator).at(1);
        QStringList listid = listdest.split(",");
        for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        {
            if (listid.contains(QString::number(itthr.value()->idUser())))
                envoyerA(itthr.value()->idUser(),  TCPMSG_Separator + nbmsg + TCPMSG_MsgBAL);
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
        QString adress = msg.split(TCPMSG_Separator).at(2);
        dlg_message(QStringList() << login + " " +  tr("vient de se connecter sur") + " " + adress, 3000);
        envoieListeSockets();
        AfficheListeSockets(TCPMSG_DataSocket);
    }

    else if (msg == TCPMSG_EnvoieListSocket)          // un client a demandé la liste mise à jour des sockets
        envoieListeSockets(sktdescriptor);
    else
        envoyerATous(msg, sktdescriptor);
}

void TcpServer::envoieListeSockets(qintptr sktdescriptor)
{
    // le 1er item de gListeSockets est le serveur
    gListeSockets = Utils::getIpAdress() + TCPMSG_Separator + Utils::getMACAdress() + TCPMSG_Separator + QHostInfo::localHostName() + TCPMSG_Separator + QString::number(idAdmin) + "{}";
    // les suivants sont les clients
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        gListeSockets += itthr.value()->getData() + TCPMSG_Separator + QString::number(itthr.value()->idUser()) + "{}";

    gListeSockets += TCPMSG_ListeSockets;

    if (sktdescriptor == 0)
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
    return gListeSockets;
}

TcpSocket* TcpServer::SocketFromDescriptor(qintptr sktdescriptor)
{
    QMap<qintptr, TcpSocket*>::const_iterator itthread = socketdescriptors.find(sktdescriptor);
    if (itthread != socketdescriptors.constEnd())
        return itthread.value();
    else return Q_NULLPTR;
}

void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin)
{
    for(QMap<qintptr, TcpSocket*>::const_iterator itthread = socketdescriptors.constBegin(); itthread != socketdescriptors.constEnd(); ++itthread )
    {
        if (itthread.key() != emetteurorigin)
             itthread.value()->envoyerMessage(msg);
    }
}

void TcpServer::envoyerA(int iduser, QString msg)
{
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
        if (iduser == itthr.value()->idUser())
            itthr.value()->envoyerMessage(msg);
}

void TcpServer::AfficheListeSockets(QString fonction)
{
    qDebug() << "liste des connexions " + fonction + " " + QTime::currentTime().toString("hh-mm-ss");
    for(QMap<qintptr, TcpSocket*>::iterator itthr = socketdescriptors.begin(); itthr != socketdescriptors.end(); ++itthr )
    {
        QStringList listdata = itthr.value()->getData().split(TCPMSG_Separator);
        QString msg;
        QStringList::const_iterator itsocket;
        for( itsocket = listdata.constBegin(); itsocket != listdata.constEnd(); ++itsocket )
            if (*itsocket != "")
                msg += *itsocket + " - ";
        qDebug() << msg;
    }
}