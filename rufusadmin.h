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

/*!
 * L'application RufusAdmin sert 4 taches:
 * 1. programmer les sauvegardes de la base de données et des fichiers d'images.
       Cette programmation n'est possible que si le pogramme est éxécuté sur la machine qui héberge le serveur pour des raisons techniques liés à Linux (cron table) et OSX (launchctl)
 * 2. enregistrer dans la base de données les liens vers les fichiers d'imagerie au fur et à mesure de leur création par les appareils d'imagerie
        . Un dossier qui va stocker toutes les images est créé à un endroit du réseau (en général un dossier Imagerie, sous dossier de Rufus)
        . Dans ce dossier 1 sous-dossier d'échange est créé, spécifique à chaque appareil d'imagerie
        . Chaque appareil exporte ses images au format pdf ou jpg dans ce dossier d'échange
        . un timer interne scrute le dossier d'échange de chaque appareil
        . 2 sous-dossiers sont créés dans le dossier Imagerie pour chaque appareil
            . un dossier NomdelAppareil_echecstranserts dans lequel sont enregistrés les fichiers qui n'ont pas pu être incorporés dans la base de données
            . un dossier NomdelAppareil_transfertOK où sont stockés les transferts réussis.
        . chaque fois qu'un nouveau fichier apparaît dans le fichier d'échange, le programme essaie de le reconnaître en identifiant l'id patient concerné dans le nom du fichier.
            Certains appareils                                                                                       ne permettent pas d'enregistrer un id, le programme utilise alors nom et prenom
            . s'il réussit
                . le fichier est renommé avec l'architecture idpatient_nompatient_prenompatient_dateexamen_typexamen plus l'extension jpg ou pdf
                . il est recopié dans le dossier NomdelAppareil_transfertOK
                . le lien vers ce fichier est enregistré dans la table Impression dans le champ lienversfichier
                . il est effacé du dossier d'échange
                . une ligne est crée dans le fichier 0journaltransferts_datedujour.txt du dossier NomdelAppareil_transfertsOK pour enregistrer l'opération
            . en cas d'échec
                . le fichier est recopié dans le dossier NomdelAppareil_echectransfert
                . il est effacé du dossier d'échange
                . une ligne est crée dans le fichier 0journaltransferts_datedujour.txt dans le dossier NomdelAppareil_echecstransferts pour enregistrer l'opération
 * 3. Alimenter la table Images.EcahngeImages dans laquelle seront intégrés chaque image correspondant au dossier patient ouvert.
      Cette table est alimentée à la demande avec les images de ce patient au fur et à mesure que le programme les demande
      La table est vidée avant chaque procédure de sauvegatde
 * 4. ëtre le serveur TCP du réseau local
*/

#ifndef RUFUSADMIN_H
#define RUFUSADMIN_H

#include <QDebug>
#include <QFileDialog>
#include <QHostInfo>
#include <QMainWindow>
#include <QSplashScreen>
#include <QSystemTrayIcon>
#include <poppler-qt5.h>
#include "dlg_gestionbanques.h"
#include "dlg_gestionusers.h"
#include "dlg_gestionlieux.h"
#include "dlg_message.h"
#include "dlg_motifs.h"
#include "dlg_paramconnexion.h"
#include "importdocsexternesthread.h"
#include "widgetbuttonframe.h"
#include "utils.h"
#include "tcpserver.h"
#include "tcpsocket.h"
#include "gbl_datas.h"
#include "cls_item.h"
#include "database.h"
#include "cls_user.h"
#include <QTimer>
#include "flags.h"

namespace Ui {
class RufusAdmin;
}

