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

#ifndef MACROS_H
#define MACROS_H

// Les versions de la base et des ressources
#define VERSION_BASE                                56  // version de la base de données
#define VERSION_RESSOURCES                          34  // version des fichiers de ressources

// Les bases
#define DB_CCAM                               "CCAM"
#define DB_CONSULTS                           "rufus"
#define DB_COMPTA                             "ComptaMedicale"
#define DB_OPHTA                              "Ophtalmologie"
#define DB_IMAGES                             "Images"

// Les tables
#define TBL_ARCHIVESBANQUE                    "ComptaMedicale.archivesbanques"
#define TBL_BANQUES                           "ComptaMedicale.banques"
#define TBL_COMPTES                           "ComptaMedicale.Comptes"
#define TBL_DEPENSES                          "ComptaMedicale.Depenses"
#define TBL_FACTURES                          "ComptaMedicale.Factures"
#define TBL_LIGNESCOMPTES                     "ComptaMedicale.lignescomptes"
#define TBL_LIGNESPAIEMENTS                   "ComptaMedicale.lignespaiements"
#define TBL_RECETTES                          "ComptaMedicale.lignesrecettes"
#define TBL_RECETTESSPECIALES                 "ComptaMedicale.autresrecettes"
#define TBL_REMISECHEQUES                     "ComptaMedicale.remisecheques"
#define TBL_RUBRIQUES2035                     "ComptaMedicale.rubriques2035"
#define TBL_TIERS                             "ComptaMedicale.tiers"
#define TBL_TYPEPAIEMENTACTES                 "ComptaMedicale.typepaiementactes"
#define TBL_VERROUCOMPTAACTES                 "ComptaMedicale.verrouscomptaactes"

#define TBL_ECHANGEIMAGES                     "Images.EchangeImages"
#define TBL_DOCSASUPPRIMER                    "Images.DocsASupprimer"
#define TBL_FACTURESASUPPRIMER                "Images.FacturesASupprimer"

#define TBL_BILANORTHO                        "Ophtalmologie.bilanortho"
#define TBL_BIOMETRIES                        "Ophtalmologie.biometries"
#define TBL_COMMENTAIRESLUNETTES              "Ophtalmologie.commentaireslunettes"
#define TBL_DONNEES_OPHTA_PATIENTS            "Ophtalmologie.DonneesOphtaPatients"
#define TBL_REFRACTION                        "Ophtalmologie.refractions"
#define TBL_TONOMETRIE                        "Ophtalmologie.tonometries"

#define TBL_ACTES                             "rufus.Actes"
#define TBL_APPAREILSCONNECTESCENTRE          "rufus.appareilsconnectescentre"
#define TBL_APPAREILSREFRACTION               "rufus.AppareilsRefraction"
#define TBL_CCAM                              "rufus.ccam"
#define TBL_CORRESPONDANTS                    "rufus.correspondants"
#define TBL_COTATIONS                         "rufus.cotations"
#define TBL_COURRIERS                         "rufus.courriers"
#define TBL_DONNEESSOCIALESPATIENTS           "rufus.donneessocialespatients"
#define TBL_FLAGS                             "rufus.flags"
#define TBL_IMPRESSIONS                       "rufus.Impressions"
#define TBL_JOINTURESDOCS                     "rufus.jointuresdocuments"
#define TBL_JOINTURESLIEUX                    "rufus.jointuresLieux"
#define TBL_LIEUXEXERCICE                     "rufus.LieuxExercice"
#define TBL_LISTEAPPAREILS                    "rufus.listeappareils"
#define TBL_LISTETIERS                        "rufus.listetiers"
#define TBL_MESSAGES                          "rufus.Messagerie"
#define TBL_MESSAGESJOINTURES                 "rufus.MessagerieJointures"
#define TBL_METADOCUMENTS                     "rufus.Metadocuments"
#define TBL_MOTIFSRDV                         "rufus.MotifsRDV"
#define TBL_MOTSCLES                          "rufus.MotsCles"
#define TBL_MOTSCLESJOINTURES                 "rufus.MotsClesJointures"
#define TBL_PARAMSYSTEME                      "rufus.ParametresSysteme"
#define TBL_PATIENTS                          "rufus.Patients"
#define TBL_RENSEIGNEMENTSMEDICAUXPATIENTS    "rufus.renseignementsmedicauxpatients"
#define TBL_SALLEDATTENTE                     "rufus.salledattente"
#define TBL_SPECIALITES                       "rufus.specialitesmedicales"
#define TBL_UTILISATEURS                      "rufus.utilisateurs"
#define TBL_USERSCONNECTES                    "rufus.UtilisateursConnectes"
#define TBL_VILLES                            "rufus.Villes"

