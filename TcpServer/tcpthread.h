#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "utils.h"

class TcpThread : public QThread
{
    Q_OBJECT
public:
    explicit        TcpThread(qintptr ID, QObject *parent = Q_NULLPTR);
    void            run();
    void            envoyerMessage(QString msg);
    QAbstractSocket::SocketState state();
signals:
    void            error(QTcpSocket::SocketError socketerror);
    void            emitmsg(qintptr sktdescriptor, QString msg);
    void            deconnexion(qintptr sktdescriptor);

public slots:
    void            TraiteDonneesRecues();
    void            disconnected();

private:
    int a=0;
    QTcpSocket      *socket;
    qintptr         socketDescriptor;
    QByteArray      *buffer;                                                // le buffer stocke les data jusqu'à ce que tout le bloc soit reçu
    qint32*         sizedata;                                               // le stockage de la taille permet de savoir si le bloc a été reçu
    int             iduser;                                                 // stocke l'id corresondant au user correspondant à la connexion - utilisé pour la messagerie
    QString         dataclient;                                             // stocke l'adresse IP et l'adresse MAC du client - utilisé pour le changement de TcpServer
private slots:
    void            erreurSocket(QAbstractSocket::SocketError);
    void            Test();
};

#endif // TCPTHREAD_H
