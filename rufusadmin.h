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

/*
 * L'application RufusAdmin sert 3 taches:
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
            Certains appareil ne permettent pas d'enregistrer un id, le programme utilise alors nom et prenom
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
 * 3. Alimenter une table temporaire dans laquelle seront intégrés chaque image correspondant au dossier patient ouvert.
      Ces tables sont alimentées à la demande avec les images de ce patient au fur et à mesure que le programme les demande
      la table est détruite quand le dossier patient est fermé
      Un timer lance une routine qui détruit les images des patients qui ne sont plus en consultation.
*/

#ifndef RUFUSADMIN_H
#define RUFUSADMIN_H

#include <QDebug>
//#include <QDomDocument>
#include <QFileDialog>
#include <QHostInfo>
#include <QMainWindow>
#include <QSplashScreen>
#include <QSystemTrayIcon>
#include <poppler-qt5.h>
#include "dlg_banque.h"
#include "dlg_gestionusers.h"
#include "dlg_gestionlieux.h"
#include "dlg_message.h"
#include "dlg_motifs.h"
#include "dlg_paramconnexion.h"
#include "importdocsexternesthread.h"
#include "widgetbuttonframe.h"
#include "utils.h"
#include "gestiontcpserver.h"
#include "gbl_datas.h"
#include "cls_item.h"
#include "database.h"
#include "cls_user.h"
#include <QTimer>

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
    dlg_banque                  *Dlg_Banq;
    dlg_gestionusers            *Dlg_GestUsr;
    dlg_motifs                  *Dlg_motifs;
    dlg_paramconnexion          *DlgParam;
    int                         idAdminDocs;
    int                         idlieuExercice;
    int                         gMode;
    int                         dureeVeille;
    enum gMode                  {Poste, ReseauLocal, Distant};
    QIcon                       gIconRufusAdmin;
    QMenu                       *trayIconMenu;
    QMap<QString,QIcon>         gmapIcons;
    QString                     gnomFichIni;
    QString                     gNomFichRufusIni;
    QString                     gNouvMDP, gAncMDP, gConfirmMDP;
    QString                     Domaine;
    QString                     gNomLieuExercice;
    QStringList                 glistAppareils;
    QIcon                       giconBackup, giconCopy, giconErase, giconSunglasses, giconSortirDossier, giconOK, giconAnnul,
                                giconEuro, giconEuroCount, giconFermeAppuye, giconFermeRelache, giconHelp, giconNull;
    QSettings                   *gsettingsIni;
    QSqlDatabase                db;
    QSystemTrayIcon             *gRufusAdminTrayIcon;
    QTimer                      *gTimerUserConnecte, *gTimerVerifDivers, *gTimerInactive, *gTimerSupprDocs, *gTimerDocsAExporter;
    ImportDocsExternesThread    *ImportDocsExtThread;
    UpDialog                    *gAskAppareil, *gAskMDP;
    WidgetButtonFrame           *widgAppareils;
    User                        *UserAdmin;

    double                      CalcBaseSize();
    int                         DetermineLieuExercice();
    bool                        eventFilter(QObject *obj, QEvent *event);
    void                        setMapDatas();
    QMap<QString, QIcon>        MapIcons();
    void                        closeEvent(QCloseEvent *);
    void                        AskAppareil();
    void                        ChoixMenuSystemTray(QString txt);
    void                        ConnexionBase();
    void                        Edit(QString txt);
    QStringList                 DecomposeScriptSQL(QString nomficscript);
    QString                     getDossierDocuments(QString Appareil);
    void                        Message(QString mess, int pause = 1000, bool bottom = true);
    void                        EffaceMessage(int pause = 1000);
    void                        NouvAppareil();
    void                        Remplir_Table();
    void                        RestoreFontAppli();
    void                        SupprAppareil();
    void                        setPosteImportDocs(bool a = true);
    void                        ReconstruitListeLieuxExercice();
    bool                        TraiteErreurRequete(QSqlQuery query, QString requete, QString ErrorMessage = "");
    bool                        VerifBase();
    bool                        VerifMDP(QString MDP, QString Msg);

    void                        ConnectTimers();
    void                        DisconnectTimers();
    void                        ConnectTimerInactive();
    void                        DisconnectTimerInactive();

private slots:
    void                        Slot_CalcExporteDocs();
    void                        Slot_ChoixButtonFrame(int);
    void                        Slot_ChoixDossierStockageApp();
    void                        Slot_ModifDirImagerie();
    void                        Slot_EnregistreAppareil();
    void                        Slot_EnregDossierStockageApp(QString dir);
    void                        Slot_EnregistreEmplacementServeur(int);
    void                        Slot_EnregistreNouvMDPAdmin();
    void                        Slot_ExporteDocs();
    void                        Slot_GestionBanques();
    void                        Slot_GestLieux();
    void                        Slot_GestUser();
    void                        Slot_MasqueAppli();
    void                        Slot_MetAJourLaConnexion();
    void                        Slot_ModifMDP();
    void                        Slot_ImportDocsExternes();
    void                        Slot_ParamMotifs();
    void                        Slot_RestaureBase();
    void                        Slot_SupprimerDocs();
    void                        Slot_TrayIconMenu();
    void                        Slot_VerifPosteImport();
    void                        Slot_VerifVersionBase();

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
    bool                    ImmediateBackup();
private slots:
    void                    Slot_CalcTimeBupRestore();
    void                    Slot_ModifDirBackup();
    void                    Slot_ModifDateBackup();
    void                    Slot_EffacePrgSauvegarde();
private:
    double                  BaseSize, ImagesSize, VideosSize, FreeSpace;
    UpDialog                *gAskBupRestore;
    UpLabel                 *labelResume, *labelVolumeLibre;
    QMap<QString, double>   dir_size(const QString DirPath);
    void                    AskBupRestore(bool restore, QString pathorigin, QString pathdestination, bool OKini = true, bool OKRessces = true, bool OKimages = true, bool OKvideos = true);
    void                    DefinitScriptBackup(QString path, bool AvecImages= true, bool AvecVideos = true);
    QString                 getExpressionSize(double size);
    void                    ModifParamBackup();

    // TCPServer, TCPSocket
    GestionTcPServer    *TcpServer;
    quint16             PortTCPServer;
    QTimer              *gTimerSalDatCorrespMsg, *gTimerVerifVerrou;
    int                 gflagMG, gflagSalDat;
    QString             gIPadr, gMacAdress;
    QString             gSocketStatut;
    QDateTime           gDateDernierMessage;
    int                 GetflagMG();
    int                 GetflagSalDat();
    void                FermeTCP();
    void                ResumeStatut();

    void                VerifModifsSalledAttenteCorrespondantsetNouveauxMessages();
    void                MAJTcpMsgEtFlagSalDat();
    void                MAJflagMG();
    void                VerifVerrouDossier();
signals:
    void                ModifEdit(QString txt);

};

#endif // RUFUSADMIN_H