// Les champs
                            //! Table Actes
#define CP_MOTIF_ACTES                               "ActeMotif"
#define CP_TEXTE_ACTES                               "ActeTexte"
#define CP_CONCLUSION_ACTES                          "ActeConclusion"
#define CP_ACTEDATE_ACTES                            "ActeDate"
#define CP_IDUSER_ACTES                              "idUser"
#define CP_IDUSERPARENT_ACTES                        "UserParent"
#define CP_IDUSERCOMPTABLE_ACTES                     "UserComptable"
#define CP_COURRIERAFAIRE_ACTES                      "ActeCourrierafaire"

                            //! Table renseignementsmedicauxpatients
#define CP_ATCDTSOPH_RMP                             "RMPAtcdtsOphs"
#define CP_TRAITMTOPH_RMP                            "RMPTtOphs"
#define CP_IMPORTANT_RMP                             "Important"
#define CP_RESUME_RMP                                "Resume"
#define CP_ATCDTSPERSOS_RMP                          "RMPAtcdtsPersos"
#define CP_ATCDTSFAMLXS_RMP                          "RMPAtcdtsFamiliaux"
#define CP_TRAITMTGEN_RMP                            "RMPTtGeneral"
#define CP_AUTRESTOXIQUES_RMP                        "Autrestoxiques"
#define CP_IDMG_RMP                                  "idCorMedMG"
#define CP_IDSPE1_RMP                                "idCorMedSpe1"
#define CP_IDSPE2_RMP                                "idCorMedSpe2"
#define CP_TABAC_RMP                                 "Tabac"

                            //! Table Messagerie
#define CP_TEXTMSG_MESSAGERIE                        "TexteMessage"

                            //! Table salledattente
#define CP_IDPAT_SALDAT                              "idPat"
#define CP_IDUSER_SALDAT                             "idUser"
#define CP_STATUT_SALDAT                             "Statut"
#define CP_HEURESTATUT_SALDAT                        "HeureStatut"
#define CP_HEURERDV_SALDAT                           "HeureRDV"
#define CP_HEUREARRIVEE_SALDAT                       "HeureArrivee"
#define CP_MOTIF_SALDAT                              "Motif"
#define CP_MESSAGE_SALDAT                            "Message"
#define CP_IDACTEAPAYER_SALDAT                       "idActeAPayer"
#define CP_POSTEEXAMEN_SALDAT                        "PosteExamen"
#define CP_IDUSERENCOURSEXAM_SALDAT                  "idUserEnCoursExam"
#define CP_IDSALDAT_SALDAT                           "idSalDat"

                            //! Table ComptaMedicale.banques
#define CP_IDBANQUE_BANQUES                          "idBanque"
#define CP_IDABREGE_BANQUES                          "idBanqueAbrege"
#define CP_NOMBANQUE_BANQUES                         "NomBanque"
#define CP_NOMABREGE_BANQUES                         "Codeanque"

                            //! Table ComptaMedicale.Comptes
#define CP_IDCOMPTE_COMPTES                          "idCompte"
#define CP_IDBANQUE_COMPTES                          "idBanque"
#define CP_IDUSER_COMPTES                            "idUser"
#define CP_IBAN_COMPTES                              "IBAN"
#define CP_INTITULE_COMPTES                          "IntituleCompte"
#define CP_NOMABREGE_COMPTES                         "NomCompteAbrege"
#define CP_SOLDE_COMPTES                             "SoldeSurDernierReleve"
#define CP_PARTAGE_COMPTES                           "Partage"
#define CP_DESACTIVE_COMPTES                         "Desactive"

                            //! Table ComptaMedicale.Depenses
#define CP_IDDEPENSE_DEPENSES                       "idDep"
#define CP_IDUSER_DEPENSES                          "idUser"
#define CP_DATE_DEPENSES                            "DateDep"
#define CP_REFFISCALE_DEPENSES                      "RefFiscale"
#define CP_OBJET_DEPENSES                           "Objet"
#define CP_MONTANT_DEPENSES                         "Montant"
#define CP_FAMILLEFISCALE_DEPENSES                  "FamFiscale"
#define CP_IDOPERATION_DEPENSES                     "Nooperation"
#define CP_MONNAIE_DEPENSES                         "Monnaie"
#define CP_IDRECETTE_DEPENSES                       "idRec"
#define CP_MODEPAIEMENT_DEPENSES                    "ModePaiement"
#define CP_COMPTE_DEPENSES                          "Compte"
#define CP_NUMCHEQUE_DEPENSES                       "Nocheque"
#define CP_IDFACTURE_DEPENSES                       "idFacture"