class RufusAdmin : public QMainWindow
{
    Q_OBJECT

public:
    explicit RufusAdmin(QWidget *parent = Q_NULLPTR);
    ~RufusAdmin();

private:
    Ui::RufusAdmin              *ui;
    dlg_gestionbanques          *Dlg_Banq;
    dlg_gestionusers            *Dlg_GestUsr;
    dlg_motifs                  *Dlg_motifs;
    dlg_paramconnexion          *DlgParam;
    Flags                       *flags;
    bool                        m_ok;
    int                         m_idadmindocs;
    int                         m_idlieuexeercice;
    int                         m_flagcorrespondants, m_flagsalledattente, m_flagmessages;
    int                         m_dureeVeille;
    QIcon                       ic_RufusAdmin;
    QMenu                       *trayIconMenu;
    QMap<QString,QIcon>         map_icons;
    QString                     m_nomfichierini;
    QString                     m_nomfichieriniRufus;
    QString                     m_nouvMDP, m_ancMDP, m_confirmMDP;
    QString                     m_domaine;
    QString                     m_nomLieuExercice;
    QStringList                 m_listeAppareils;
    QIcon                       ic_Backup, ic_Copy, ic_Erase, ic_Sunglasses, ic_SortirDossier, ic_OK, ic_Annul,
                                 ic_Euro,  ic_EuroCount,  ic_FermeAppuye,  ic_FermeRelache,  ic_Help,  ic_Null;
    QSettings                   *m_settings;
    ParametresSysteme           *m_parametres;
    DataBase                    *db;
    QSystemTrayIcon             *ictray_RufusAdminTrayIcon;
    QTimer                      *t_timerUserConnecte, *t_timerVerifDivers, *t_timerSupprDocs, *t_timerDocsAExporter, *t_timerProgressBar;
    QTimer                      t_timer;
    ImportDocsExternesThread    *ImportDocsExtThread;
    UpDialog                    *dlg_askAppareil, *dlg_askMDP;
    WidgetButtonFrame           *wdg_buttonframe;
    User                        *UserAdmin;

    qint64                      CalcBaseSize();
    int                         DetermineLieuExercice();
    bool                        eventFilter(QObject *obj, QEvent *event);
    void                        setMapDatas();
    QMap<QString, QIcon>        MapIcons();
    void                        closeEvent(QCloseEvent *);
    void                        AskAppareil();
    void                        ChoixMenuSystemTray(QString txt);
    void                        ConnexionBase();
    void                        Edit(QString txt, int delaieffacement=0);
    QStringList                 DecomposeScriptSQL(QString nomficscript);
    QString                     getDossierDocuments(QString Appareil);
    QString                     getExpressionSize(double size);
    void                        Message(QString mess, int pause = 1000, bool bottom = true);
    void                        EffaceMessage(int pause = 1000);
    void                        NouvAppareil();
    void                        Remplir_Table();
    void                        RestoreFontAppli();
    void                        SupprAppareil();
    void                        SupprimerDocsEtFactures();
    void                        setPosteImportDocs(bool a = true);
    bool                        SetUserAllData(User *usr);
    void                        ReconstruitListeLieuxExercice();
    bool                        VerifBase();
    bool                        VerifMDP(QString MDP, QString Msg);

    void                        ConnectTimers();
    void                        DisconnectTimers();
    void                        ConnectTimerInactive();
    void                        DisconnectTimerInactive();

    void                        ExporteDocs();
private slots:
    void                        AfficheMessageImport(QStringList listmsg, int pause, bool bottom=true);
    void                        ListeAppareils();
    void                        Slot_CalcExporteDocs();
    void                        Slot_ChoixButtonFrame(int);
    void                        Slot_ChoixDossierStockageApp();
    void                        Slot_ModifDirImagerie();
    void                        Slot_EnregistreAppareil();
    void                        Slot_EnregDossierStockageApp(QString dir);
    void                        Slot_EnregistreEmplacementServeur(int);
    void                        Slot_EnregistreNouvMDPAdmin();
    void                        Slot_GestionBanques();
    void                        Slot_GestionMotifs();
    void                        Slot_GestLieux();
    void                        Slot_GestUser();
    void                        Slot_MasqueAppli();
    void                        Slot_MetAJourLaConnexion();
    void                        Slot_ModifMDP();
    void                        Slot_ImportDocsExternes();
    void                        Slot_RestaureBase();
    void                        Slot_TrayIconMenu();
    void                        Slot_VerifPosteImport();
    void                        Slot_VerifVersionBase();

