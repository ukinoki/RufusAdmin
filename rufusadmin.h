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

    bool                        AutresPostesConnectes();
    int                         DetermineLieuExercice();
    bool                        eventFilter(QObject *obj, QEvent *event);
    void                        setMapIcons();
    QMap<QString, QIcon>        MapIcons();
    void                        closeEvent(QCloseEvent *);
    void                        AskAppareil();
    void                        CalcExporteDocs();
    void                        ChoixButtonFrame();
    void                        ChoixMenuSystemTray(QString txt);
    void                        ConnexionBase();
    QStringList                 DecomposeScriptSQL(QString nomficscript);
    void                        Edit(QString txt, int delaieffacement=0);
    void                        EnregistreAppareil();
    void                        EnregistreEmplacementServeur(int);
    QString                     getDossierDocuments(QString Appareil);
    void                        Message(QString mess, int pause = 1000, bool bottom = true);
    void                        EffaceMessage(int pause = 1000);
    void                        EnregistreNouvMDPAdmin();
    void                        GestionBanques();
    void                        GestionMotifs();
    void                        GestionLieux();
    void                        GestionUsers();
    void                        ImportDocsExternes();
    void                        ListeAppareils();
    void                        MasqueAppli();
    void                        MetAJourLaConnexion();
    void                        ModifDirImagerie();
    void                        ModifMDP();
    void                        NouvAppareil();
    void                        Remplir_Table();
    void                        RestoreFontAppli();
    void                        SupprAppareil();
    void                        SupprimerDocsEtFactures();
    void                        setPosteImportDocs(bool a = true);
    bool                        SetUserAllData(User *usr);
    void                        ReconstruitListeLieuxExercice();
    void                        TrayIconMenu();
    bool                        VerifBase();
    bool                        VerifMDP(QString MDP, QString Msg);
    void                        VerifPosteImport();
    void                        VerifVersionBase();

    void                        ConnectTimers();
    void                        DisconnectTimers();
    void                        ConnectTimerInactive();
    void                        DisconnectTimerInactive();

    void                        ExporteDocs();
private slots:
    void                        AfficheMessageImport(QStringList listmsg, int pause, bool bottom=true);
    void                        Slot_ChoixDossierStockageApp();
    void                        Slot_EnregDossierStockageApp(QString dir);


    //--------------------------------------------------------------------------------------------------------
    // les sauvegardes
    //--------------------------------------------------------------------------------------------------------
    /*! LA SAUVEGARDE DE LA BASE DE DONNEES

      La sauvegarde de la BDD peut-être planifiée dans le Qframe ui->Sauvegardeframe
      On peut planifier l'emplacement du fichier de sauvegarde, l'heure de la sauvegarde, et les jours de la sauvegarde.
      La sauvegarde ne peut se programmer que sur le serveur et pas ailleurs. Il faut donc installer une instance de RufusAdmin sur le serveur.
      Les éléments du cadre ui->Sauvegardeframe sont donc désactivés si on n'est pas en mode Poste, autrement dit, sur le serveur.

      Les paramètres de programmation de la sauvegarde sont sauvegardés dans la base de données dans la table ParametresSyteme

      La sauvegarde se fait par le script RufusBackupScript.sh qui lance le prg mysqldump de sauvegarde des données et recopie les fichiers d'imagerie, les factures et les videos vers l'emplacement de sauvegarde.
      Ce script définit l'emplacement de la sauvegarde, le nom de la sauvegarde et détruit les sauvegardes datant de plus de 14 jours
      Le script RufusBackupScript.sh est situé dans le dossier /Users/nomdutilisateur/Documents/Rufus

      Le lancement de la sauvegarde au moment programmé se fait
        . Sous Mac,  par un autre script -> c'est le fichier xml rufus.bup.plist situé dans /Users/nomutilisateur/Library/LaunchAgents.
          Ce fichier est chargé au démarrage par le launchd Apple.
          Il est donc éxécuté même quand Rufus ne tourne pas
        . Sous Linux, c'est un timer t_timerbackup qui lance la sauvegarde et la fonction BackupWakeUp()

      Au lancement du programme,
            * les paramètres de sauvegarde automatique sont récupérés pour régler l'affichage des données dans  ui->Sauvegardeframe.
            * si le programme est utilisé sur le  serveur, et s'il y a une programmation valide des sauvegardes automatiques
            * la programmation de sauvegarde va lancer la fonction ParamAutoBackup()
                * en recréant le fichier rufus.bup.plist sous MacOS
                * en lançant le timer t_timerbackup sous Linux
            * dans le cas contraire, la fonction EffaceProgrammationBackup() efface le fichier rufus.bup.plist sous MacOS

      Une modification de l'emplacement de sauvegarde se fait par un clic sur le bouton ui->DirBackuppushButton qui va lancer ModifDirBachup()
            * cette fonction va créer le fichier RufusScriptBackup.sh et modifier le paramètres correspondant de sauvegarde automatique dans la base de données
      Un changement de jour lance ModifDateBackup().
            * cette fonction va modifier les paramètres correspondants de sauvegarde automatique dans la base de données
            * sous MacOs, cette fonction va modifier le fichier xml rufus.bup.plist et recharger ce fichier dans le launchd
      Le bouton ui->EffacePrgSauvupPushButton réinitialise la programmation en déclenchant la fonction EffaceAutoBackup():
            * annule les données de programmation dans rufus.ini,`
            * réinitialise l'affichage dans ui->Sauvegardeframe,`
            * supprime le script de sauvegarde RufusBackupScript.sh
            * sous Mac, supprime le script de programmation rufus.bup.plist et le décharge du launchd
            * sous Linux, arrête le timer t_timerbackup

      Si le programme s'éxécute sur le serveur le QFrame ui->Sauvegardeframe est enabled, pas dans le cas contraire

      Une modification de l'heure ou du jour du backup
      lance la fonction ModifDateHeureBackup() qui va modifier les paramètres de backup en BDD
      puis lancer ParamAutoBackup()

      Une modification du dossier de destination du backup
      lance la fonction ModifDirBackup() qui va modifier les paramètres de backup en BDD
      puis lancer ParamAutoBackup()

      Le  bouton ui->ImmediatBackupupPushButton lance la fonction startImmediateBackup() qui va
        * vérifier qu'il n'y a pas d'autres postes connectés
        * créer les paramètres de la fonction ImmediateBackup() et la lancer

     La fonction ImmediateBackup() est lancée par
        * la fonction startImmediateBackup déclenchée par un click sur le bouton ui->ImmediatBackupupPushButton de dlg_param.cpp
        * une mise à jour de la BDD

     La fonction DefinitScriptBackup crée le fichier RufusScriptBackup.sh qui va éxécuter la sauvegarde.
     Elle est lancée par
        * ParamAutoBackup() sous Mac
        * Backup() utilisée pour un backup immédiat de la base (ImmediateBackup() ou backup auto sous Linux avec BackupWakeUp())

     */

