/* (C) 2020 LAINE SERGE
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
    emit ModifListeSockets(listestringidPostesConnectes());  // déclenche la fonction void RufusAdmin::ResumeTCPSocketStatut() qui recalcule le statut des sockets
    m_islaunched = true;
    return true;
}

void TcpServer::incomingConnection(qintptr descriptor)
{
    Logs::LogSktMessage("void TcpServer::incomingConnection(qintptr socketDescriptor) - " + QString::number(descriptor));
    TcpSocket *socket = new TcpSocket(descriptor);
    map_socketdescriptors  .insert(descriptor, socket);

    connect(socket,     &TcpSocket::receiveTCPmsg,        this,   &TcpServer::TraiteMessageRecu);
    connect(socket,     &TcpSocket::deconnexion,          this,   &TcpServer::Deconnexion);

}

void TcpServer::Deconnexion(qintptr descriptor)
{
    Logs::LogSktMessage("TcpServer::Deconnexion(qintptr sktdescriptor)- skt descriptor " + QString::number(descriptor));

    TcpSocket *skt = SocketFromDescriptor(descriptor);
    if (skt == Q_NULLPTR)
        return;
    if (skt->stringid() == "")
        return;
    PosteConnecte * post = Datas::I()->postesconnectes->getByStringId(skt->stringid());
    if (post == Q_NULLPTR)
        return;
    QString adress  = post->nomposte();
    User *usr = Datas::I()->users->getById(post->id());
    QString login = ( usr != Q_NULLPTR? usr->login() : tr("utilisateur inconnu"));
    UpSystemTrayIcon::I()->showMessage(tr("Messages"), login + " " +  tr("vient de se déconnecter sur") + " " + adress, Icons::icSunglasses(), 3000);

    map_socketdescriptors   .remove(descriptor);
    envoyerATous(post->stringid() + TCPMSG_DeconnexionPoste, descriptor);
    emit ModifListeSockets(listestringidPostesConnectes());
    emit deconnexionposte(post->stringid());
    skt->deleteLater();
    Logs::LogSktMessage("TcpServer::Deconnexion(qintptr sktdescriptor) - skt retrouvé -> deconnexion\n\t"
                        "adress " + adress + "\n\t"
                        "login =  " + login);
}

void TcpServer::TraiteMessageRecu(qintptr descriptor, QString msg)
{
    Logs::LogSktMessage("TcpServer::TraiteMessageRecu(qintptr sktdescriptor, QString msg)\n\t"
                        "skt descriptor = " + QString::number(descriptor) + "\n\t"
                        "msg = " + msg);
    //qDebug() << msg;
    TcpSocket *skt = SocketFromDescriptor(descriptor);
    if (skt == Q_NULLPTR)
        return;
    if (msg.contains(TCPMSG_MsgBAL))
    {
        /*! un message de BAL reçu par le tcpserver a la struture
                * QString contenant la liste des id de destinataires séparés par des virgules
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
        QMap<qintptr, TcpSocket*>::iterator itskt;
        for ( itskt = map_socketdescriptors.begin(); itskt != map_socketdescriptors.end();)
        {
            if (!OKControleIterator(itskt))
                break;
            TcpSocket *skts = const_cast<TcpSocket*>(itskt.value());
            if (skts != Q_NULLPTR)
                if (skts->stringid() != "")
                {
                    PosteConnecte * post = Datas::I()->postesconnectes->getByStringId(skts->stringid());
                    if (post != Q_NULLPTR)
                        if (listid.contains(QString::number(post->id())) && !listdestfin.contains(post->id())) {
                            listdestfin << post->id();
                            envoyerBALMsgA(post->id(), nbmsg + TCPMSG_MsgBAL);
                        }
                }
            ++ itskt;
        }
        Flags::I()->MAJflagMessages();
    }
    else if (msg.contains(TCPMSG_StringidPoste))         // le stringid du poste qui vient de se connecter
    {
        msg.remove(TCPMSG_StringidPoste);
        // qDebug() << "stringid = " << msg;
        skt->setStringid(msg);
        QString resume;
        PosteConnecte *post = Datas::I()->postesconnectes->getByStringId(msg);
        if (!post)
            resume = tr("un inconnu vient de se connecter de je ne sais où");
        else
        {
            User *usr = Datas::I()->users->getById(post->id());
            resume = (usr? usr->login() : "xxx") + " " +  tr("vient de se connecter sur") + " " + post->nomposte();
        }
        // qDebug() << resume;
        Logs::LogSktMessage("TcpServer::TraiteMessageRecu() - msg.contains(TCPMSG_StringidPoste)\n\t"
                            "data = " + msg + " -----------------------------------------> " + resume);
        UpSystemTrayIcon::I()->showMessage(tr("Messages"), resume, Icons::icSunglasses(), 3000);
        envoieListeSockets();
    }
    else if (msg == TCPMSG_EnvoieListSocket)          // un client a demandé la liste mise à jour des sockets
        envoieListeSockets(descriptor);
    else if (msg.contains(TCPMSG_MAJDocsExternes))
    {
        //qDebug() << msg;
        envoyerATous(msg);
    }
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
    else if (msg.contains(TCPMSG_AskListeStringId))
    {
        QString listidpost = listestringidPostesConnectes() + TCPMSG_AskListeStringId;
        skt->envoyerMessage(listidpost);
    }
    else
        envoyerATous(msg, descriptor);
}

void TcpServer::envoieListeSockets(qintptr descriptor)
{
    QString desc = (descriptor == -1? "tous" : QString::number(descriptor));
    Logs::LogSktMessage("TcpServer::envoieListeSockets(qintptr sktdescriptor) - destinataire = " + desc);
    QString listidpost = listestringidPostesConnectes();
    if (descriptor == -1)
        envoyerATous(listidpost);
    else
    {
        if (SocketFromDescriptor(descriptor) != Q_NULLPTR)
            SocketFromDescriptor(descriptor)->envoyerMessage(listidpost);
    }
    emit ModifListeSockets(listidpost);
}

bool TcpServer::islaunched() const
{
    return m_islaunched;
}

QString TcpServer::listestringidPostesConnectes()
{
    QString liststringidpost = "";
    PosteConnecte *post = Datas::I()->postesconnectes->admin();
    if (post != Q_NULLPTR)
        liststringidpost += post->stringid();
    if (map_socketdescriptors.count()>0)
        for (auto itskt = map_socketdescriptors.begin(); itskt != map_socketdescriptors.end();)
        {
            if (!OKControleIterator(itskt))
                break;
            TcpSocket *skts = const_cast<TcpSocket*>(itskt.value());
            if (skts != Q_NULLPTR)
                if (skts->stringid() != "")
                    liststringidpost += TCPMSG_Separator + skts->stringid();
            ++ itskt;
        }
    return liststringidpost + TCPMSG_ListeStringIdPostesConnectes;
}

TcpSocket* TcpServer::SocketFromDescriptor(qintptr descriptor)
{
    QString msg = "TcpServer::SocketFromDescriptor(qintptr sktdescriptor) - sktdescriptor = " + QString::number(descriptor);
    auto itskt = map_socketdescriptors.find(descriptor);
    if (itskt != map_socketdescriptors.end()) {
        Logs::LogSktMessage("TcpServer::SocketFromDescriptor(qintptr descriptor)\n\t" + msg + " - trouvé");
        return itskt.value();
    }
    else {
        Logs::LogSktMessage(msg + " = QNULLPTR");
        return Q_NULLPTR;
    }
}

void TcpServer::envoyerATous(QString msg, qintptr emetteur)
{
    QString aexclure = (emetteur == 0? "personne" : datadescriptor(emetteur));
    Logs::LogSktMessage("TcpServer::envoyerATous(QString msg, qintptr emetteur)\n\t"
                        "emetteur a exclure = " + aexclure + "\n\t"
                        "msg = " + msg);
    for (auto itskt = map_socketdescriptors.begin(); itskt != map_socketdescriptors.end();)
    {
        if (!OKControleIterator(itskt))
            break;
        TcpSocket *skts = const_cast<TcpSocket*>(itskt.value());
        if (skts != Q_NULLPTR)
            if (skts->socketDescriptor() != emetteur)
            {
                Logs::LogSktMessage("TcpServer::envoyerATous(QString msg, qintptr emetteur)\n\t"
                                    "destinataire = " + datadescriptor(itskt.key()) + "\n\t"
                                                                                      "msg = " + msg);
                skts->envoyerMessage(msg);
            }
        ++ itskt;
    }
}

void TcpServer::envoyerBALMsgA(int iduser, QString msg)
{
    User * usr = Datas::I()->users->getById(iduser);
    if (usr == Q_NULLPTR)
    {
        Logs::ERROR("void TcpServer::envoyerBALMsgA(int iduser, QString msg)\n\t"
                            "iduser = " + QString::number(iduser) + " - user non retrouvé" + "\n\t"
                            "msg = " + msg);
        return;
    }
    Logs::LogSktMessage("void TcpServer::envoyerBALMsgA(int iduser, QString msg)\n\t"
                        "iduser = " + QString::number(iduser) + " - " + Datas::I()->users->getById(iduser)->login() + "\n\t"
                        "msg = " + msg);
    QList<int> listdestfin = QList<int>();
    for (auto itskt = map_socketdescriptors.begin(); itskt != map_socketdescriptors.end();)
    {
        if (!OKControleIterator(itskt))
            break;
        TcpSocket *skts = const_cast<TcpSocket*>(itskt.value());
        if (skts != Q_NULLPTR)
            if (skts->stringid() != "")
            {
                PosteConnecte * post = Datas::I()->postesconnectes->getByStringId(skts->stringid());
                if (post != Q_NULLPTR)
                    if (iduser == post->id() && !listdestfin.contains(post->id())) {
                        listdestfin << post->id();
                        itskt.value()->envoyerMessage(msg);
                    }
            }
        ++ itskt;
    }
}

void TcpServer::AfficheListeSockets(QString fonction) /*! pas utilisé */
{
    qDebug() << "TcpServer::AfficheListeSockets(QString fonction) - " + fonction;
    for (auto itskt = map_socketdescriptors.begin(); itskt != map_socketdescriptors.end();)
    {
        if (!OKControleIterator(itskt))
            break;
        TcpSocket *skts = const_cast<TcpSocket*>(itskt.value());
        if (skts != Q_NULLPTR)
            if (skts->stringid() != "")
            {
                PosteConnecte * post = Datas::I()->postesconnectes->getByStringId(skts->stringid());
                if (post != Q_NULLPTR)
                    qDebug() << post->ipadress() << post->macadress() << post->nomposte() << post->id();
            }
        ++ itskt;
    }
}

QString TcpServer::datadescriptor(qintptr descriptor)
{
    TcpSocket* skt = SocketFromDescriptor(descriptor);
    if (skt == Q_NULLPTR)
        return "nulldata - " + QString::number(descriptor);
    if (skt->stringid() == "")
        return "nullstringid - " + QString::number(descriptor);
    //qDebug() << " descriptor = " << descriptor << " - stringid = " << skt->stringid();
    PosteConnecte * post = Datas::I()->postesconnectes->getByStringId(skt->stringid());
    if (post == Q_NULLPTR)
        return "nullposte - " + QString::number(descriptor);
    return QString::number(descriptor) + " - " + Datas::I()->users->getById(post->id())->login() + " / " + post->nomposte();
}

bool TcpServer::OKControleIterator(QMap<qintptr, TcpSocket *>::iterator &itr)
{
    if (itr.value() == Q_NULLPTR)
        itr = map_socketdescriptors.erase(itr);
    return (itr != map_socketdescriptors.end());
}