    //--------------------------------------------------------------------------------------------------------
    // les sauvegardes
    //--------------------------------------------------------------------------------------------------------
    /*! LA SAUVEGARDE DE LA BASE DE DONNEES

      La sauvegarde de la BDD peut-être planifiée dans le Qframe ui->Sauvegardeframe de la fiche dlg_param
      On peut planifier l'emplacement du fichier de sauvegarde, l'heure de la sauvegarde, et les jours de la sauvegarde.
      La sauvegarde ne peut se programmer que sur le serveur et pas ailleurs. Il faut donc installer une instance de Rufus sur le serveur.
      Les éléments du cadre ui->Sauvegardeframe sont donc désactivés si on n'est pas en mode Poste, autrement dit, sur le serveur.

      Les paramètres de programmation de la sauvegarde sont sauvegardés dans la base de données dans la table ParametresSyteme

      La sauvegarde se fait par le script RufusBackupScript.sh qui lance le prg mysqldump de sauvegarde des données et recopie les fichiers d'imagerie, les factures et les videos vers l'emplacement de sauvegarde.
      Ce script définit l'emplacement de la sauvegarde, le nom de la sauvegarde et détruit les sauvegardes datant de plus de 14 jours
      Le script RufusBackupScript.sh est situé dans le dossier /Users/nomdutilisateur/Documents/Rufus

      Le lancement de la sauvegarde au moment programmé se fait
        . Sous Mac,  par un autre script -> c'est le fichier xml rufus.bup.plist situé dans /Users/nomutilisateur/Library/LaunchAgents.
          Ce fichier est chargé au démarrage par le launchd Apple.
          Il est donc éxécuté même quand Rufus ne tourne pas
        . Sous Linux, c'est un timer t_timerbackup qui lance la sauvegarde et la fonction BackupWakeUp(QString NomDirDestination, QTime timebkup, Days days)

      Au chargement de Rufus, les données de Rufus.ini sont récupérées pour régler l'affichage des données dans  ui->Sauvegardeframe.

      Une modification de l'emplacement de sauvegarde se fait par un clic sur le bouton ui->DirBackuppushButton qui va lancer le slot Slot_ModifDirBachup()
            * ce slot va créer le fichier RufusScriptBackup.sh et enregistrer l'emplacement de sauvegarde dans rufus.ini
      Un changement d'heure ou de jour lance le slot Slot_ModifScriptList().
            * ce slot va modifier le fichier xml rufus.bup.plist, recharger ce fichier dans le launchd et enregistrer les données de programmation dans le rufusadmin.ini.
      Le bouton ui->EffacePrgSauvupPushButton réinitialise la programmation en déclenchant la fonction EffaceAutoBackup():
            * annule les données de programmation dans rufus.ini,`
            * réinitialise l'affichage dans ui->Sauvegardeframe,`
            * supprime le script de sauvegarde RufusBackupScript.sh
            * sous Mac, supprime le script de programmation rufus.bup.plist et le décharge du launchd
            * sous Linux, arrête le timer t_timerbackup
     */
    /*LA SAUVEGARDE DE LA BASE DE DONNEES

      La sauvegarde de la BDD peut-être planifiée dans le Qframe ui->Sauvegardeframe.
      On peut planifier l'emplacement du fichier de sauvegarde, l'heure de la sauvegarde, et les jours de la sauvegarde.
      La sauvegarde ne peut se programmer que sur le serveur et pas ailleurs. Il faut donc installer une instance de RufusAdmin sur le serveur.
      Les éléments du cadre ui->Sauvegardeframe sont donc désactivés si on n'est pas en mode Poste, autrement dit, sur le serveur.

      Les paramètres de programmation de la sauvegarde sont sauvegardés dans la table parametressystemequi ne sert en l'occurence qu'à gérer l'affichage des paramètres de sauvegarde

      La sauvegarde se fait par un script qui lance le prg mysqldump de sauvegarde des données.
      Ce script définit l'emplacement de la sauvegarde, le nom de la sauvegarde et détruit les sauvegardes datant de plus de 14 jours
      . pour Mac c'est le script RufusBackupScript.sh situé dans le dossier /Users/nomdutilisateur/Documents/Rufus
      La programmation de la sauvegarde se fait par un autre script qui va déterminer les jours de la semaine et l'heure de la sauvegarde.
      . Pour Mac, c'est le fichier xml rufus.bup.plist situé dans /Users/nomutilisateur/Library/LaunchAgents. Ce fichier est chargé au démarrage par le launchd Apple.

      Au chargement de RufusAdmin, les données de RufusAdmin.ini sont récupérées pour régler l'affichage des données dans  ui->Sauvegardeframe.

      Une modification de l'emplacement de sauvegarde se fait par un clic sur le bouton ui->DirBackuppushButton qui va lancer le slot Slot_ModifDirBachup()
            * ce slot va créer le fichier RufusScriptBackup.sh et enregistrer l'emplacement de sauvegarde dans rufusadmin.ini
      Un changement d'heure ou de jour lance le slot Slot_ModifScriptList().
            * ce slot va modifier le fichier xml rufus.bup.plist, recharger ce fichier dans le launchd et enregistrer les données de programmation dans le rufusadmin.ini.
      Le bouton ui->EffacePrgSauvupPushButton réinitialise la programmation en déclenchant le slot Slot_EffacePrgSauvegarde()
          Ce slot annule les données de programmation dans rufusadmin.ini,`
            réinitialise l'affichage dans ui->Sauvegardeframe,`
            supprime le script de sauvegarde RufusBackupScript.sh
            et le script de programmation rufus.bup.plist
            et, sur Mac, décharge ce fichier du launchd
     */
public:
    enum Day {
                Lundi       = 0x1,
                Mardi       = 0x2,
                Mercredi    = 0x4,
                Jeudi       = 0x8,
                Vendredi    = 0x10,
                Samedi      = 0x20,
                Dimanche    = 0x40
              };    Q_ENUM(Day)
    Q_DECLARE_FLAGS(Days, Day)
    QTimer                  t_timerbackup;
    bool                    ImmediateBackup(QString dirdestination = "", bool verifposteconnecte = true, bool full=false);
                            /*! lance un backup immédiat */
private slots:
    void                    Slot_ImmediateBackup();

private:
    qint64                  m_basesize, m_imagessize, m_videossize, m_freespace;
    UpDialog                *dlg_askBupRestore;
    UpLabel                 *wdg_resumelbl, *wdg_volumelibrelbl;
    void                    AskBupRestore(bool restore, QString pathorigin, QString pathdestination, bool OKini = true, bool OKRessces = true, bool OKimages = true, bool OKvideos = true);
                            /*! crée le script RufusScriptBackup.sh qui va éxécuter la sauvegarde */
    void                    BackupWakeUp(QString NomDirDestination, QTime timebkup, Days days);
                            /*! sous Linux, déclenche le backup au moment programmé */
    void                    CalcTimeBupRestore();
                            /*! calcule la durée approximative du backup */
    void                    DefinitScriptBackup(QString NomDirDestination, QString NomDirStockageImagerie, bool AvecImages= true, bool AvecVideos = true);
                            /*! crée le script RufusScriptBackup.sh qui va éxécuter la sauvegarde */
    void                    EffaceBDDDataBackup();
                            /*! efface les données de sauvegarde (moment et emplacement) dans la base de données */
    void                    EffaceProgrammationBackup();
                            /*! efface le paramétrage de la sauvegarde
                            * suppression de RufusScriptBackup.sh
                            * suppression de rufus.bup.plist sous Mac et arrêt du timer t_timerbackup sous Linux
                            */
    void                    InitBackupAuto();
                            /*! prépare le paramétrage de la fonction ParamAutoBackup() en fonction des paramètres enregistrés dans la base */
    void                    ModifDateHeureBackup();
                            /*! modifie l'heure ou la date du backup automatique et relance InitBackupAuto() */
    void                    ModifDirBackup();
                            /*! modifie le dossier de destination du backup automatique et relance InitBackupAuto() */
    void                    ParamAutoBackup(QString dirdestination, QString dirimagerie, QTime timebackup, Days days);
                            /*! paramètre le moment et l'emplacement de la sauvegarde
                             * sous Mac, crée le fichier xml rufus.bup.plist
                             * sous Linux, lance le timer t_timerbackup
                            */
    //--------------------------------------------------------------------------------------------------------
    // fin sauvegardes
    //--------------------------------------------------------------------------------------------------------
    void                    ProgrammeSQLVideImagesTemp(QTime timebackup);   /*! programme l'effacement des données temporaires d'imageire
                                                                             * vide la table EchangeImages
                                                                             * purge les champs jpg et pdf de la table Factures
                                                                             */

    // TCPServer, TCPSocket
    bool                m_utiliseTCP;
    TcpServer           *TCPServer;
    quint16             m_portTCPserver;
    QTimer              *t_timerSalDatCorrespMsg, *t_timerVerifVerrou;
    QString             m_IPadress, m_macAdress;
    QString             m_socketStatut;
    QDateTime           m_dateDernierMessage;
    void                ResumeTCPSocketStatut();

    void                VerifModifsFlags();
    void                VerifVerrouDossier();
signals:
    void                ModifEdit(QString txt);

};

#endif // RUFUSADMIN_H