public:
    QTimer                  t_timerbackup;

private:
    qint64                  m_basesize, m_imagessize, m_videossize, m_facturessize,  m_freespace;
    UpDialog                *dlg_buprestore;
    UpLabel                 *wdg_resumelbl, *wdg_volumelibrelbl;
    void                    AskBupRestore(bool restore, QString pathorigin, QString pathdestination, bool OKini = true, bool OKRessces = true, bool OKimages = true, bool OKvideos = true, bool OKfactures = true);
                            /*! crée le script RufusScriptBackup.sh qui va éxécuter la sauvegarde */
    bool                    Backup(QString pathdirdestination, bool OKBase, bool OKImages, bool OKVideos, bool OKFactures, bool isbkupauto);
                            /*! utilisée par ImmediateBackup() pour sauvegarder la base et/ou les fichiers d'imagerie suivant le choix fait dans AskBackupRestore()
                            * et par le timer t_timerbackup sous Linux pour effectuer une sauvegarde automatique et sans choix des options dans ce cas */
    void                    BackupWakeUp();
                            /*! sous Linux, déclenche le backup au moment programmé */
    qint64                  CalcBaseSize();
                            /*! calcule le volume de la base */
    void                    CalcTimeBupRestore();
                            /*! calcule la durée approximative du backup */
    void                    DefinitScriptBackup(QString pathdirdestination, bool AvecImages= true, bool AvecVideos = true);
                            /*! crée le script RufusScriptBackup.sh qui va éxécuter la sauvegarde */
    void                    DefinitScriptRestore(QStringList ListNomFiles);
                            /*! crée le script RufusScriptRestore.sh qui va éxécuter la restauration de la base MySQL et le lance */
    void                    EffaceBDDDataBackup();
                            /*! efface le paramètrage de sauvegarde (moment et emplacement) dans la base de données */
    void                    EffaceProgrammationBackup();
                            /*! efface la programmation de la sauvegarde qui a été créé sur le poste à partir du paramètrage enregistré dans la base de données
                            * n'efface pas le paramètrage de sauvegarde (moment et emplacement) dans la base de données
                            * suppression de RufusScriptBackup.sh
                            * suppression de rufus.bup.plist sous Mac et arrêt du timer t_timerbackup sous Linux
                            */
    void                    startImmediateBackup();
                            /*! paramètre la fonction ImmediateBackup() et la lance */
    bool                    ImmediateBackup(QString dirdestination = "", bool verifposteconnecte = true);
                            /*! lance un backup immédiat */
    void                    ModifHeureBackup();
                            /*! modifie l'heure du backup automatique et relance ParamAutoBackup() */
    void                    ModifDateBackup();
                            /*! modifie la date du backup automatique et relance ParamAutoBackup() */
    void                    ModifDirBackup();
                            /*! modifie le dossier de destination du backup automatique et relance ParamAutoBackup() */
    void                    ParamAutoBackup();
                            /*! paramètre le moment et l'emplacement de la sauvegarde
                             * sous Mac, crée le fichier xml rufus.bup.plist
                             * sous Linux, lance le timer t_timerbackup
                            */
    void                    RestaureBase();
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
