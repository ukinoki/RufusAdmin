#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "utils.h"

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit        TcpSocket(qintptr ID, QObject *parent = Q_NULLPTR);
    ~TcpSocket();
    QThread         thread;
    qintptr         sktdescriptor;
    void            run();
    void            envoyerMessage(QString msg);
    QAbstractSocket::SocketState state();
    void            setIdUser(int id);
    int             idUser();
    void            setData(QString datas);
    QString         getData();

signals:
    void            error(QTcpSocket::SocketError socketerror);
    void            emitmsg(qintptr sktdescriptor, QString msg);
    void            deconnexion(qintptr sktdescriptor);

public slots:
    void            TraiteDonneesRecues();
    void            Deconnexion();

private:
    int a;
    QTcpSocket      *socket;
    QByteArray      *buffer;                                                // le buffer stocke les data jusqu'à ce que tout le bloc soit reçu
    qint32*         sizedata;                                               // le stockage de la taille permet de savoir si le bloc a été reçu
    int             iduser;                                                 // stocke l'id corresondant au user correspondant à la connexion - utilisé pour la messagerie
    QString         datasclient;                                            // stocke l'adresse IP et l'adresse MAC du client

private slots:
    void            erreurSocket(QAbstractSocket::SocketError);
    void            Test();
};

#endif // TCPSOCKET_H