#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "upmessagebox.h"
#include "dlg_message.h"
#include "utils.h"
#include <QTime>
#include <QTimer>
#include "gbl_datas.h"
#include "tcpthread.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static      TcpServer* getInstance();
    bool        start();
    void        envoyerATous(QString msg, qintptr emetteurorigin = 0);        /* envoi de message commun sur tous les sockets - emetteurorigin sera exclu de la liste des destinataires */
    QString     ListeSockets();
    void        setId(int id);
    void        envoyerA(int iduser, QString msg);                                      /* envoi d'un message sur un QTcpSocket en particulier */

private:
    static TcpServer*        instance;
    TcpServer();

    int                             m_id;
    void                            envoieListeSockets(qintptr sktdescriptor = 0);      /* envoie la liste des sockets à tous les socketdescriptors */
    QMap<QTcpSocket*, QByteArray*>  buffers;                                            // le buffer stocke les data jusqu'à ce que tout le bloc soit reçu
    QMap<qintptr, TcpThread*>       socketthreads;                                      // le mapping des thread à partir des scoketdescriptor
    QMap<qintptr, int>              idusers;                                            // stocke l'id corresondant au user correspondant à la connexion - utilisé pour la messagerie
    QMap<qintptr, QString>          dataclients;                                        // stocke l'adresse IP et l'adresse MAC du client - utilisé pour le changement de TcpServer

    void                            AfficheListeSockets(QString fonction = "");         /* utilisé pour le debugging seulement */
    QString                         gListeSockets;
    int                             idAdmin;
    TcpThread*                      getThreadFromDescriptor(qintptr socketdescriptor);

signals:
    void                            ModifListeSockets();

private slots:
    void                            TraiteMessageRecu(qintptr sktdescriptor, QString msg);/* traitement des messages reçus*/
    void                            Deconnexion(qintptr sktdescriptor);                 /* deconnexion d'un socket */

protected:
    void                            incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
