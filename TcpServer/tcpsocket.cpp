#include "tcpsocket.h"
#include <QTimer>

TcpSocket::TcpSocket(qintptr ID, QObject *parent) : QObject (parent)
{
    moveToThread(&thread);
    sktdescriptor = ID;
    iduser = -1;
    datasclient = "";
    buffer = new QByteArray();
    sizedata = new qint32(0);
    a = 0;
    QTimer tim;
    tim.start(10000);
    connect(&tim,   SIGNAL(timeout()),                                this, SLOT(Test()));

    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(ID))
    {
        emit error(socket->error());
        return;
    }
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(&thread, SIGNAL(finished()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()),                            this, SLOT(TraiteDonneesRecues()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),                         this, SLOT(Deconnexion()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),    this, SLOT(erreurSocket(QAbstractSocket::SocketError)));
    thread.start();
}

TcpSocket::~TcpSocket()
{
    thread.quit();
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

void TcpSocket::Test()
{
    envoyerMessage("Test");
}

void TcpSocket::TraiteDonneesRecues()
{
    qint32 *s = sizedata;
    qint32 size = *s;
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
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
                //qDebug() << socketDescriptor << " Data in: " << buffer << " message = " << messageRecu;
                emit emitmsg(sktdescriptor, messageRecu);
            }
        }
    }
}

void TcpSocket::envoyerMessage(QString msg)
{
    QByteArray paquet = msg.toUtf8();
    qDebug() << "message = envoyé par le serveur " + msg + " - destinataire = " + socket->peerAddress().toString();
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
    thread.exit(0);
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
