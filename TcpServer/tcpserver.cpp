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

    TcpThread *thread = new TcpThread(socketDescriptor, this);
    // once a thread is not needed, it will be beleted later
    connect(thread,     SIGNAL(finished()),                 thread, SLOT(deleteLater()));
    connect(thread,     SIGNAL(emitmsg(qintptr, QString)),  this,   SLOT(TraiteMessageRecu(qintptr, QString)));
    connect(thread,     SIGNAL(deconnexion(qintptr)),       this,   SLOT(Deconnexion(qintptr)));

    thread->start();
    socketthreads  .insert(socketDescriptor, thread);
}

void TcpServer::Deconnexion(qintptr sktdescriptor)
{
    qDebug() << "deconnexion";
    int id = -1;
    QString adress = "";
    QMap<qintptr, int>::iterator itid = idusers.find(sktdescriptor);
    if (itid != idusers.end())
        id = itid.value();
    QMap<qintptr, QString>::iterator itdata = dataclients.find(sktdescriptor);
    if (itdata != dataclients.end())
        adress = itdata.value().split(TCPMSG_Separator).at(2);
    QString msgconnexion = QString::number(id) + TCPMSG_Separator + adress;
    QString login = Datas::I()->users->getLoginById(id);
    dlg_message(QStringList() << login + " " +  tr("vient de se déconnecter sur") + " " + adress, 3000);

    socketthreads   .remove(sktdescriptor);
    idusers         .remove(sktdescriptor);
    dataclients     .remove(sktdescriptor);
    envoieListeSockets();
    AfficheListeSockets("Deconnexion(qintptr sktdesciptor)");
}

void TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg)
{
    if (msg.contains(TCPMSG_MsgBAL))
    {
        msg.remove(TCPMSG_MsgBAL);
        QString listdest = msg.split(TCPMSG_Separator).at(0);
        QString nbmsg = msg.split(TCPMSG_Separator).at(1);
        QStringList listid = listdest.split(",");
        for(QMap<qintptr, int>::iterator itcl = idusers.begin(); itcl != idusers.end(); ++itcl )
        {
            if (listid.contains(QString::number(itcl.value())))
                envoyerA(itcl.value(),  TCPMSG_Separator + nbmsg + TCPMSG_MsgBAL);
        }
    }
    else if (msg.contains(TCPMSG_idUser))
    {
        msg.remove(TCPMSG_idUser);
        idusers.insert(sktdescriptor,msg.toInt());
    }
    else if (msg.contains(TCPMSG_DataSocket))               // les datas  du client qui vient de se connecter reçues par le serveur
    {
        msg.remove(TCPMSG_DataSocket);
        dataclients.insert(sktdescriptor,msg);
        int id = -1;
        QMap<qintptr, int>::iterator itid = idusers.find(sktdescriptor);
        if (itid != idusers.end())
            id = itid.value();
        QString login = Datas::I()->users->getLoginById(id);
        QString adress = msg.split(TCPMSG_Separator).at(2);
        dlg_message(QStringList() << login + " " +  tr("vient de se connecter sur") + " " + adress, 3000);
        envoieListeSockets();
        AfficheListeSockets(TCPMSG_DataSocket);
    }

    else if (msg.contains(TCPMSG_FaireLAppelSocket))          // Vérifie que chaque client présent sur les listes du serveur eest toujours connecté - fait l'appel
    {
        //TODO - éliminer les doublons
        for(QMap<qintptr, QString>::iterator itcl = dataclients.begin(); itcl != dataclients.end(); ++itcl )
        {
            TcpThread* thr = getThreadFromDescriptor(itcl.key());
            if (thr != Q_NULLPTR)
                Deconnexion(itcl.key());
            else if (thr->state() == QAbstractSocket::UnconnectedState)
            {
                Deconnexion(itcl.key());
                --itcl;
            }
        }
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
    for(QMap<qintptr, QString>::iterator itcl = dataclients.begin(); itcl != dataclients.end(); ++itcl )
    {
        QMap<qintptr, int>::iterator itid = idusers.find(itcl.key());
        int id = itid.value();
        gListeSockets += itcl.value() + TCPMSG_Separator + QString::number(id) + "{}";
    }
    gListeSockets += TCPMSG_ListeSockets;
    if (sktdescriptor == 0)
        envoyerATous(gListeSockets);
    else
    {
        TcpThread* thr = getThreadFromDescriptor(sktdescriptor);
        if (thr != Q_NULLPTR)
            thr->envoyerMessage(gListeSockets);
    }
    emit ModifListeSockets();
}

QString TcpServer::ListeSockets()
{
    return gListeSockets;
}

TcpThread* TcpServer::getThreadFromDescriptor(qintptr sktdescriptor)
{
    QMap<qintptr, TcpThread*>::const_iterator itthread = socketthreads.find(sktdescriptor);
    if (itthread != socketthreads.constEnd())
        return itthread.value();
    else return Q_NULLPTR;
}

void TcpServer::envoyerATous(QString msg, qintptr emetteurorigin)
{
    for(QMap<qintptr, TcpThread*>::const_iterator itthread = socketthreads.constBegin(); itthread != socketthreads.constEnd(); ++itthread )
    {
        if (itthread.key() != emetteurorigin)
             itthread.value()->envoyerMessage(msg);
    }
}

void TcpServer::envoyerA(int iduser, QString msg)
{
    for(QMap<qintptr, int>::const_iterator itusr = idusers.begin(); itusr != idusers.end(); ++itusr )
    {
        qintptr sktdescript = itusr.key();
        if (iduser == idusers.value(sktdescript))
        {
            TcpThread* thr = getThreadFromDescriptor(sktdescript);
            if (thr != Q_NULLPTR)
                thr->envoyerMessage(msg);
        }
    }
}

void TcpServer::AfficheListeSockets(QString fonction)
{
    qDebug() << "liste des connexions " + fonction + " " + QTime::currentTime().toString("hh-mm-ss");
//    for(QMap<qintptr,QString>::iterator itcl = dataclients.begin(); itcl != dataclients.end(); ++itcl )
//    {
//        QStringList listdata = itcl.value().split(TCPMSG_Separator);
//        QString msg;
//        QStringList::const_iterator itsocket;
//        for( itsocket = listdata.constBegin(); itsocket != listdata.constEnd(); ++itsocket )
//            if (*itsocket != "")
//                msg += *itsocket + " - ";
//        qDebug() << msg;
//    }
}
