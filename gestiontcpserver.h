#ifndef GESTIONTCPSERVER_H
#define GESTIONTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "upmessagebox.h"
#include "dlg_message.h"
#include "utils.h"
#include <QTime>
#include <QTimer>
#include "gbl_datas.h"

class GestionTcPServer : public QTcpServer
{
    Q_OBJECT
public:
    static GestionTcPServer* getInstance();
    bool        start();
    void        envoyerATous(QString msg, QTcpSocket *emetteurorigin = Q_NULLPTR);      /* envoi de message commun sur tous les sockets - emetteurorigin sera exclu de la liste des destinataires */
    QString     ListeSockets();
    void        setId(int id);
    void        envoyerA(int iduser, QString msg);                                      /* envoi d'un message sur un QTcpSocket en particulier */

private:
    static GestionTcPServer*        instance;
    GestionTcPServer();

    int                             m_id;
    void                            nouvelleconnexion();
    void                            envoieListeSockets(QTcpSocket *tcl = Q_NULLPTR);    /* envoie la liste des sockets à tous les sockets */
    QMap<QTcpSocket*, QByteArray*>  buffers;                                            // le buffer stocke les data jusqu'à ce que tout le bloc soit reçu
    QMap<QTcpSocket*, qint32*>      sizes;                                              // le stockage de la taille permet de savoir si le bloc a été reçu
    QMap<QTcpSocket*, int>          idusers;                                            // stocke l'id corresondant au user correspondant à la connexion - utilisé pour la messagerie
    QMap<QTcpSocket*, QString>      dataclients;                                        // stocke l'adresse IP et l'adresse MAC du client - utilisé pour le changement de TcpServer

    void                            TraiteMessageRecu(QTcpSocket *tclient, QString msg);/* traitement des messages reçus*/
    void                            Deconnexion(QTcpSocket* tcl);                       /* deconnexion d'un socket */

    void                            envoyerA(QTcpSocket *tcl, QString msg);             /* envoi d'un message sur un QTcpSocket en particulier */
    void                            AfficheListeSockets(QString fonction = "");         /* utilisé pour le debugging seulement */
    QString                         gListeSockets;
    int                             idAdmin;

private slots:
    void                            TraiteDonneesRecues();                              /* traduction des messages reçus*/
    void                            DeconnexionParLeSocket();                           /* deconnexion d'un socket de lui même */

signals:
    void                            ModifListeSockets();
};

#endif // GESTIONTCPSERVER_H
