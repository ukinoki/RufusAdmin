#include "gestiontcpserver.h"
#include <QTcpSocket>

GestionTcPServer* GestionTcPServer::instance = Q_NULLPTR;

GestionTcPServer* GestionTcPServer::getInstance()
{
    if( !instance )
        instance = new GestionTcPServer();
    return instance;
}

GestionTcPServer::GestionTcPServer()
{
}

void GestionTcPServer::setId(int id)
{
    m_id = id;
    gListeSockets = Utils::getIpAdress() + TCPMSG_Separator + Utils::getMACAdress() + TCPMSG_Separator + QHostInfo::localHostName() + TCPMSG_Separator + QString::number(m_id) + "{}" TCPMSG_ListeSockets;
}

bool GestionTcPServer::start()
{
    QString port = NOM_PORT_TCPSERVEUR;
    if (!listen(QHostAddress::Any, port.toUShort())) // Démarrage du serveur sur toutes les IP disponibles et sur le port NOM_PORT_TCPSERVEUR
    {
        UpMessageBox::Watch(Q_NULLPTR, tr("Le serveur n'a pas pu être démarré. Raison :<br />"), errorString());        // le serveur n'a pas été démarré correctement
        return false;
    }
    // le serveur a été démarré correctement
    connect (this, &GestionTcPServer::newConnection,    this,   &GestionTcPServer::nouvelleconnexion);
    emit ModifListeSockets();
    return true;
}

void GestionTcPServer::nouvelleconnexion()
{
    while (hasPendingConnections())
    {
        QTcpSocket *tcl     = nextPendingConnection();
        buffers             .insert(tcl, new QByteArray());
        sizes               .insert(tcl, new qint32(0));
        idusers             .insert(tcl, -1);
        dataclients         .insert(tcl, "");
        connect(tcl,        SIGNAL(readyRead()),    this, SLOT(TraiteDonneesRecues()));
        connect(tcl,        SIGNAL(disconnected()), this, SLOT(DeconnexionParLeSocket()));
    }
}

void GestionTcPServer::TraiteDonneesRecues()
{
    QTcpSocket *tcl = qobject_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(tcl);
    qint32 *s = sizes.value(tcl);
    qint32 size = *s;
    while (tcl->bytesAvailable() > 0)
    {
        buffer->append(tcl->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = Utils::ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // toutes les données ont été reçues
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                QString messageRecu = QString::fromUtf8(data);
                TraiteMessageRecu(tcl, messageRecu);
            }
        }
    }
}

void GestionTcPServer::DeconnexionParLeSocket()     // le client s'est déconnecté spontanément
{
    qDebug() << "deconnexion appelée par le client";
    QTcpSocket *tcl = qobject_cast<QTcpSocket*>(sender());
    if (tcl)
        Deconnexion(tcl);
}

void GestionTcPServer::Deconnexion(QTcpSocket *tcl)
{
    int id = -1;
    QString adress = "";
    QMap<QTcpSocket*, int>::iterator itid = idusers.find(tcl);
    if (itid != idusers.end())
        id = itid.value();
    QMap<QTcpSocket*, QString>::iterator itdata = dataclients.find(tcl);
    if (itdata != dataclients.end())
        adress = itdata.value().split(TCPMSG_Separator).at(2);
    QString msgconnexion = QString::number(id) + TCPMSG_Separator + adress;
    QString login = Datas::I()->users->getLoginById(id);
    dlg_message(QStringList() << login + " " +  tr("vient de se déconnecter sur") + " " + adress, 3000);

    buffers     .remove(tcl);
    sizes       .remove(tcl);
    idusers     .remove(tcl);
    dataclients .remove(tcl);
    tcl->deleteLater();
    envoieListeSockets();
    AfficheListeSockets("Deconnexion(QTcpSocket *tcl)");
}

