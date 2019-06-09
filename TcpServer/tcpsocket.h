/* (C) 2018 LAINE SERGE
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

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "utils.h"
#include "database.h"
#include "log.h"
#include "gbl_datas.h"

            /*! L'UTILISATION DU TCP PAR RUFUS
            * Rufus peut fonctionner en réseau de 2 manières, avec ou sans RufusAdmin.
            * Certaines tâches requièrent l'échange d'informations entre postes:
                * modification de la salle d'attente,
                * modification de la liste des correspondants,
                * envoi de messages entre postes
                * enregistrement d'un nouveau document externe pour un patient

            * A SANS RUFUS ADMIN = UTILISATION DE TIMERS
                * Il n'y a pas d'envoi direct d'informations d'un poste à un autre
                * chaque poste scrute à intervalle régulier le contenu de la table rufus.flags pour savoir si
                    * la salle d'attente a été modifiée
                    * la liste de correspondants a été modifiée
                    * de nouveaux messages sont échangés
                * quand un poste affiche le dossier d'un patient, il scrute en plus la table documents pour savoir si de nouveaux documents sont arrivés pour ce patient
                * chaque poste met à jour les flags correspondants quand il modifie la salle d'attente, la liste de correspondants ou quand il envoie un message
                * Bien entendu, ce système consomme des ressources au niveau du serveur et ne peut donc pas être envisagé qund le nombre de postes sur le réseau est élevé.

            * B AVEC RUFUSADMIN = AVEC TCP
                * chaque poste envoie un message TCP à RufusAdmin quand
                    * il a modifié la salle d'attente
                    * il a modifié la liste de correspondants
                    * il a envoyé un message à un ou plusieurs autres postes
                    * RufusAdmin traite et rediffuse le message vers les postes concernés, toujours par TCP
                * quand RufusAdmin enregistre un nouveau document, il en informe le poste qui affiche le dossier correspondant à ce document.

            * C LES POSTES DISTANTS
                * avec ou sans RufusAdmin, les postes distants fonctionnent sans TCP
            * RufusAdmin continue à surveiller et à mettre à jour les flags correspondants pour informer/surveiller les postes distants qui fonctionnent sans TCP
            */

/*
 * les règles non écrites
 * pas de classe héritée de thread
 * pas de slot appelé dans un thread depuis l'extérieur autrement que par un signal
 */

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    static TcpSocket*   I();
    TcpSocket(qintptr ID, QTcpSocket *parent = Q_NULLPTR);
    ~TcpSocket();
    QThread         thread;
    qintptr         sktdescriptor;
    void            envoyerMessage(QString msg);
    QAbstractSocket::SocketState state();
    void            setIdUser(int id);
    int             idUser();
    void            setData(QString datas);
    QString         getData();
    bool            TcpConnectToServer(QString ipadrserver = "");   /* Crée la connexion avec le TcpServer sur le réseau */

signals:
    void            error(QTcpSocket::SocketError socketerror);
    void            emitmsg(qintptr sktdescriptor, QString msg);
    void            deconnexion(qintptr sktdescriptor);

public slots:
    void            TraiteDonneesRecues();
    void            Deconnexion();

private:
    int a;
    TcpSocket();
    static TcpSocket    *instance;
    QTcpSocket      *socket;
    quint16             PortTCPServer;
    QByteArray      buffer;                                                 // le buffer stocke les data jusqu'à ce que tout le bloc soit reçu
    qint32          sizedata;                                               // le stockage de la taille permet de savoir si le bloc a été reçu
    int             iduser;                                                 // stocke l'id correspondant au user correspondant à la connexion - utilisé pour la messagerie
    QString         datasclient;                                            // stocke l'adresse IP, l'adresse MAC du client et le nom du poste connecté

private slots:
    void            erreurSocket(QAbstractSocket::SocketError);
};

#endif // TCPSOCKET_H
