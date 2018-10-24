/* (C) 2018 LAINE SERGE
This file is part of RufusAdmin.

RufusAdmin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License,
or any later version.

RufusAdmin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RufusAdmin.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MACROS_H
#define MACROS_H

#define VERSION_BASE                                46                  // version de la base de données

// Les bases
#define NOM_BASE_CONSULTS                           "rufus"
#define NOM_BASE_COMPTA                             "ComptaMedicale"
#define NOM_BASE_OPHTA                              "Ophtalmologie"
#define NOM_BASE_IMAGES                             "Images"

// Les tables

#define NOM_TABLE_ARCHIVESBANQUE                    "ComptaMedicale.archivesbanques"
#define NOM_TABLE_BANQUES                           "ComptaMedicale.banques"
#define NOM_TABLE_COMPTES                           "ComptaMedicale.Comptes"
#define NOM_TABLE_RECETTES                          "ComptaMedicale.lignesrecettes"
#define NOM_TABLE_DEPENSES                          "ComptaMedicale.Depenses"
#define NOM_TABLE_LIGNESCOMPTES                     "ComptaMedicale.lignescomptes"
#define NOM_TABLE_REMISECHEQUES                     "ComptaMedicale.remisecheques"
#define NOM_TABLE_VERROUCOMPTAACTES                 "ComptaMedicale.verrouscomptaactes"

#define NOM_TABLE_DOCSASUPPRIMER                    "Images.DocsASupprimer"

#define NOM_TABLE_ACTES                             "rufus.Actes"
#define NOM_TABLE_APPAREILSCONNECTESCENTRE          "rufus.appareilsconnectescentre"
#define NOM_TABLE_COTATIONS                         "rufus.cotations"
#define NOM_TABLE_FLAGS                             "rufus.flags"
#define NOM_TABLE_IMPRESSIONS                       "rufus.Impressions"
#define NOM_TABLE_JOINTURESLIEUX                    "rufus.jointuresLieux"
#define NOM_TABLE_LIEUXEXERCICE                     "rufus.LieuxExercice"
#define NOM_TABLE_LISTEAPPAREILS                    "rufus.listeappareils"
#define NOM_TABLE_MOTIFSRDV                         "rufus.MotifsRDV"
#define NOM_TABLE_PARAMSYSTEME                      "rufus.ParametresSysteme"
#define NOM_TABLE_PATIENTS                          "rufus.Patients"
#define NOM_TABLE_SALLEDATTENTE                     "rufus.salledattente"
#define NOM_TABLE_UTILISATEURS                      "rufus.utilisateurs"
#define NOM_TABLE_USERSCONNECTES                    "rufus.UtilisateursConnectes"
#define NOM_TABLE_VILLES                            "rufus.Villes"

// Divers
#define IMAGERIE                                    "Imagerie"
#define DOCUMENTRECU                                "DocRecu"

#define NOM_ADMINISTRATEURDOCS                      "Admin"
#define NOM_POSTEIMPORTDOCS                         "posteimportdocs"

//les TCP
#define NOM_PORT_TCPSERVEUR                         "50885"
#define TCPMSG_idUser                               "idUser"
#define TCPMSG_MAJCorrespondants                    "MajCorrespondants"
#define TCPMSG_MAJSalAttente                        "MajSalAttente"
#define TCPMSG_MsgBAL                               "MsgRecuDansBAL"
#define TCPMSG_Separator                            "triumphspeedtriple1050"
#define TCPMSG_NonPrioritaire                       "ServeurNonPrioritaire"
#define TCPMSG_DataSocket                           "DataSocket"
#define TCPMSG_ListeSockets                         "ListeSockets"

// Les fichiers et répertoires divers
#define NOMDIR_LIBS2                                "/Applications";
#define NOMDIR_RESSOURCES                           "/Ressources"
#define NOMDIR_RUFUSADMIN                           "/Documents/RufusAdmin"
#define NOMDIR_LOGS                                 "/Logs"
#define NOMDIR_RUFUS                                "/Documents/Rufus"
#define NOMDIR_IMAGES                               "/Images"
#define NOMDIR_VIDEOS                               "/Videos"
#define NOMDIR_PROV                                 "/Prov"
#define NOMDIR_ECHECSTRANSFERTS                     "/EchecsTransferts"
#define NOMFIC_INI                                  "/Documents/RufusAdmin/RufusAdmin.ini"
#define NOMFIC_INIRUFUS                             "/Documents/Rufus/Rufus.ini"
#define SCRIPTBACKUPFILE    "/Documents/Rufus/RufusScriptBackup.sh"
#define SCRIPTPLISTFILE     "/Library/LaunchAgents/rufus.bup.plist"

// Variables mysql
#define MAX_ALLOWED_PACKET                          "16"

// Durée mise en veille
#define MISE_EN_VEILLE                              "180000"

// le son d'alarme
#define NOM_ALARME                                  "://goutte.wav"

// la taille maximale des images
#define TAILLEMAXIIMAGES                            262144

//les styles
#define UPGROUBOXINACTIVE                           "UpGroupBox {font: bold; border: 1px solid rgb(164,164,164); border-radius: 10px;}"
#define UPGROUBOXACTIVE                             "UpGroupBox {border: 2px solid rgb(164, 205, 255); border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E0E0E0, stop: 1 #FFFFFF);}"

// Constantes de statut salle d'attente  //+++++ATTENTION ces constantes servent dans des requêtes SQL - NE PAS METTRE DE CARACTERES SPECIAUX
#define ARRIVE                                      "Arrivé"
#define ENCOURS                                     "En cours"
#define ENATTENTENOUVELEXAMEN                       "En attente de nouvel examen par "
#define RETOURACCUEIL                               "Retour accueil"
#define ENCOURSEXAMEN                               "En cours de consultation avec "

#ifdef Q_OS_MACX
#define POLICEPARDEFAUT         "Comic Sans MS"
#define POINTPARDEFAUT          13
#define PATH_SPEC_MAC "/../../.."
#endif
#ifdef Q_OS_LINUX
#define POLICEPARDEFAUT         "Noto Sans MS"
#define POINTPARDEFAUT          9
#define LINUX "/../../.."
#endif
#ifdef Q_OS_WIN
#define POLICEPARDEFAUT         "Comic Sans MS"
#define POINTPARDEFAUT          13
#endif

#define OPHTALIBERAL            "ophlib"
#define OPHTAREMPLACANT         "ophrmp"
#define OPHTASALARIE            "ophsal"
#define OPHTANOCOMPTA           "ophnocompta"
#define OPHTAASSISTANT          "ophassistant"
#define ORTHOLIBERAL            "ortholib"
#define ORTHOREMPLACANT         "orthormp"
#define ORTHOSALARIE            "orthosal"
#define ORTHONOCOMPTA           "orthonocompta"
#define ORTHOASSISTANT          "orthoassistant"
#define AUTRESOIGNANTLIBERAL    "autresoignantlib"
#define AUTRESOIGNANTREMPLACANT "autresoignantrmp"
#define AUTRESOIGNANTSALARIE    "autresoignantsal"
#define AUTRESOIGNANTNOCOMPTA   "autrenocompta"
#define AUTRESOIGNANTASSISTANT  "autreassistant"
#define AUTREFONCTION           "a"
#define SECRETAIRE              "s"
#define SOCIETECOMPTABLE        "SEL"

//les TCP
#define TCPMSG_MAJCorrespondants                    "MajCorrespondants"                                 /* fin du message invitant à mettre à jour la liste des correspondants */
#define TCPMSG_MAJSalAttente                        "MajSalAttente"                                     /* fin du message invitant à mettre à jour la salle d'attente */
#define TCPMSG_MsgBAL                               "MsgRecuDansBAL"                                    /* fin du message informant de la réception d'un message de messagerie instantanée */

#define NOM_PORT_TCPSERVEUR                         "50885"
#define TCPMSG_idUser                               "idUser"                                            /* nom du message founissant l'idUser */
#define TCPMSG_Separator                            "triumphspeedtriple1050"                            /* separateur au sein d'un message */
#define TCPMSG_DataSocket                           "DataSocket"                                        /* fin du message fournissant IP et MAC d'un client */
#define TCPMSG_ListeSockets                         "ListeSockets"                                      /* fin du message informant le client que ce message contient la liste des TcpSockets */
#define TCPMSG_FaireLAppelSocket                    "AppelListe"                                        /* fin du message demandant la verification de l'absence de sockets déconnectés dans la liste */
#define TCPMSG_EnvoieListSocket                     "EnvoieListe"                                       /* fin du message demandant l'envoi de la liste des sockets à un client */

#endif // MACROS_H