void GestionTcPServer::TraiteMessageRecu(QTcpSocket *tcl,  QString msg)
{
    if (msg.contains(TCPMSG_MsgBAL))
    {
        msg.remove(TCPMSG_MsgBAL);
        QStringList listid = msg.split(",");
        for(QMap<QTcpSocket*, int>::iterator itcl = idusers.begin(); itcl != idusers.end(); ++itcl )
        {
            if (listid.contains(QString::number(itcl.value())))
                envoyerA(const_cast<QTcpSocket*>(itcl.key()),  TCPMSG_MsgBAL);
        }
    }
    else if (msg.contains(TCPMSG_idUser))
    {
        msg.remove(TCPMSG_idUser);
        idusers.insert(tcl,msg.toInt());
    }
    else if (msg.contains(TCPMSG_DataSocket))               // les datas  du client qui vient de se connecter reçues par le serveur
    {
        msg.remove(TCPMSG_DataSocket);
        dataclients.insert(tcl,msg);
        int id = -1;
        QMap<QTcpSocket*, int>::iterator itid = idusers.find(tcl);
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
        for(QMap<QTcpSocket*, QString>::iterator itcl = dataclients.begin(); itcl != dataclients.end(); ++itcl )
            if (itcl.key()->state() == QAbstractSocket::UnconnectedState)
            {
                Deconnexion(itcl.key());
                --itcl;
            }
    }
    else if (msg == TCPMSG_EnvoieListSocket)          // un client a demandé la liste mise à jour des sockets
        envoieListeSockets(tcl);
    else
        envoyerATous(msg, tcl);
}

void GestionTcPServer::envoieListeSockets(QTcpSocket *tcl)
{
    // le 1er item de gListeSockets est le serveur
    gListeSockets = Utils::getIpAdress() + TCPMSG_Separator + Utils::getMACAdress() + TCPMSG_Separator + QHostInfo::localHostName() + TCPMSG_Separator + QString::number(idAdmin) + "{}";
    // les suivants sont les clients
    for(QMap<QTcpSocket*, QString>::iterator itcl = dataclients.begin(); itcl != dataclients.end(); ++itcl )
    {
        QMap<QTcpSocket*, int>::iterator itid = idusers.find(itcl.key());
        int id = itid.value();
        gListeSockets += itcl.value() + TCPMSG_Separator + QString::number(id) + "{}";
    }
    gListeSockets += TCPMSG_ListeSockets;
    if (tcl == Q_NULLPTR)
        envoyerATous(gListeSockets);
    else
        envoyerA(tcl, gListeSockets);
    emit ModifListeSockets();
}

QString GestionTcPServer::ListeSockets()
{
    return gListeSockets;
}

void GestionTcPServer::envoyerATous(QString msg, QTcpSocket *emetteurorigin)
{
   for(QMap<QTcpSocket*, QByteArray*>::const_iterator itcl = buffers.begin(); itcl != buffers.end(); ++itcl )
    {
        QTcpSocket *clt = const_cast<QTcpSocket*>(itcl.key());
        QString data = dataclients.value(clt);
        //qDebug() << "envoyé à tous  -> client " + data.replace(TCPMSG_Separator, " - ");
        if (clt != emetteurorigin)
             envoyerA(clt, msg);
    }
}

void GestionTcPServer::envoyerA(QTcpSocket * tcl, QString msg)
{
    QByteArray paquet = msg.toUtf8();
    //qDebug() << "message = envoyé par le serveur " + msg + " - destinataire = " + tcl->peerAddress().toString();
    if(tcl->state() == QAbstractSocket::ConnectedState)
    {
        tcl->write(Utils::IntToArray(paquet.size()));
        tcl->write(paquet);
        tcl->waitForBytesWritten();
    }
}

void GestionTcPServer::envoyerA(int iduser, QString msg)
{
    for(QMap<QTcpSocket*, int>::const_iterator itusr = idusers.begin(); itusr != idusers.end(); ++itusr )
     {
         QTcpSocket *tcl = const_cast<QTcpSocket*>(itusr.key());
         if (iduser == idusers.value(tcl))
              envoyerA(tcl, msg);
     }
}

void GestionTcPServer::AfficheListeSockets(QString fonction)
{
//    qDebug() << "liste des connexions " + fonction + " " + QTime::currentTime().toString("hh-mm-ss");
//    for(QMap<QTcpSocket*, QString>::iterator itcl = dataclients.begin(); itcl != dataclients.end(); ++itcl )
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