// Divers
#define PDF                                         "pdf"
#define JPG                                         "jpg"

#define BILANORTHOPTIQUE                            "BO"
#define CORRECTION                                  "Correction"
#define COURRIER                                    "Courrier"
#define DOCUMENTRECU                                "DocRecu"
#define IMAGERIE                                    "Imagerie"
#define ORHTOPTIE                                   "Orthoptie"
#define PRESCRIPTION                                "Prescription"
#define PRESCRIPTIONLUNETTES                        "PrescrLun"
#define COURRIERADMINISTRATIF                       "DocAdmin"
#define VIDEO                                       "Video"
#define FACTURE                                     "Facture"
#define ECHEANCIER                                  "Echéancier"

#define NOM_APPLI                                   "Rufus"

#define NOM_POSTEIMPORTDOCS                         "posteimportdocs"
#define NOM_ADMINISTRATEURDOCS                      "Admin"
#define NOM_MDPADMINISTRATEUR                       "bob"

//les TCP
#define NOM_PORT_TCPSERVEUR                         "50885"
#define TCPMSG_idUser                               "idUser"                                            /*! nom du message fournissant l'idUser */
#define TCPMSG_MAJCorrespondants                    "MajCorrespondants"                                 /*! fin du message invitant à mettre à jour la liste des correspondants */
#define TCPMSG_MAJDocsExternes                      "MajDocsExternes"                                   /*! fin du message invitant à mettre à jour la liste des documents externes pour un patient */
#define TCPMSG_MAJSalAttente                        "MajSalAttente"                                     /*! fin du message invitant à mettre à jour la salle d'attente */
#define TCPMSG_MAJListePatients                     "MajListePatients"                                  /*! fin du message invitant à mettre à jour la liste des patients */
#define TCPMSG_MsgBAL                               "MsgRecuDansBAL"                                    /*! fin du message informant de la réception d'un message de messagerie instantanée */
#define TCPMSG_Separator                            "triumphspeedtriple1050"                            /*! separateur au sein d'un message */
#define TCPMSG_DataSocket                           "DataSocket"                                        /*! fin du message fournissant IP et MAC d'un client */
#define TCPMSG_ListeSockets                         "ListeSockets"                                      /*! fin du message informant le client que ce message contient la liste des TcpSockets */
#define TCPMSG_EnvoieListSocket                     "EnvoieListe"                                       /*! fin du message demandant l'envoi de la liste des sockets à un client */
#define TCPMSG_Disconnect                           "Deconnexion"                                       /*! fin du message demandant la deconnexion et la fermeture d'un socket */
#define TCPMSG_TestConnexion                        "TestConnexion"                                     /*! fin du message de test de la connexion */
#define TCPMSG_OKConnexion                          "OKConnexion"                                       /*! fin du message de confirmation de la connexion */

// Constantes de statut salle d'attente  //+++++ATTENTION ces constantes servent dans des requêtes SQL - NE PAS METTRE DE CARACTERES SPECIAUX
#define ARRIVE                                      "Arrivé"
#define ENCOURS                                     "En cours"
#define ENATTENTENOUVELEXAMEN                       "En attente de nouvel examen par "
#define RETOURACCUEIL                               "Retour accueil"
#define ENCOURSEXAMEN                               "En cours de consultation avec "

// Les fichiers et répertoires divers
#define DIR_LIBS2                                "/Applications";
#define DIR_RESSOURCES                           "/Ressources"
#define DIR_RUFUSADMIN                           "/Documents/RufusAdmin"
#define DIR_RUFUS                                "/Documents/Rufus"
#define DIR_FACTURES                             "/Factures"
#define DIR_ORIGINAUX                            "/Originaux"
#define DIR_FACTURESSANSLIEN                     "/FacturesSansLien"
#define DIR_DOSSIERECHANGE                       "/DossierEchange"
#define DIR_CRDOSSIERS                           "/CRDossiers"
#define DIR_IMAGERIE                             "/Imagerie"
#define DIR_IMAGES                               "/Images"
#define DIR_VIDEOS                               "/Videos"
#define DIR_PROV                                 "/Prov"
#define DIR_LOGS                                 "/Logs"
#define DIR_ECHECSTRANSFERTS                     "/EchecsTransferts"

