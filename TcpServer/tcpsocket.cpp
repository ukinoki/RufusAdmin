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
    m_bufferarray.clear();
    m_datasize = 0;
}

TcpSocket::TcpSocket(qintptr ID, QObject *parent) : QTcpSocket (parent)
{
    sktdescriptor = ID;
    m_bufferarray.clear();
    m_datasize = 0;

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

QString TcpSocket::stringid() const                     { return m_stringid;}
void TcpSocket::setStringid(const QString &stringid)    { m_stringid = stringid; }

void TcpSocket::TraiteDonneesRecues()
{
    while (bytesAvailable() > 0)
    {
        m_bufferarray.append(readAll());
        while ((m_datasize == 0 && m_bufferarray.size() >= 4) || (m_datasize > 0 && m_bufferarray.size() >= m_datasize)) // on n'a toujours pas la taille du message ou on n'a pas le message complet
        {
            if (m_datasize == 0 && m_bufferarray.size() >= 4)                // on a les 4 premiers caractères => on a la taille du message
            {
                m_datasize = Utils::ArrayToInt(m_bufferarray.mid(0, 4));
                m_bufferarray.remove(0, 4);
            }
            if (m_datasize > 0 && m_bufferarray.size() >= m_datasize)          // le message est complet
            {
                QByteArray data = m_bufferarray.mid(0, m_datasize);
                QString msg = QString::fromUtf8(data);              // traitement du message
                //qDebug() << " Data in: " << buffer << " message = " << msg;
                m_bufferarray.clear();                                     // on remet à 0 buffer et sizedata
                m_datasize = 0;
                emit receiveTCPmsg(sktdescriptor, msg);
            }
        }
    }
}

void TcpSocket::envoyerMessage(QString msg)
{
    QByteArray paquet = msg.toUtf8();
    PosteConnecte* post = Datas::I()->postesconnectes->getByStringId(stringid());
    if (post == Q_NULLPTR)
        return;
    User *usr = Datas::I()->users->getById(post->id());
    QString login = ( usr != Q_NULLPTR? usr->login() : "" );
    Logs::LogSktMessage("TcpSocket::envoyerMessage(QString msg)\n\t"
                        "msg = " + msg + "\n\t"
                        "émetteur = " + login);
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
    //qDebug() << "deconnexion socket entrant";
    emit deconnexion(sktdescriptor);
}

void TcpSocket::erreurSocket(QAbstractSocket::SocketError erreur)
{
    //qDebug() << "le cient ne répond plus - " << erreur << " - " << errorString();
    QString err =  QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey(erreur);
    Logs::LogSktMessage("TcpSocket::erreurSocket(QAbstractSocket::SocketError erreur) - " + err);
    /*
        le cient ne répond plus
        QAbstractSocket::RemoteHostClosedError
        "The remote host closed the connection"
    */
}
