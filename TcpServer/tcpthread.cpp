#include "tcpthread.h"
#include <QTimer>

TcpThread::TcpThread(qintptr ID, QObject *parent) : QThread(parent)
{
    this->socketDescriptor = ID;
}

void TcpThread::run()
{
    socket = new QTcpSocket();
    buffer = new QByteArray();
    sizedata = new qint32(0);

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(readyRead()),                            this, SLOT(TraiteDonneesRecues()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),                         this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),    this, SLOT(erreurSocket(QAbstractSocket::SocketError)));
    QTimer tim;
    tim.start(10000);
    connect(&tim,   SIGNAL(timeout),                                this, SLOT(Test()));
    exec();
}

QAbstractSocket::SocketState TcpThread::state()
{
    return socket->state();
}

void TcpThread::Test()
{
    envoyerMessage("Test");
}


void TcpThread::TraiteDonneesRecues()
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
                emit emitmsg(socketDescriptor, messageRecu);
            }
        }
    }
}

void TcpThread::envoyerMessage(QString msg)
{
    qDebug() << msg + QString::number(++a);
        QByteArray paquet = msg.toUtf8();
        //qDebug() << "message = envoyé par le serveur " + msg + " - destinataire = " + socket->peerAddress().toString();
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            socket->write(Utils::IntToArray(paquet.size()));
            socket->write(paquet);
            socket->waitForBytesWritten();
        }
}

void TcpThread::disconnected()
{
    qDebug() << "deconnexion du client";
    socket->deleteLater();
    emit deconnexion(socketDescriptor);
    exit(0);
}

void TcpThread::erreurSocket(QAbstractSocket::SocketError erreur)
{
    qDebug() << "le cient ne répond plus";
    qDebug() << socket->errorString();
}