#define FILE_ENTETEORDO                           "/Documents/Rufus/Ressources/Entete_Ordonnance.txt"
#define FILE_CORPSORDO                            "/Documents/Rufus/Ressources/Corps_Ordonnance.txt"
#define FILE_ENTETEORDOALD                        "/Documents/Rufus/Ressources/Entete_OrdoALD.txt"
#define FILE_CORPSORDOALD                         "/Documents/Rufus/Ressources/Corps_OrdoALD.txt"
#define FILE_PIEDPAGE                             "/Documents/Rufus/Ressources/Pied_Ordonnance.txt"
#define FILE_PIEDORDOLUNETTES                     "/Documents/Rufus/Ressources/Pied_Ordonnance_Lunettes.txt"
#define FILE_PDF                                  "/Documents/Rufus/Ressources/pdf.pdf"
#define FILE_TEST                                 "/Documents/Rufus/Ressources/test.txt"
#define FILE_INI                                  "/Documents/RufusAdmin/RufusAdmin.ini"
#define FILE_INIRUFUS                             "/Documents/Rufus/Rufus.ini"
#define SCRIPTBACKUPFILE                            "/Documents/Rufus/RufusScriptBackup.sh"
#define SCRIPTRESTOREFILE                           "/Documents/Rufus/RufusScriptRestore.sh"
#define SCRIPTPLISTFILE                             "/Library/LaunchAgents/rufus.bup.plist"

// Variables mysql
#define MAX_ALLOWED_PACKET                          "16"

// Durée mise en veille
#define MISE_EN_VEILLE                              "120000"

// le son d'alarme
#define NOM_ALARME                                  "://goutte.wav"

// la largeur de la formule de réfraction dans le champ ActeTexte
#define LARGEUR_FORMULE                             "310"

// le lien vers la CCAM
#define LIEN_CCAM                                   "https://www.ameli.fr/accueil-de-la-ccam/trouver-un-acte/consultation-par-chapitre.php?chap=a%3A0%3A%7B%7D&add=2#chapitre_2"

// la taille maximale des images
#define TAILLEMAXIIMAGES                            262144

//les styles
#define UPGROUBOXINACTIVE                           "UpGroupBox {font: bold; border: 1px solid rgb(164,164,164); border-radius: 10px;}"
#define UPGROUBOXACTIVE                             "UpGroupBox {border: 2px solid rgb(164, 205, 255); border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E0E0E0, stop: 1 #FFFFFF);}"

// la police d'écran par défaut et le facteur de correction des td width html en impression qui varie entre Linux et OSX (???)
#ifdef Q_OS_MACX
#define POLICEPARDEFAUT         "Comic Sans MS"
#define POINTPARDEFAUT          13
#define CORRECTION_td_width     1
#endif
#ifdef Q_OS_LINUX
#define POLICEPARDEFAUT         "Comic Sans MS"
#define POINTPARDEFAUT          10
#define CORRECTION_td_width     1.3
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

#define HAUTEUR_SANS_ORDONNANCE_MINI                    191                 // Hauteur du formulaire Refraction sans la partie ordonnance sans detail sans observ
#define HAUTEUR_SANS_ORDONNANCE_AVEC_DETAIL             335                 // Hauteur du formulaire Refraction sans la partie ordonnance avec detail sans observ
#define HAUTEUR_AVEC_ORDONNANCE_SANS_DETAIL             450                 // Hauteur du formulaire Refraction avec partie ordonnance sans detail
#define HAUTEUR_AVEC_ORDONNANCE_AVEC_DETAIL             590                 // Hauteur du formulaire Refraction avec partie ordonnance avec detail
#define LARGEURLISTE   1300
#define LARGEURNORMALE 1260
#define HAUTEUR_MINI        280
#define HAUTEUR_NORMALE     970

//Opérations bancaires
#define VIREMENT            "Virement"
#define CHEQUE              "Chèque"
#define PRELEVEMENT         "Prélèvement"
#define TIP                 "TIP"
#define ESPECES             "Espèces"
#define CARTECREDIT         "Carte de crédit"

// ancres et comentaires html
#define HTMLCOMMENT_LINUX           "<!LINUX>"
#define HTMLCOMMENT_MAC             "<!MAC>"
#define HTMLANCHOR_BODEBUT          "<BOdebut>"
#define HTMLANCHOR_BOFIN            "<BOfin>"


#endif // MACROS_H
