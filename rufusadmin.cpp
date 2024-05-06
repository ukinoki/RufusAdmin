/* (C) 2020 LAINE SERGE
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

#include "rufusadmin.h"
#include "ui_rufusadmin.h"
#include <QDebug>

RufusAdmin::RufusAdmin(QWidget *parent) : QMainWindow(parent), ui(new Ui::RufusAdmin)
{
    //! la version du programme correspond à la date de publication, suivie de "/" puis d'un sous-n° - p.e. "23-6-2017/3"
    qApp->setApplicationName("RufusAdmin");
    qApp->setApplicationVersion("05-05-2024/1");       // doit impérativement être composé de date version / n°version);

    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    QString border = "border-image: url(://wallpaper.jpg)";
    qApp->setStyleSheet(
        "QDialog{" + border + "}"
        "QGroupBox{font:bold;}"
        "QLineEdit {background-color:white; border: 1px solid rgb(150,150,150);border-radius: 5px;}"
        "QLineEdit:focus {border: 1px solid rgb(164, 205, 255);border-radius: 5px;}"
        "QRadioButton::indicator {width: 18px; height: 18px;}"
        "QRadioButton::indicator::checked {image: url(://blueball.png);}"
        "QScrollArea {background-color:rgba(237, 237, 237); border: 1px solid rgb(150,150,150);}"
        "QTabBar::tab:selected {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #fafafa, stop: 1.0 rgb(164, 205, 255));}"
        "QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #fafafa, stop: 0.4 #f4f4f4,stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);}"
        "QTabBar::tab:selected {border-color: #9B9B9B; border-bottom-color: #C2C7CB;}"
        "QTabBar::tab:!selected {margin-top: 4px;}"
        "QTabBar::tab {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
        "border: 1px solid #C4C4C3;"
        "border-bottom-color: #C2C7CB;"
        "border-top-left-radius: 4px; border-top-right-radius: 4px;"
        "min-width: 8ex;"
        "padding: 2px;}"
        "QTabWidget::pane {border-top: 1px solid #C2C7CB;}"
        "QTabWidget::tab-bar {left: 20px;}"
        "QTextEdit {background-color:white; border: 1px solid rgb(150,150,150); border-radius: 10px;}"
        "QTextEdit:focus {border: 2px solid rgb(164, 205, 255);border-radius: 10px;}"
        "QTreeWidget{background-color: white;}"
        "QToolBar {border-radius: 5px; margin-left: 5px; margin-right: 5px;  margin-top: 3px; margin-bottom: 3px;}"
        "QWidget#centralWidget{" + border + "}");

    ic_Backup         .addFile(QStringLiteral("://Backup.png"),               QSize(),        QIcon::Normal, QIcon::Off);
    ic_Copy           .addFile(QStringLiteral("://copy.png"),                 QSize(),        QIcon::Normal, QIcon::Off);
    ic_Sunglasses     .addFile(QStringLiteral("://Sunglasses.png"),           QSize(),        QIcon::Normal, QIcon::Off);
    ic_SortirDossier  .addFile(QStringLiteral("://metadoc.png"),              QSize(25,25),   QIcon::Normal, QIcon::Off);
    ic_Annul          .addFile(QStringLiteral("://Supprime.png"),             QSize(30,30),   QIcon::Normal, QIcon::Off);
    ic_Euro           .addFile(QStringLiteral("://euro.ico"),                 QSize(40,40),   QIcon::Normal, QIcon::Off);
    ic_EuroCount      .addFile(QStringLiteral("://EuroCount.png"),            QSize(40,40),   QIcon::Normal, QIcon::Off);
    ic_FermeAppuye    .addFile(QStringLiteral("://shutdowndown.png"),         QSize(30,30),   QIcon::Normal, QIcon::Off);
    ic_FermeRelache   .addFile(QStringLiteral("://shutdown.png"),             QSize(30,30),   QIcon::Normal, QIcon::Off);
    ic_Null           .addFile(QStringLiteral(""),                            QSize(30,30),   QIcon::Normal, QIcon::Off);
    ic_OK             .addFile(QStringLiteral("://start.png"),                QSize(),        QIcon::Normal, QIcon::Off);
    ic_RufusAdmin     .addFile(QStringLiteral("://wrench-screwdriver.png"),   QSize(),        QIcon::Normal, QIcon::Off);
    ic_Erase          .addFile(QStringLiteral("://eraser.png"),               QSize(),        QIcon::Normal, QIcon::Off);

    ui->EffacePrgSauvupPushButton   ->setIcon(ic_Erase);
    ui->EffacePrgSauvupPushButton   ->setIconSize(QSize(35,35));
    ui->RestaurBaseupPushButton     ->setIcon(ic_Copy);
    ui->ImmediatBackupupPushButton  ->setIcon(ic_Backup);

    setWindowIcon(ic_Sunglasses);

    ui->FermepushButton->setUpButtonStyle(UpSmallButton::CLOSEBUTTON);
    m_settings              = new QSettings(PATH_FILE_INI, QSettings::IniFormat);
    m_nouvMDP               = "nouv";
    m_ancMDP                = "anc";
    m_confirmMDP            = "confirm";
    Utils::mkpath(PATH_DIR_RUFUS);
    RestoreFontAppli(); // les polices doivent être appliquées après la définition des styles
    setMapIcons();

    ConnexionBase();
    m_parametres            = db->parametres();
    m_dirimagerie           = db->dirimagerie();
    if (!VerifBase())
        exit(0);
    Datas::I()->users       ->initListe();

    DataBase::I()->setidUserConnected(Admin()->id());
    RecalcCurrentDateTime();

    /*! On ne peut pas utiliser Utils::VerifMDP(Admin()->password(),tr("Saisissez le mot de passe Administrateur")))
     * parce qu'à ce stade du code, Admin()->password() est encore égal à la valeur SHA1 du mdp admin
     * et qu'il faut récupérer la valeur non cryptée du mdp
     */
    //db->setmdpadmin(Utils::calcSHA1("bob"));
    QString mdp ("");
    QString dbmdp = db->getMDPAdmin() ;
    if (Utils::VerifMDP(dbmdp,tr("Saisissez le mot de passe Administrateur"),mdp))
    {
        if (dbmdp == mdp)
            db->setmdpadmin(Utils::calcSHA1(mdp));
    }
    else
        exit(0);


    // on vérifie que le programme n'est pas déjà en cours d'éxécution sur un autre poste
    QString reqp = "select " CP_NOMPOSTE_USRCONNECT " from " TBL_USERSCONNECTES
                   " where " CP_IDUSER_USRCONNECT " = " + QString::number(Admin()->id()) +
                   " and " CP_NOMPOSTE_USRCONNECT " != '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEUR "'"
                   " and " CP_IDLIEU_USRCONNECT " = " + QString::number(Datas::I()->sites->idcurrentsite()) +
                   " and time_to_sec(timediff(now(),heurederniereconnexion)) < 60";
    QList<QVariantList> listusr2 = db->StandardSelectSQL(reqp, m_ok);
    if (listusr2.size()>0)
    {
        UpMessageBox::Watch(this, tr("Programme déjà en cours d'éxécution sur le poste ") + listusr2.at(0).at(0).toString().remove(" - " NOM_ADMINISTRATEUR), tr("Sortie du programme"));
        exit(0);
    }
    else
        db->StandardSQL("delete from " TBL_USERSCONNECTES " where " CP_IDUSER_USRCONNECT " = " + QString::number(Admin()->id()) + " and " CP_IDLIEU_USRCONNECT " = " + QString::number(Datas::I()->sites->idcurrentsite()));

    Datas::I()->sites->initListe();
    DetermineLieuExercice();
    flags           = Flags::I();
    m_IPadress      = Utils::IPAdress();
    m_macAdress     = Utils::MACAdress();
    m_utiliseTCP    = (m_IPadress!=""); /*! quand le poste n'est connecté à aucun réseau local, il n'a pas d'IP locale => on désactive le TCPServer
                                         * sinon, il faut initialiser le TCP avant de lancer la fonction MetAJourLaConnexion()
                                         * parce que si un poste a été déconnecté accidentellement RufusAdmin plante en éliminant ce poste de la liste si le TCP n'a pas été initialisé */
    if (m_utiliseTCP)
        TCPServer           = TcpServer::I();
    MetAJourLaConnexion();
    ui->AppareilsconnectesupLabel->setText(tr("Appareils connectés au réseau") + " <font color=\"green\"><b>" + Datas::I()->sites->currentsite()->nom() + "</b></font> ");

    //recherche de l'idUser du compte AdminDocs
    QString req = "select " CP_ID_USR " from " TBL_UTILISATEURS " where " CP_LOGIN_USR " = '" NOM_ADMINISTRATEUR "'";
    QList<QVariantList> listusr = db->StandardSelectSQL(req, m_ok);
    if (listusr.size()==0)
    {
        req = "select " CP_ID_USR " from " TBL_UTILISATEURS " where " CP_NOM_USR " = '" NOM_ADMINISTRATEUR "'";
        QList<QVariantList> listusers = db->StandardSelectSQL(req, m_ok);
        if (listusers.size()>0)
            db->StandardSQL("update " TBL_UTILISATEURS " set " CP_LOGIN_USR " = '" NOM_ADMINISTRATEUR "' where " CP_NOM_USR " = '" NOM_ADMINISTRATEUR "'");
        else
            db->StandardSQL("insert into " TBL_UTILISATEURS " (" CP_LOGIN_USR ", " CP_NOM_USR ", " CP_MDP_USR ") values ('" NOM_ADMINISTRATEUR "','" NOM_ADMINISTRATEUR ", '" MDP_ADMINISTRATEUR "')");
    }

    // 5 mettre en place le TcpSocket
    if (m_utiliseTCP)
    {
        connect(TCPServer,  &TcpServer::ModifListeSockets,      this,   &RufusAdmin::ResumeTCPSocketStatut);
        connect(TCPServer,  &TcpServer::deconnexionposte,       this,   &RufusAdmin::DeconnexionPoste);         // élimine les instances du poste déconnecté dans la liste des postes et dans la bdd
        TCPServer           ->start();
    }
    flags->remiseAZeroFlags();
    m_flagcorrespondants    = flags->flagCorrespondants();
    m_flagmessages          = flags->flagMessages();
    m_flagsalledattente     = flags->flagSalleDAttente();

    t_timerSalDatCorrespMsg = new QTimer(this);     /* scrutation des modifs de la salle d'attente et des correspondants et de l'arrivée de nouveaux messages utilisé par
                                                           pour verifier les modifications faites par les postes distants
                                                        */
    t_timerUserConnecte     = new QTimer(this);     // mise à jour de la connexion à la base de données
    t_timerSupprDocs        = new QTimer(this);     // utilisé par le poste importateur pour vérifier s'il y a des documents à supprimer
    t_timerVerifDivers      = new QTimer(this);     // vérification du poste importateur des documents et de la version de la base
    t_timerSupprDocs        = new QTimer(this);     // verification des documents à supprimer
    t_timerProgressBar      = new QTimer(this);     // progression de la progressbar - quand la progressbar est au maximum, la fiche est cachée

    setPosteImportDocs(); // on prend la place d'importateur des documents dans les utilisateurs connectés
    VerifPosteImport();
    VerifVersionBase();
    CalcExporteDocs();
    QList<QVariantList> listdate = db->StandardSelectSQL("select max(" CP_DATECREATION_MSG ") from " TBL_MESSAGES, m_ok);
    if (listdate.size()==0)
        m_dateDernierMessage = db->ServerDateTime();
    else
        m_dateDernierMessage = listdate.at(0).at(0).toDateTime();

    t_timerSalDatCorrespMsg  ->start(1000);
    t_timerUserConnecte      ->start(10000);
    t_timerVerifDivers       ->start(30000);

    m_dureeVeille = 30000;
    ui->MiseEnVeilleprogressBar->setMinimum(0);
    ui->MiseEnVeilleprogressBar->setMaximum(m_dureeVeille);
    ui->MiseEnVeilleprogressBar->setInvertedAppearance(true);
    connect(t_timerProgressBar, &QTimer::timeout, this, [=] {ui->MiseEnVeilleprogressBar->setValue(ui->MiseEnVeilleprogressBar->value()-1);});
    t_timerProgressBar      ->start(1);
    t_timerSupprDocs        ->start(60000);     // "toutes les 60 secondes"
    t_timerDocsAExporter    = new QTimer(this);
    t_timerDocsAExporter    ->start(60000);     // "toutes les 60 secondes"
    ConnectTimers();

    connect(ui->ExportImagespushButton,         &QPushButton::clicked,              this,   &RufusAdmin::ExporteDocs);
    connect(ui->FermepushButton,                &QPushButton::clicked,              this,   &RufusAdmin::MasqueAppli);
    connect(ui->GestionBanquespushButton,       &QPushButton::clicked,              this,   &RufusAdmin::GestionBanques);
    connect(ui->GestLieuxpushButton,            &QPushButton::clicked,              this,   &RufusAdmin::GestionLieux);
    connect(ui->GestionMotifspushButton,        &QPushButton::clicked,              this,   &RufusAdmin::GestionMotifs);
    connect(ui->GestUserpushButton,             &QPushButton::clicked,              this,   &RufusAdmin::GestionUsers);
    connect(ui->InitMDPAdminpushButton,         &QPushButton::clicked,              this,   &RufusAdmin::ModifMDP);
    connect(ui->NetworkStatuspushButton,        &QPushButton::clicked,              this,   [=] {Edit(m_socketStatut, 20000);});
    // MAJ Salle d'attente ----------------------------------------------------------------------------------
    connect(flags,                              &Flags::UpdSalleDAttente,           this,   [=](int a)  { m_flagsalledattente = a; } );
    // MAJ Correspondants ----------------------------------------------------------------------------------
    connect(flags,                              &Flags::UpdCorrespondants,          this,   [=](int a)  { m_flagcorrespondants = a; } );
    // MAJ messages ----------------------------------------------------------------------------------
    connect(flags,                              &Flags::UpdMessages,                this,   [=](int a)  { m_flagmessages = a; } );



    wdg_buttonframe = new WidgetButtonFrame(ui->AppareilsConnectesupTableWidget);
    wdg_buttonframe->AddButtons(WidgetButtonFrame::Plus | WidgetButtonFrame::Moins);
    connect(wdg_buttonframe, &WidgetButtonFrame::choix, this, &RufusAdmin::ChoixButtonFrame);

    // Mise en forme de la table AppareilsConnectes
    ui->AppareilsConnectesupTableWidget->setPalette(QPalette(Qt::white));
    ui->AppareilsConnectesupTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->AppareilsConnectesupTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->AppareilsConnectesupTableWidget->verticalHeader()->setVisible(false);
    ui->AppareilsConnectesupTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->AppareilsConnectesupTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->AppareilsConnectesupTableWidget->setColumnCount(6);
    ui->AppareilsConnectesupTableWidget->setColumnWidth(0,0);       // idppareil
    ui->AppareilsConnectesupTableWidget->setColumnWidth(1,160);     // TitreExamen
    ui->AppareilsConnectesupTableWidget->setColumnWidth(2,220);     // NomAppareil
    ui->AppareilsConnectesupTableWidget->setColumnWidth(3,0);       // Format
    ui->AppareilsConnectesupTableWidget->setColumnWidth(4,462);     // Dossier d'échange
    ui->AppareilsConnectesupTableWidget->setColumnWidth(5,30);      // Bouton dossier
    ui->AppareilsConnectesupTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("id"));
    ui->AppareilsConnectesupTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Type d'examen")));
    ui->AppareilsConnectesupTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Nom de l'appareil")));
    ui->AppareilsConnectesupTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Format nom fichier")));
    ui->AppareilsConnectesupTableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Dossier d'échange")));
    ui->AppareilsConnectesupTableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(""));
    ui->AppareilsConnectesupTableWidget->horizontalHeader()->setVisible(true);
    for (int i=0; i<ui->AppareilsConnectesupTableWidget->columnCount(); i++)
        ui->AppareilsConnectesupTableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);
    ui->AppareilsConnectesupTableWidget->FixLargeurTotale();
    wdg_buttonframe->widgButtonParent()->setFixedWidth(ui->AppareilsConnectesupTableWidget->width());
    QVBoxLayout *applay = new QVBoxLayout();
    applay      ->addWidget(ui->MiseEnVeilleprogressBar);
    QHBoxLayout *Stocklay = new QHBoxLayout();
    Stocklay    ->addWidget(ui->StockageupLabel);
    Stocklay    ->addWidget(ui->StockageupLineEdit);
    Stocklay    ->addSpacerItem(new QSpacerItem(5,15,QSizePolicy::Expanding, QSizePolicy::Expanding));
    QHBoxLayout *appcentrelay = new QHBoxLayout();
    appcentrelay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Expanding));
    appcentrelay->addWidget(ui->AppareilsconnectesupLabel);
    appcentrelay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Expanding));
    applay      ->addLayout(appcentrelay);
    applay      ->addWidget(wdg_buttonframe->widgButtonParent());
    applay      ->addLayout(Stocklay);

    int marge   = 5;
    applay      ->setContentsMargins(marge,marge,marge,marge);
    applay      ->setSpacing(marge);
    ui->Appareilsconnectesframe ->setFixedWidth(wdg_buttonframe->widgButtonParent()->width() + marge + marge + marge);
    ui->Appareilsconnectesframe ->setLayout(applay);
    ui->Sauvegardeframe         ->setFixedWidth(wdg_buttonframe->widgButtonParent()->width() + marge + marge + marge);
    setFixedWidth(ui->Appareilsconnectesframe->width() + 20);
    Remplir_Table();
    if (db->ModeAccesDataBase() == Utils::Poste)
        setWindowTitle("RufusAdmin - " + Datas::I()->sites->currentsite()->nom());

    ui->Exportframe         ->setVisible(db->ModeAccesDataBase() != Utils::Distant);

    connect (ui->StockageupLineEdit, &QLineEdit::textChanged, this, [=](QString s) {ui->StockageupLineEdit->setImmediateToolTip(s);});
    ui->StockageupLineEdit->setText(db->dirimagerie());

    ReconstruitListeLieuxExercice();
    Datas::I()->comptes->initListe();

    if (m_parametres->versionbase()==0)
        ui->VersionBaselabel->setText(tr("Version de la base") + "\t<font color=\"red\"><b>" + tr("inconnue") + "</b></font>");
    else
        ui->VersionBaselabel->setText(tr("Version de la base ") + "<font color=\"green\"><b>" + QString::number(m_parametres->versionbase()) + "</b></font>");
    ui->VersionRufuslabel->setText(tr("Version de RufusAdmin ") + "<font color=\"green\"><b>" + qApp->applicationVersion() + "</b></font>");

    ui->Sauvegardeframe         ->setEnabled(db->ModeAccesDataBase() == Utils::Poste);
    ui->ParamSauvegardelabel    ->setVisible(db->ModeAccesDataBase() != Utils::Poste);
    ui->ParamSauvegardelabel    ->setEnabled(db->ModeAccesDataBase() != Utils::Poste);

    if (db->ModeAccesDataBase() == Utils::Poste)
    {
        connect (ui->DirBackupuplineEdit, &QLineEdit::textChanged, this, [=](QString s) {ui->DirBackupuplineEdit->setImmediateToolTip(s);});
        if (QDir(m_parametres->dirbkup()).exists())
            ui->DirBackupuplineEdit->setText(m_parametres->dirbkup());
        if (m_parametres->heurebkup().isValid())
            ui->HeureBackuptimeEdit->setTime(m_parametres->heurebkup());
        ui->LundiradioButton    ->setChecked(m_parametres->daysbkup().testFlag(Utils::Lundi));
        ui->MardiradioButton    ->setChecked(m_parametres->daysbkup().testFlag(Utils::Mardi));
        ui->MercrediradioButton ->setChecked(m_parametres->daysbkup().testFlag(Utils::Mercredi));
        ui->JeudiradioButton    ->setChecked(m_parametres->daysbkup().testFlag(Utils::Jeudi));
        ui->VendrediradioButton ->setChecked(m_parametres->daysbkup().testFlag(Utils::Vendredi));
        ui->SamediradioButton   ->setChecked(m_parametres->daysbkup().testFlag(Utils::Samedi));
        ui->DimancheradioButton ->setChecked(m_parametres->daysbkup().testFlag(Utils::Dimanche));
        connect(ui->DirBackuppushButton,        &QPushButton::clicked,      this,   &RufusAdmin::ModifDirBackup);
        foreach(QRadioButton *butt, ui->JourSauvegardeframe->findChildren<QRadioButton*>())
            connect(butt,                       &QPushButton::clicked,      this,   &RufusAdmin::ModifDateBackup);
        connect(ui->HeureBackuptimeEdit,        &QTimeEdit::timeChanged,    this,   &RufusAdmin::ModifHeureBackup);
        connect(ui->EffacePrgSauvupPushButton,  &QPushButton::clicked,      this,   &RufusAdmin::EffaceBDDDataBackup);
        connect(ui->ImmediatBackupupPushButton, &QPushButton::clicked,      this,   &RufusAdmin::startImmediateBackup);
        connect(ui->RestaurBaseupPushButton,    &QPushButton::clicked,      this,   &RufusAdmin::RestaureBase);
        ui->EffacePrgSauvupPushButton->setEnabled(m_parametres->daysbkup()
                                               && QDir(m_parametres->dirbkup()).exists()
                                               && m_parametres->dirbkup() != ""
                                               && m_parametres->heurebkup() != QTime());
    }

    trayIconMenu = new QMenu();
    ictray_RufusAdminTrayIcon = new QSystemTrayIcon();
    ictray_RufusAdminTrayIcon->setContextMenu(trayIconMenu);
    ictray_RufusAdminTrayIcon->setIcon(ic_RufusAdmin);
    ictray_RufusAdminTrayIcon->setVisible(true);
    connect(trayIconMenu,   &QMenu::aboutToShow,    this,   &RufusAdmin::TrayIconMenu);

    m_importdocsexternesthread = new ImportDocsExternesThread();
    connect(m_importdocsexternesthread,        QOverload<QStringList, int>::of(&ImportDocsExternesThread::emitmsg),
                                                this,       &RufusAdmin::AfficheMessageImport);
    if (m_utiliseTCP)
        connect(m_importdocsexternesthread,    QOverload<QString>::of(&ImportDocsExternesThread::emitmsg),
                TCPServer,  [=] (QString msg)
        {
            //qDebug()<< msg;
            EnvoieTCPMessage(TOUS, msg);
        });
    connect(&t_timer,                   &QTimer::timeout,   this,       &RufusAdmin::ReconstruireListeAppareils);
    t_timer             .setInterval(5000);
    t_timer.start();

    Datas::I()->banques->initListe();
    Datas::I()->motifs->initListe();

    //!-------------------- GESTION DES COTATIONS FRANCE-------------------------------------------------------*/
    ui->CotationsFrancecheckBox->setChecked(db->parametres()->cotationsfrance());
    connect (ui->CotationsFrancecheckBox, &QCheckBox::stateChanged, this, [=](int state){db->setcotationsfrance(state == Qt::Checked);});
    //!-------------------- GESTION DES COTATIONS FRANCE-------------------------------------------------------*/

    //!-------------------- GESTION DES VILLES ET DES CODES POSTAUX-------------------------------------------------------*/
    enum Villes::TownsFrom from;
    if (m_parametres->villesfrance())
        from = Villes::DATABASE;
    else
        from = Villes::CUSTOM;
    Datas::I()->villes  ->initListe(from);
    ui->UtiliseBDDVillescheckBox     ->setChecked(db->parametres()->villesfrance() == true);
    ui->UtiliseCustomVillescheckBox  ->setChecked(db->parametres()->villesfrance() == false);
    ui->ModifListVillesupPushButton  ->setVisible(db->parametres()->villesfrance() == false);
    connect(ui->UtiliseCustomVillescheckBox, &QCheckBox::stateChanged, this, [=](int state){
        ui->ModifListVillesupPushButton->setVisible(state == Qt::Checked);
        enum Villes::TownsFrom from;
        if (state == Qt::Checked)
            from = Villes::CUSTOM;
        else
            from = Villes::DATABASE;
        ModifBDDVilles(from);
    });
    connect(ui->ModifListVillesupPushButton,    &QPushButton::clicked, this, [=]{
        dlg_listevilles *dlg_listvilles = new dlg_listevilles(this);
        dlg_listvilles->exec();
        delete dlg_listvilles;
    });


    //! - mise à jour du programmateur de sauvegarde
    if (db->ModeAccesDataBase() == Utils::Poste)
        ParamAutoBackup();
    /*! la suite sert à décharger le launchagent du programme de backup sous MacOs, plus utilisé depuis Catalina */
#ifdef Q_OS_MACOS
    if (QFile::exists(PATH_FILE_SCRIPT_MACOS_PLIST))
    {
        QFile::remove(PATH_FILE_SCRIPT_MACOS_PLIST);
        // décharge du launchd
        QString unload  = "bash -c \"/bin/launchctl unload \"" PATH_FILE_SCRIPT_MACOS_PLIST "\"\"";
        QProcess dumpProcess(this);
        dumpProcess.start(unload);
        dumpProcess.waitForFinished();
    }
#endif

    //! - mise à jour du programmateur de l'effacement des fichiers images provisoires - abandonné parce qu'il continue à fonctionner même en cas de plantage du programme
    //if (db->ModeAccesDataBase() == Utils::Poste)
        //ProgrammeSQLVideImagesTemp(m_parametres->heurebkup());
    db->StandardSQL("Use " DB_IMAGES);
    db->StandardSQL("DROP EVENT IF EXISTS VideImagesEchange");
    db->StandardSQL("Use " DB_COMPTA);
    db->StandardSQL("DROP EVENT IF EXISTS VideFactures");


    installEventFilter(this);
    EpureLogs();
}

RufusAdmin::~RufusAdmin()
{
    delete ui;
}

void RufusAdmin::closeEvent(QCloseEvent *event)
{
    QString mdp ("");
    if (!Utils::VerifMDP(db->getMDPAdmin(),tr("Saisissez le mot de passe Administrateur"), mdp))
    {
        event->ignore();
        return;
    }
    SortieAppli();
}

int RufusAdmin::SortieAppli()
{
    // on retire le poste de la variable posteimportdocs SQL
    setPosteImportDocs(false);
    // on retire Admin de la table des utilisateurs connectés
    QString req = "delete from " TBL_USERSCONNECTES
                  " where " CP_MACADRESS_USRCONNECT " = '" + Utils::MACAdress() + " - " NOM_ADMINISTRATEUR  "'"
                  " and " CP_IDLIEU_USRCONNECT " = " + QString::number(Datas::I()->sites->idcurrentsite());
    db->StandardSQL(req);
    setPosteImportDocs(false);
    if (m_utiliseTCP && TCPServer != Q_NULLPTR)
    {
        TCPServer->close();
        delete TCPServer;
    }
    exit(0);
}

void RufusAdmin::AfficheMessageImport(QStringList listmsg, int pause)
{
    ShowMessage::I()->SplashMessage(listmsg, pause);
}

bool RufusAdmin::AutresPostesConnectes()
{
    /*! avant la mise à jour 61, on ne peut pas utiliser Datas::I()->users->initListe() parce que le champ DateCreationMDP de la table utilisateurs n'existe pas */
    if (Datas::I()->users->all()->isEmpty())
        Datas::I()->users       ->initListe();
    Datas::I()->postesconnectes->initListe();
    QString id = Utils::MACAdress() + " - " + QString::number(Admin()->id());
    for (auto it = Datas::I()->postesconnectes->postesconnectes()->constBegin(); it != Datas::I()->postesconnectes->postesconnectes()->constEnd(); ++it)
    {
        PosteConnecte *post = const_cast<PosteConnecte*>(it.value());
        if (post->stringid() != id)
        {
            UpMessageBox::Information(this, tr("Autres postes connectés!"),
                                      tr("Vous ne pouvez pas effectuer d'opération de sauvegarde/restauration sur la base de données"
                                         " si vous n'êtes pas le seul poste connecté.\n"
                                         "Le poste ") + post->nomposte() + " " + tr("est aussi connecté"));
            show();
            return true;
        }
    }
    return false;
}

void RufusAdmin::DeconnexionPoste(QString stringid)
{
    //qDebug() << postestringid;
    PosteConnecte *post = Datas::I()->postesconnectes->getByStringId(stringid);
    if (post != Q_NULLPTR)
    {
        QString nomposte = post->nomposte();
        int iduserposte = post->id();
            //!> suppression du poste de la liste des postes connectés
        Datas::I()->postesconnectes->SupprimePosteConnecte(post);
        bool mettreajourlasalledattente = false;
            //!> remise en salle d'attente des patients en cours d'examen sur ce poste
        for (auto it = Datas::I()->patientsencours->patientsencours()->constBegin(); it != Datas::I()->patientsencours->patientsencours()->constEnd(); ++it)
        {
            PatientEnCours *pat = const_cast<PatientEnCours*>(it.value());
            if (pat != Q_NULLPTR)
                if (pat->iduserencoursexam() == iduserposte && pat->statut().contains(ENCOURSEXAMEN) && pat->posteexamen() == nomposte)
                {
                    ItemsList::update(pat, CP_STATUT_SALDAT, ARRIVE);
                    ItemsList::update(pat, CP_POSTEEXAMEN_SALDAT);
                    ItemsList::update(pat, CP_IDUSERENCOURSEXAM_SALDAT);
                    mettreajourlasalledattente = true;
                }
            if (mettreajourlasalledattente)
            {
                flags->MAJFlagSalleDAttente();
                EnvoieTCPMessage(TOUS, TCPMSG_MAJSalAttente);
            }
        }
        //!> on déverrouille les actes verrouillés en comptabilité par cet utilisateur s'il n'est plus connecté sur aucun poste
        bool usernotconnectedever = true;
        for (auto it = Datas::I()->postesconnectes->postesconnectes()->constBegin(); it != Datas::I()->postesconnectes->postesconnectes()->constEnd(); ++it)
        {
            PosteConnecte *post = const_cast<PosteConnecte*>(it.value());
            if(post->id() == iduserposte)
            {
                usernotconnectedever = false;
                break;
            }
        }
        if (usernotconnectedever)
            db->StandardSQL("delete from " TBL_VERROUCOMPTAACTES " where " CP_POSEPAR_VERROUCOMPTA " = " + QString::number(iduserposte));
    }
}

void RufusAdmin::ReconstruireListeAppareils()
{
    QList<AppareilImagerie*> listappareils = QList<AppareilImagerie*>();
    bool usetimer = true;  /*! Il semble que la classe QSystemFileWatcher pose quelques problèmes.
                             * au démarrage du système le signal directorychanged ne marche pas bien sur Mac quand le fichier d'échange est sur une machine Linux ou Windows
                             * il faut redémarrer une Rufus pour que ça se décide à marcher
                             * On peut utiliser un timer à la place. C'est nettement moins élégant mais ça marche très bien.
                             * Il suffit de mettre ce bool à true pour utiliser le timer
                             * Le code pour le QFileSystemWatcher a été conservé au cas où le problème serait résolu */
    if (!usetimer)
        disconnect(&m_filewatcher, nullptr, nullptr, nullptr);
    else
    {
        t_timerfilewatcher.stop();
        t_timerfilewatcher.start(2500);
        disconnect(&t_timerfilewatcher, nullptr, nullptr, nullptr);
    }


    QString req =   "select distinct list." CP_TITREEXAMEN_APPAREIL ", list." CP_NOMAPPAREIL_APPAREIL " from " TBL_APPAREILSCONNECTESCENTRE " appcon, " TBL_LISTEAPPAREILS " list"
                    " where list." CP_ID_APPAREIL " = appcon." CP_IDAPPAREIL_APPAREILS " and " CP_IDLIEU_APPAREILS " = " + QString::number(Datas::I()->sites->idcurrentsite());
    //qDebug()<< req;
    QList<QVariantList> listdocs = db->StandardSelectSQL(req, m_ok);
    if (m_ok && listdocs.size()>0)
    {
        for (int itr=0; itr<listdocs.size(); itr++)
        {
            if (!usetimer)
            {
                QStringList listpaths;
                for (int f = 0; f<m_filewatcher.directories().size(); f++)
                    listpaths << m_filewatcher.directories().at(f);
                if (listpaths.size()>0)
                    m_filewatcher.removePaths(listpaths);
            }
            QString nomappareil =  listdocs.at(itr).at(1).toString();
            QString nomdossier = m_settings->value("DossierEchangeImagerie/" + nomappareil).toString();  // le dossier où sont exportés les documents d'un appareil donné
            if (nomdossier != "")
                if (QDir(nomdossier).exists())
                {
                    QString titreexamen = listdocs.at(itr).at(0).toString();
                    AppareilImagerie *appareil = new AppareilImagerie(titreexamen, nomappareil, nomdossier);
                    if (!usetimer)
                        m_filewatcher.addPath(nomdossier);
                    ImportNouveauDocExterne(appareil);
                    listappareils << appareil;
                }
        }
    }
    // Surveillance du dossier d'imagerie ----------------------------------------------------------------------------------
    setlisteappareils(listappareils);
    if (listeappareils().size() > 0)
    {
        if (!usetimer)
            connect(&m_filewatcher,     &QFileSystemWatcher::directoryChanged,  this,   [=](QString nomdossier)
            {
                for (int i=0; i<listeappareils().size(); i++)
                    if (listeappareils().at(i)->nomdossierechange() == nomdossier)
                        ImportNouveauDocExterne(listeappareils().at(i));
            } );
        else
            connect (&t_timerfilewatcher,   &QTimer::timeout,   this, &RufusAdmin::VerifDocsDossiersEchanges);
    }
}

void RufusAdmin::VerifDocsDossiersEchanges()
{
    for (int itr=0; itr<listeappareils().size(); itr++)
    {
        AppareilImagerie *appareil =  listeappareils().at(itr);
        QString nomdossier = appareil->nomdossierechange();  // le dossier où sont exportés les documents d'un appareil donné
        if (nomdossier != "")
            if (QDir(nomdossier).exists())
            {
                QStringList filters, listnomsfiles;
                filters << "*.pdf" << "*.jpg";
                listnomsfiles = QDir(nomdossier).entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
                if (listnomsfiles.size() > 0)
                    for (int it=0; it<listnomsfiles.size(); it++)
                    {
                        QString nomfiledoc = listnomsfiles.at(it);
                        if (!nomfiledoc.contains("smbtest"))
                            m_importdocsexternesthread->RapatrieDocumentsThread(appareil, nomfiledoc);
                    }
            }
    }
}

void RufusAdmin::AskAppareil()
{
    DisconnectTimerInactive();
    dlg_askAppareil = new UpDialog(this);
    dlg_askAppareil->setWindowModality(Qt::WindowModal);
    dlg_askAppareil->setFixedSize(400,100);
    QHBoxLayout *lay = new QHBoxLayout;
    UpLabel *label = new UpLabel();
    label->setText("Nom de l'appareil");
    label->setFixedSize(120,25);
    lay->addWidget(label);
    lay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
    UpComboBox *upCombo = new UpComboBox();
    upCombo->insertItems(0,m_listeAppareilsNonConnectes);
    upCombo->setFixedSize(260,32);
    upCombo->setchamp("NomAppareil");
    lay->addWidget(upCombo);
    dlg_askAppareil->dlglayout()->insertLayout(0,lay);
    dlg_askAppareil->AjouteLayButtons(UpDialog::ButtonCancel | UpDialog::ButtonOK);
    connect(dlg_askAppareil->OKButton,    &QPushButton::clicked, this, &RufusAdmin::EnregistreAppareil);
    dlg_askAppareil->exec();
    upCombo->showPopup();
    ConnectTimerInactive();
}

/*!
 *  \brief AskBupRestore
 *  la fiche qui permet de paramètrer une opération de sauvegarde ou de restauration
 *  \param restore :            true = restauration - false = backup
 *  \param pathorigin :         le dossier de stockage de l'imagerie sur le serveur
 *  \param pathdestination :    le dossier où se trouve la backup
 *  \param OKini :              le rufus.ini est sauvegardé
 *  \param OKimages :           les fichiers images sont sauvegardés
 *  \param OKvideos :           les fichiers videos sont sauvegardés
 *
 */
void RufusAdmin::AskBupRestore(BkupRestore op, QString pathorigin, QString pathdestination, bool OKini, bool OKimages, bool OKvideos, bool OKfactures)
{
    QMap<QString,qint64>      DataDir;
    // taille de la base de données ----------------------------------------------------------------------------------------------------------------------------------------------
    m_basesize = 0;
    if (op == RestoreOp)
    {
        QStringList filters, listnomsfilestorestore;
        filters << "*.sql";
        for (int j=0; j<QDir(pathorigin).entryList(filters).size(); j++)
            listnomsfilestorestore << pathorigin + "/" + QDir(pathorigin).entryList(filters).at(j);
        for (int i=0; i<listnomsfilestorestore.size(); i++)
            m_basesize += QFile(listnomsfilestorestore.at(i)).size()/1024/1024;
    }
    else
        m_basesize = CalcBaseSize();
    m_imagessize = 0;
    m_videossize = 0;
    m_facturessize = 0;
    // espace libre sur le disque ------------------------------------------------------------------------------------------------------------------------------------------------
    m_freespace = QStorageInfo(pathdestination).bytesAvailable();
    m_freespace = m_freespace/1024/1024;
    //qDebug() << QStorageInfo(ui->DirBackupuplineEdit->text()).bytesAvailable();
    //qDebug() << QString::number(FreeSpace,'f',0);

    dlg_buprestore = new UpDialog(this);
    dlg_buprestore->setModal(true);
    dlg_buprestore->move(QPoint(x()+width()/2,y()+height()/2));
    dlg_buprestore->setWindowTitle(op == RestoreOp? tr("Dossiers à restaurer") : tr("Dossiers à sauvegarder"));
    int labelsize = 15;

    if (op == RestoreOp)
    {
        QHBoxLayout *layini = new QHBoxLayout;
        UpLabel *labelini = new UpLabel();
        labelini->setVisible(false);
        labelini->setFixedSize(labelsize, labelsize);
        layini->addWidget(labelini);
        UpCheckBox *Inichk  = new UpCheckBox();
        Inichk->setText("fichier de paramètrage RufusAdmin.ini");
        Inichk->setEnabled(OKini);
        Inichk->setChecked(OKini);
        Inichk->setObjectName("ini");
        layini->addWidget(Inichk);
        layini->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        dlg_buprestore->dlglayout()->insertLayout(0, layini);

        QDir rootimgvid = QDir(pathorigin);
        if (rootimgvid.cdUp())
            pathorigin = rootimgvid.absolutePath();
    }
    if (OKvideos)
    {
        // taille du dossier video ---------------------------------------------------------------------------------------------------------------------------------------
        DataDir = Utils::dir_size(pathorigin + NOM_DIR_VIDEOS);
        m_videossize = DataDir["Size"]/1024/1024;
        QHBoxLayout *layVideos = new QHBoxLayout;
        UpLabel *labeVideos = new UpLabel();
        labeVideos->setVisible(false);
        labeVideos->setFixedSize(labelsize, labelsize);
        layVideos->addWidget(labeVideos);
        UpCheckBox *Videoschk  = new UpCheckBox();
        Videoschk->setText("Videos");
        Videoschk->setEnabled(OKvideos || op == BackupOp);
        Videoschk->setChecked(OKvideos || op == BackupOp);
        Videoschk->setObjectName("videos");
        layVideos->addWidget(Videoschk);
        layVideos->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        UpLabel *lblvolvid = new UpLabel();
        lblvolvid->setText(Utils::getExpressionSize(m_videossize));
        layVideos->addWidget(lblvolvid);
        dlg_buprestore->dlglayout()->insertLayout(0, layVideos);
        connect(Videoschk, &UpCheckBox::clicked, this, [=] {CalcTimeBupRestore();});
    }
    if (OKimages)
    {
        // taille du dossier Images ---------------------------------------------------------------------------------------------------------------------------------------
        DataDir = Utils::dir_size(pathorigin + NOM_DIR_IMAGES);
        m_imagessize = DataDir["Size"]/1024/1024;
        QHBoxLayout *layImges = new QHBoxLayout;
        UpLabel *labelmges = new UpLabel();
        labelmges->setVisible(false);
        labelmges->setFixedSize(labelsize, labelsize);
        layImges->addWidget(labelmges);
        UpCheckBox *Imgeschk  = new UpCheckBox();
        Imgeschk->setText("Images");
        Imgeschk->setEnabled(OKimages || op == BackupOp);
        Imgeschk->setChecked(OKimages || op == BackupOp);
        Imgeschk->setObjectName("images");
        layImges->addWidget(Imgeschk);
        layImges->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        UpLabel *lblvolimg = new UpLabel();
        lblvolimg->setText(Utils::getExpressionSize(m_imagessize));
        layImges->addWidget(lblvolimg);
        dlg_buprestore->dlglayout()->insertLayout(0, layImges);
        connect(Imgeschk, &UpCheckBox::clicked, this, [=] {CalcTimeBupRestore();});
    }

    if (OKfactures)
    {
        // taille du dossier Factures ---------------------------------------------------------------------------------------------------------------------------------------
        DataDir = Utils::dir_size(pathorigin + NOM_DIR_FACTURES);
        m_facturessize = DataDir["Size"]/1024/1024;
        if (m_facturessize > 0)
        {
            QHBoxLayout *layFctures = new QHBoxLayout;
            UpLabel *labelmges = new UpLabel();
            labelmges->setVisible(false);
            labelmges->setFixedSize(labelsize, labelsize);
            layFctures->addWidget(labelmges);
            UpCheckBox *Fctureschk  = new UpCheckBox();
            Fctureschk->setText("Factures");
            Fctureschk->setEnabled(OKfactures || op == BackupOp);
            Fctureschk->setChecked(OKfactures || op == BackupOp);
            Fctureschk->setObjectName("factures");
            layFctures->addWidget(Fctureschk);
            layFctures->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
            UpLabel *lblvolfct = new UpLabel();
            lblvolfct->setText(Utils::getExpressionSize(m_facturessize));
            layFctures->addWidget(lblvolfct);
            dlg_buprestore->dlglayout()->insertLayout(0, layFctures);
            connect(Fctureschk, &UpCheckBox::clicked, this, [=] {CalcTimeBupRestore();});
        }
    }

    QHBoxLayout *layBDD = new QHBoxLayout;
    UpLabel *labelBDD = new UpLabel();
    labelBDD->setVisible(false);
    labelBDD->setFixedSize(labelsize, labelsize);
    layBDD->addWidget(labelBDD);
    UpCheckBox *BDDchk  = new UpCheckBox();
    BDDchk->setText("base de données");
    BDDchk->setChecked(true);
    BDDchk->setObjectName("base");
    layBDD->addWidget(BDDchk);
    layBDD->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
    UpLabel *lblvolbase = new UpLabel();
    lblvolbase->setText(Utils::getExpressionSize(m_basesize));
    layBDD->addWidget(lblvolbase);
    dlg_buprestore->dlglayout()->insertLayout(0, layBDD);


    QHBoxLayout *layResume = new QHBoxLayout;
    wdg_resumelbl = new UpLabel();
    layResume->addWidget(wdg_resumelbl);
    dlg_buprestore->dlglayout()->insertLayout(dlg_buprestore->dlglayout()->count()-1, layResume);

    QHBoxLayout *layVolumeLibre = new QHBoxLayout;
    wdg_volumelibrelbl = new UpLabel();
    layVolumeLibre->addWidget(wdg_volumelibrelbl);
    dlg_buprestore->dlglayout()->insertLayout(dlg_buprestore->dlglayout()->count()-1, layVolumeLibre);

    connect(BDDchk, &UpCheckBox::clicked, this, [=] {CalcTimeBupRestore();});

    dlg_buprestore->setFixedWidth(400);
    dlg_buprestore->AjouteLayButtons(UpDialog::ButtonOK);
    connect(dlg_buprestore->OKButton,    &QPushButton::clicked, dlg_buprestore, &QDialog::accept);
    CalcTimeBupRestore();
}

void RufusAdmin::CalcTimeBupRestore()
{
    double time(0);
    qint64 volume(0);
    foreach (UpCheckBox *chk, dlg_buprestore->findChildren<UpCheckBox*>())
    {
        if (chk->objectName() == "base")
        {
            if (chk->isChecked())
                volume += m_basesize;
        }
        else if (chk->objectName() == "images")
        {
            if (chk->isChecked())
                volume += m_imagessize;
        }
        else if (chk->objectName() == "videos")
        {
            if (chk->isChecked())
                volume += m_videossize;
        }
        else if (chk->objectName() == "factures")
        {
            if (chk->isChecked())
                volume += m_facturessize;
        }
    }
    time = (volume/1024 /2)*60000; //duréée approximative de sauvegarde en ms
    QString Volumelitteral = Utils::getExpressionSize(volume);
    QString timelitteral;
    if (Volumelitteral.right(2) == "Go")
    {
        QString timeb = QString::number(time/60000,'f',0);
        if (timeb != "0")
            timelitteral = timeb + tr(" minutes");
        else
            timelitteral = tr("moins d'une minute");
    }
    else if (Volumelitteral.right(2) == "To")
    {
        QString timeb = QString::number(time/60000/60,'f',0);
        if (timeb != "0")
            timelitteral = timeb + tr(" heures");
        else
            timelitteral = tr("moins d'une heure");
    }
    else
        timelitteral = tr("moins d'une minute");
    QString color = m_freespace>volume? "green": "red";
    QString msg = tr("Volume à transférer: ") + " <font color=\""+ color + "\"><b>" + Volumelitteral + "</b></font>";
    wdg_resumelbl->setText(msg + " - " + tr("Temps estimé: ") + timelitteral);
    QString Volumelibre = Utils::getExpressionSize(m_freespace);
    msg = tr("Volume disponible sur le disque: ") + " <font color=\""+ color + "\"><b>" + Volumelibre + "</b></font>";

    wdg_volumelibrelbl->setText(msg);
    dlg_buprestore->OKButton->setEnabled(m_freespace>volume);
}

void RufusAdmin::ConnectTimers()
{
    connect (t_timerUserConnecte,        &QTimer::timeout,      this,   &RufusAdmin::MetAJourLaConnexion);
    connect (t_timerVerifDivers,         &QTimer::timeout,      this,   &RufusAdmin::VerifPosteImport);
    connect (t_timerVerifDivers,         &QTimer::timeout,      this,   &RufusAdmin::VerifVersionBase);
    if (db->ModeAccesDataBase() != Utils::Distant)
    {
        connect (t_timerUserConnecte,    &QTimer::timeout,      this,   &RufusAdmin::ExporteDocs);
        connect (t_timerSupprDocs,       &QTimer::timeout,      this,   &RufusAdmin::SupprimerDocsEtFactures);
        connect (t_timerDocsAExporter,   &QTimer::timeout,      this,   &RufusAdmin::CalcExporteDocs);
    }
    ConnectTimerInactive();
    connect (t_timerSalDatCorrespMsg,    &QTimer::timeout,      this,   &RufusAdmin::VerifModifsFlags);
}

void RufusAdmin::DisconnectTimers()
{
    disconnect (t_timerUserConnecte,     &QTimer::timeout,      this,   &RufusAdmin::MetAJourLaConnexion);
    disconnect (t_timerSupprDocs,        &QTimer::timeout,      this,   &RufusAdmin::SupprimerDocsEtFactures);
    disconnect (t_timerVerifDivers,      &QTimer::timeout,      this,   &RufusAdmin::VerifPosteImport);
    disconnect (t_timerVerifDivers,      &QTimer::timeout,      this,   &RufusAdmin::VerifVersionBase);
    disconnect (t_timerDocsAExporter,    &QTimer::timeout,      this,   &RufusAdmin::CalcExporteDocs);
    disconnect (t_timerUserConnecte,     &QTimer::timeout,      this,   &RufusAdmin::ExporteDocs);
    DisconnectTimerInactive();
    t_timerSalDatCorrespMsg  ->disconnect();
}

void RufusAdmin::ConnectTimerInactive()
{
    connect(ui->MiseEnVeilleprogressBar, &QProgressBar::valueChanged, this, [=]
    {
        if (ui->MiseEnVeilleprogressBar->value() == ui->MiseEnVeilleprogressBar->minimum())
            MasqueAppli();
    });
}

void RufusAdmin::DisconnectTimerInactive()
{
    ui->MiseEnVeilleprogressBar->disconnect();
}

qint64 RufusAdmin::CalcBaseSize()
{
    qint64 basesize = 0;
    QString sizereq = "SELECT SUM(SizeMB) from "
                      "(SELECT table_schema, round(sum(data_length+index_length)/1024/1024,4) AS SizeMB FROM information_schema.tables"
                      " where table_schema = 'ComptaMedicale'"
                      " or table_schema = 'Ophtalmologie'"
                      " or table_schema = 'rufus'"
                      " GROUP BY table_schema)"
                      " as bdd";
    QVariantList basedata = db->getFirstRecordFromStandardSelectSQL(sizereq,m_ok);
    if (m_ok && basedata.size()>0)
        basesize = basedata.at(0).toLongLong();
    return basesize;
}

void RufusAdmin::ConnexionBase()
{
    db = DataBase::I();
    QString error = "";
    db->setModeacces(Utils::Poste);
    db->initParametresConnexionSQL("localhost", 3306);
    error = db->connectToDataBase(DB_RUFUS);

    if( error.size() )
    {
        UpMessageBox::Watch(this, tr("Erreur sur le serveur MySQL"),
                            tr("Impossible de se connecter au serveur avec le login ") + LOGIN_SQL
                            + tr(" et ce mot de passe") + "\n"
                            + error);
        exit(0);
    }
    db->StandardSQL("SET GLOBAL max_allowed_packet=" MAX_ALLOWED_PACKET "*1024*1024 ;");
}

/*-----------------------------------------------------------------------------------------------------------------
    -- Détermination du lieu exercice pour la session en cours -------------------------------------------------------------
    -----------------------------------------------------------------------------------------------------------------*/
void RufusAdmin::DetermineLieuExercice()
{
    if (db->ModeAccesDataBase() == Utils::Distant)
    {
        QList<Site*> listEtab;
        foreach (Site *site, *Datas::I()->sites->sites())
        {
            if (site->id() != m_parametres->idlieupardefaut())
            listEtab << site;
        }
        if (listEtab.size() == 0)
            return;
        else if (listEtab.size() == 1)
        {
            Datas::I()->sites->setcurrentsite(listEtab.first());
            return;
        }

        /* Cas ou le praticien travaille dans plusieur centres
         * on lui demande de sélectionner le centre où il se trouve au moment de la connexion
        */
        DisconnectTimerInactive();
        UpDialog *gAskLieux     = new UpDialog();
        gAskLieux               ->AjouteLayButtons();
        QGroupBox*boxlieux      = new QGroupBox();
        gAskLieux->dlglayout()  ->insertWidget(0,boxlieux);
        boxlieux                ->setAccessibleName("Parent");
        boxlieux                ->setTitle(tr("D'où vous connectez-vous?"));
        QFontMetrics fm         = QFontMetrics(qApp->font());
        int hauteurligne        = int(fm.height()*1.6);
        boxlieux                ->setFixedHeight(((listEtab.size() + 1)*hauteurligne)+5);
        QVBoxLayout *vbox       = new QVBoxLayout;
        bool isFirst = true;
        foreach (Site *etab, listEtab)
        {
            UpRadioButton *pradiobutt = new UpRadioButton(boxlieux);
            pradiobutt->setText(etab->nom());
            pradiobutt->setitem(etab);
            pradiobutt->setImmediateToolTip(etab->coordonnees());
            pradiobutt->setChecked(isFirst);
            vbox      ->addWidget(pradiobutt);
            isFirst = false;
        }
        vbox                    ->setContentsMargins(8,0,8,0);
        boxlieux                ->setLayout(vbox);
        gAskLieux               ->setModal(true);
        gAskLieux->dlglayout()  ->setSizeConstraint(QLayout::SetFixedSize);
        connect(gAskLieux->OKButton,   &QPushButton::clicked,  gAskLieux, &UpDialog::accept);
        gAskLieux->exec();
        foreach (UpRadioButton * rb, boxlieux->findChildren<UpRadioButton*>())
            if( rb->isChecked() )
            {
                Datas::I()->sites->setcurrentsite(qobject_cast<Site*>(rb->item()));
                break;
            }
        delete gAskLieux;
        ConnectTimerInactive();
    }
    else
    {
        if (m_parametres->idlieupardefaut()>=1)
            Datas::I()->sites->setcurrentsite(Datas::I()->sites->getById(m_parametres->idlieupardefaut()));
        else
        {
            Datas::I()->sites->setcurrentsite(Datas::I()->sites->sites()->first());
            db->StandardSQL("update " TBL_PARAMSYSTEME " set idLieuParDefaut = " + QString::number(Datas::I()->sites->idcurrentsite()));
            m_parametres->setidlieupardefaut(Datas::I()->sites->idcurrentsite());
        }
    }
}

bool RufusAdmin::eventFilter(QObject *obj, QEvent *event)
{
    if (!event->spontaneous() && event->type() != QEvent::UpdateRequest)
    {
        //qDebug() << "event = " << event;
        ui->MiseEnVeilleprogressBar->setValue(m_dureeVeille);
    }
    return QWidget::eventFilter(obj, event);
}

QString RufusAdmin::getDossierDocuments(QString Appareil)
{
    return m_settings->value("DossierEchangeImagerie/" + Appareil).toString();
}

void RufusAdmin::NouvAppareil()
{
    AskAppareil();
    delete dlg_askAppareil;
}

void RufusAdmin::SupprAppareil()
{
    if (ui->AppareilsConnectesupTableWidget->selectedItems().size()==0)
        return;
    QString req = " select list." CP_TITREEXAMEN_APPAREIL ", list." CP_NOMAPPAREIL_APPAREIL " from " TBL_LISTEAPPAREILS " list, " TBL_APPAREILSCONNECTESCENTRE " appcon"
                  " where list." CP_ID_APPAREIL " = appcon." CP_IDAPPAREIL_APPAREILS
                  " and list." CP_ID_APPAREIL " = " + ui->AppareilsConnectesupTableWidget->selectedItems().at(0)->text();
    QList<QVariantList> listapps = db->StandardSelectSQL(req, m_ok);
    if (!m_ok)
        return;
    if (listapps.size()==0)
        return;
    UpMessageBox msgbox(this);
    msgbox.setText(tr("Suppression d'un appareil!"));
    msgbox.setInformativeText(tr("Voulez vous vraiment supprimer l'appareil") + "\n"
                              + listapps.at(0).at(1).toString() + " ?");
    msgbox.setIcon(UpMessageBox::Warning);
    UpSmallButton *OKBouton = new UpSmallButton();
    OKBouton->setText(tr("Supprimer"));
    UpSmallButton *NoBouton = new UpSmallButton();
    NoBouton->setText(tr("Annuler"));
    msgbox.addButton(NoBouton, UpSmallButton::CANCELBUTTON);
    msgbox.addButton(OKBouton, UpSmallButton::SUPPRBUTTON);
    msgbox.exec();
    if (msgbox.clickedButton() == OKBouton)
    {
        req = "delete from " TBL_APPAREILSCONNECTESCENTRE " where " CP_IDAPPAREIL_APPAREILS " = "
                + ui->AppareilsConnectesupTableWidget->selectedItems().at(0)->text()
                + " and " CP_IDLIEU_APPAREILS " = " + QString::number(Datas::I()->sites->idcurrentsite());
        db->StandardSQL(req);
        m_settings->remove("DossierEchangeImagerie/" + listapps.at(0).at(1).toString());
        Remplir_Table();
    }
}

void RufusAdmin::RestoreFontAppli()
{
    QFont AppFont = QFont(POLICEPARDEFAUT);
    Utils::CalcFontSize(AppFont);
    qApp->setFont(AppFont);
}

void RufusAdmin::Remplir_Table()
{
    QTableWidgetItem    *pItem0;
    QTableWidgetItem    *pItem1;
    QTableWidgetItem    *pItem2;
    QTableWidgetItem    *pItem3;
    UpLineEdit          *line4;
    UpPushButton        *dossbouton;
    int i;
    QFontMetrics fm(qApp->font());
    QFont disabledFont = qApp->font();
    disabledFont.setItalic(true);
    QPalette palette;
    palette.setColor(QPalette::Text,Qt::gray);

    ui->AppareilsConnectesupTableWidget->clearContents();

    QString  Remplirtablerequete = "SELECT list." CP_ID_APPAREIL ", list." CP_TITREEXAMEN_APPAREIL ", list." CP_NOMAPPAREIL_APPAREIL ", " CP_FORMAT_APPAREIL
              " FROM "  TBL_APPAREILSCONNECTESCENTRE " appcon , " TBL_LISTEAPPAREILS " list"
              " where list." CP_ID_APPAREIL " = appcon." CP_IDAPPAREIL_APPAREILS
              " and " CP_IDLIEU_APPAREILS " = " + QString::number(Datas::I()->sites->idcurrentsite()) +
              " ORDER BY " CP_TITREEXAMEN_APPAREIL;
    QList<QVariantList> listapps = db->StandardSelectSQL(Remplirtablerequete, m_ok);
    if (!m_ok)
        return;
    ui->AppareilsConnectesupTableWidget->setRowCount(listapps.size());
    for (i = 0; i < listapps.size(); i++)
    {
        pItem0      = new QTableWidgetItem() ;
        pItem1      = new QTableWidgetItem() ;
        pItem2      = new QTableWidgetItem() ;
        pItem3      = new QTableWidgetItem() ;
        line4       = new UpLineEdit(ui->AppareilsConnectesupTableWidget);
        dossbouton  = new UpPushButton(ui->AppareilsConnectesupTableWidget);

        int col = 0;
        pItem0->setText(listapps.at(i).at(0).toString());                             // idAppareil
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem0);

        col++; //1
        pItem1->setText(listapps.at(i).at(1).toString());                             // TypeExamen
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem1);

        col++; //2
        pItem2->setText(listapps.at(i).at(2).toString());                             // NomAppareil
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem2);

        col++; //3                                                                    // Format fichier
        pItem3->setText(listapps.at(i).at(3).toString());
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem3);

        col++; //4                                                                    // chemin
        line4->setText(getDossierDocuments(listapps.at(i).at(2).toString()));
        line4->setRow(i);
        line4->setFocusPolicy(Qt::NoFocus);
        line4->setStyleSheet("UpLineEdit {background-color:white; border: 0px solid rgb(150,150,150);border-radius: 0px;}"
                              "UpLineEdit:focus {border: 0px solid rgb(164, 205, 255);border-radius: 0px;}");
        connect(line4,  &UpLineEdit::TextModified,   this,   &RufusAdmin::EnregDossierStockageApp);
        ui->AppareilsConnectesupTableWidget->setCellWidget(i,col,line4);

        col++; //5                                                                    // bouton
        QPixmap pix = ic_SortirDossier.pixmap(QSize(15,15));
        QIcon ic;
        ic.addPixmap(pix);
        dossbouton->setIcon(ic);
        dossbouton->setiD(listapps.at(i).at(0).toInt());
        dossbouton->setFixedSize(15,15);
        dossbouton->setFlat(true);
        dossbouton->setFocusPolicy(Qt::NoFocus);
        QWidget *widg = new QWidget;
        QHBoxLayout *l = new QHBoxLayout();
        l->setAlignment( Qt::AlignCenter );
        l->setContentsMargins(0,0,0,0);
        l->addWidget(dossbouton);
        widg->setLayout(l);
        ui->AppareilsConnectesupTableWidget->setCellWidget(i,col,widg);
        connect(dossbouton, &QPushButton::clicked,  this,   &RufusAdmin::ChoixDossierStockageApp);

        ui->AppareilsConnectesupTableWidget->setRowHeight(i,int(fm.height()*1.3));
    }

    m_listeAppareilsNonConnectes.clear();
    QString req = "select " CP_NOMAPPAREIL_APPAREIL " from " TBL_LISTEAPPAREILS
                  " where " CP_ID_APPAREIL " not in (select " CP_IDAPPAREIL_APPAREILS " from " TBL_APPAREILSCONNECTESCENTRE " where " CP_IDLIEU_APPAREILS " = " + QString::number(Datas::I()->sites->idcurrentsite()) + ")";
    QList<QVariantList> listnomsapps = db->StandardSelectSQL(req, m_ok);
    if (listnomsapps.size() == 0)
        wdg_buttonframe->wdg_plusBouton->setEnabled(false);
    else
        for (int i=0; i<listnomsapps.size(); i++)
            m_listeAppareilsNonConnectes << listnomsapps.at(i).at(0).toString();
    wdg_buttonframe->wdg_moinsBouton->setEnabled(listapps.size()>0);
}

void RufusAdmin::setPosteImportDocs(bool a)
{
    if (db->ModeAccesDataBase() == Utils::Distant)
        return;
    // Il n'y pas de variables utilisateur globale dans MySQL, on est donc obligé de passer par une procédure stockée pour en simuler une
    // pour créer une procédure avec Qt, séparer le drop du create, ne pas utiliser les délimiteurs et utiliser les retours à la ligne \n\.......

    // si a = true, on se met en poste importateur - Admin
    // si a = false, on retire le poste en cours et on met NULL à la place.

    QString IpAdress("NULL");
    QString req = "USE `" DB_RUFUS "`;";
    db->StandardSQL(req);

    req = "DROP PROCEDURE IF EXISTS " MYSQL_PROC_POSTEIMPORTDOCS ";";
    db->StandardSQL(req);

    if (a)
        IpAdress = QHostInfo::localHostName() + " - " NOM_ADMINISTRATEUR;
    req = "CREATE PROCEDURE " MYSQL_PROC_POSTEIMPORTDOCS "()\n\
          BEGIN\n\
          SELECT '" + IpAdress + "';\n\
          END ;";
    db->StandardSQL(req);
    if (a)
        MetAJourLaConnexion();
}

void RufusAdmin::ChoixButtonFrame()
{
    switch (wdg_buttonframe->Choix()) {
    case WidgetButtonFrame::Plus:
        NouvAppareil();
        break;
    case WidgetButtonFrame::Moins:
        SupprAppareil();
        break;
    default:
        break;
    }
}

void RufusAdmin::ChoixDossierStockageApp()
{
    UpPushButton *bout = static_cast<UpPushButton*>(sender());
    QString req = "select " CP_TITREEXAMEN_APPAREIL ", " CP_NOMAPPAREIL_APPAREIL " from " TBL_LISTEAPPAREILS " where " CP_ID_APPAREIL " = " + QString::number(bout->iD());
    QString exam = "";
    QVariantList examdata = db->getFirstRecordFromStandardSelectSQL(req, m_ok);
    if (!m_ok)
        return;
    if (examdata.size()>0)
        exam = examdata.at(1).toString();
    DisconnectTimerInactive();
    QString dir = getDossierDocuments(exam);
    if (dir == "" || !QDir(dir).exists())
        dir = PATH_DIR_RUFUS;
    QUrl url = Utils::getExistingDirectoryUrl(this, "", QUrl::fromLocalFile(dir), QStringList()<<db->parametres()->dirbkup());
    if (url == QUrl())
        return;
    int row;
    UpLineEdit *line = Q_NULLPTR;
    row = ui->AppareilsConnectesupTableWidget->findItems(QString::number(bout->iD()), Qt::MatchExactly).at(0)->row();
    line    = dynamic_cast<UpLineEdit*>(ui->AppareilsConnectesupTableWidget->cellWidget(row,4));
    if (line!=Q_NULLPTR)
        line->setText(url.path());
    m_settings->setValue("DossierEchangeImagerie/" + exam, url.path());
    ConnectTimerInactive();
}

void RufusAdmin::EnregistreAppareil()
{
    if (!dlg_askAppareil) return;
    QString req = "insert into " TBL_APPAREILSCONNECTESCENTRE " (" CP_IDAPPAREIL_APPAREILS ", " CP_IDLIEU_APPAREILS ") Values("
                  " (select " CP_ID_APPAREIL " from " TBL_LISTEAPPAREILS " where " CP_NOMAPPAREIL_APPAREIL " = '" + dlg_askAppareil->findChildren<UpComboBox*>().at(0)->currentText() + "'), "
                  + QString::number(Datas::I()->sites->idcurrentsite()) + ")";
    db->StandardSQL(req);
    dlg_askAppareil->done(0);
    Remplir_Table();
}

void RufusAdmin::EnregDossierStockageApp(QString dir)
{
    UpLineEdit *line    = dynamic_cast<UpLineEdit*>(sender());
    if (line==Q_NULLPTR) return;
    if (!QDir(dir).exists() && dir != "")
    {
        QString textline = line->valeuravant();
        UpMessageBox::Watch(this,tr("Repertoire invalide!"));
        line->setText(textline);
        return;
    }
    QString id;
    id = ui->AppareilsConnectesupTableWidget->item(line->Row(),0)->text();
    QString req = "select " CP_NOMAPPAREIL_APPAREIL " from " TBL_LISTEAPPAREILS " where " CP_ID_APPAREIL " = " + id;
    QString exam = "";
    QVariantList examdata = db->getFirstRecordFromStandardSelectSQL(req, m_ok, tr("Impossible de retrouver le nom de l'appareil"));
    if (!m_ok)
        return;
    if (examdata.size()>0)
        exam = examdata.at(0).toString();
    if (exam != "")
        m_settings->setValue("DossierEchangeImagerie/" + exam, dir);
}

void RufusAdmin::EnregistreEmplacementServeur(int idx)
{
    int idlieu = ui->EmplacementServeurupComboBox->itemData(idx).toInt();
    if (ui->EmplacementServeurupComboBox->itemData(idx).toString() != "")
    {
        db->setidlieupardefaut(idlieu);
        Datas::I()->sites->setcurrentsite(Datas::I()->sites->getById(idlieu));
        ui->AppareilsconnectesupLabel->setText(tr("Appareils connectés au réseau") + " <font color=\"green\"><b>" + Datas::I()->sites->currentsite()->nom() + "</b></font> ");
    }
}

//!> supprime les fichiers de logs antérieurs à J - anciennete jours
void RufusAdmin::EpureLogs(int anciennete)
{
    QDir dirlogs = QDir(PATH_DIR_LOGS);
    QStringList listfiles = dirlogs.entryList();
    for (int i=0; i<listfiles.size(); ++i)
    {
        QFile file(listfiles.at(i));
        QDate datefile = QDate::fromString(file.fileName().left(10), "yyyy-MM-dd");
        if (datefile < QDate::currentDate().addDays(-anciennete))
            QFile::remove(PATH_DIR_LOGS + "/" + file.fileName());
    }
}

void RufusAdmin::EnregistreNouvMDPAdmin()
{
    if (dlg_askMDP != Q_NULLPTR)
    {
        // Vérifier la cohérence
        QString anc, nouv, confirm;
        UpMessageBox msgbox(this);
        msgbox.setText(tr("Erreur"));
        msgbox.setIcon(UpMessageBox::Warning);
        UpSmallButton *OKBouton = new UpSmallButton;
        OKBouton->setText("OK");
        msgbox.addButton(OKBouton, UpSmallButton::STARTBUTTON);
        anc         = dlg_askMDP->findChild<UpLineEdit*>(m_ancMDP)->text();
        nouv        = dlg_askMDP->findChild<UpLineEdit*>(m_nouvMDP)->text();
        confirm     = dlg_askMDP->findChild<UpLineEdit*>(m_confirmMDP)->text();

        if (anc == "")
        {
            Utils::playAlarm();
            msgbox.setInformativeText(tr("Ancien mot de passe requis"));
            dlg_askMDP->findChild<UpLineEdit*>(m_ancMDP)->setFocus();
            msgbox.exec();
            return;
        }
        if (Utils::calcSHA1(anc) != m_parametres->mdpadmin() && anc != m_parametres->mdpadmin())
        {
            Utils::playAlarm();
            msgbox.setInformativeText(tr("Le mot de passe que vous voulez modifier n'est pas le bon\n"));
            dlg_askMDP->findChild<UpLineEdit*>(m_ancMDP)->setFocus();
            msgbox.exec();
            return;
        }
        if (!Utils::RegularExpressionMatches(Utils::rgx_AlphaNumeric_3_12,nouv) || nouv == "")
        {
            Utils::playAlarm();
            msgbox.setInformativeText(tr("Le nouveau mot de passe n'est pas conforme\n(au moins 3 caractères - chiffres ou lettres non accentuées -\n"));
            dlg_askMDP->findChild<UpLineEdit*>(m_nouvMDP)->setFocus();
            msgbox.exec();
            return;
        }
        if (nouv != confirm)
        {
            Utils::playAlarm();
            msgbox.setInformativeText("Les mots de passe ne correspondent pas\n");
            dlg_askMDP->findChild<UpLineEdit*>(m_nouvMDP)->setFocus();
            msgbox.exec();
            return;
        }
        msgbox.setText(tr("Modifications enregistrées"));
        msgbox.setInformativeText(tr("Le nouveau mot de passe a été enregistré avec succès"));
        db->setmdpadmin(Utils::calcSHA1(nouv));
        dlg_askMDP->done(0);
        msgbox.exec();
    }
}

void RufusAdmin::EnvoieTCPMessage(enum TypeMessage typemessage, QString msg, int dest)
{
   if (!m_utiliseTCP)
       return;
   if (typemessage == TOUS)
       TCPServer->envoyerATous(msg);
   else if (typemessage == BAL)
       TCPServer->envoyerBALMsgA(dest, msg);
}

void RufusAdmin::setMapIcons()
{
    map_icons["OK"]             =  ic_OK;
    map_icons["Annul"]          =  ic_Annul;
    map_icons["Euro"]           =  ic_Euro;
    map_icons["EuroCount"]      =  ic_EuroCount;
    map_icons["FermeAppuye"]    =  ic_FermeAppuye;
    map_icons["FermeRelache"]   =  ic_FermeRelache;
    map_icons["Help"]           =  ic_Help;
    map_icons["Null"]           =  ic_Null;
}

QMap<QString, QIcon> RufusAdmin::MapIcons()
{
    return map_icons;
}

void RufusAdmin::CalcExporteDocs()
{
    if (db->ModeAccesDataBase() == Utils::Distant)
        return;
    QString totreq = "SELECT " CP_ID_DOCSEXTERNES " FROM " TBL_DOCSEXTERNES " where " CP_JPG_DOCSEXTERNES " is not null or " CP_PDF_DOCSEXTERNES " is not null limit 1";
    ui->ExportImagespushButton->setEnabled(db->StandardSelectSQL(totreq, m_ok).size()>0);
}

void RufusAdmin::ExporteDocs()
{
    bool ok;
    QString NomDirStockageImagerie  = db->dirimagerie();
    if (!QDir(NomDirStockageImagerie).exists() || NomDirStockageImagerie == "")
    {
        QString msg = tr("Le dossier de sauvegarde d'imagerie") + " <font color=\"red\"><b>" + NomDirStockageImagerie + "</b></font>" + tr(" n'existe pas");
        msg += "<br />" + tr("Renseignez un dossier valide dans") + " <font color=\"green\"><b>" + tr("Emplacement de stockage des documents archivés") + "</b></font>";
        ShowMessage::I()->SplashMessage(msg, 6000);
        return;
    }
    QString CheminEchecTransfrDir   = NomDirStockageImagerie + NOM_DIR_ECHECSTRANSFERTS;
    if (!Utils::mkpath(CheminEchecTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminEchecTransfrDir + "</b></font>" + tr(" invalide");
        ShowMessage::I()->SplashMessage(msg, 3000);
        return;
    }
    int total = db->StandardSelectSQL("SELECT " CP_ID_DOCSEXTERNES " FROM " TBL_DOCSEXTERNES " where " CP_JPG_DOCSEXTERNES " is not null or " CP_PDF_DOCSEXTERNES " is not null",ok).size();
    total +=    db->StandardSelectSQL("SELECT " CP_ID_FACTURES " FROM " TBL_FACTURES " where " CP_JPG_FACTURES " is not null or " CP_PDF_FACTURES " is not null", ok).size();
    if (total>100)
    {
        int min = total/180;
        int hour = min/60;
        min = min - (hour*60);
        QString tps = QTime(hour,min).toString("H'H'mm'mn'");
        if (UpMessageBox::Question(this, tr("Nombreux documents à convertir"),
                                   tr("Il y a ") + QString::number(total) + tr(" documents à convertir.") +"\n" +
                                   tr("Cette procédure devrait durer environ ") + tps + "\n" +
                                   tr("et figera l'éxécution du programme durant ce temps") + tps + "\n" +
                                   tr("Voulez vous le faire maintenant?"))
                                   !=UpSmallButton::STARTBUTTON)
        {
            disconnect (t_timerDocsAExporter,    &QTimer::timeout,      this,   &RufusAdmin::CalcExporteDocs);
            return;
        }
    }
    if (total==0)
        return;

    DisconnectTimers();



/* LES DOCUMENTS  ============================================*/
    int faits = 0;
    QTime debut = QTime::currentTime();
    QStringList listmsg;
    QString duree;
    QString CheminOKTransfrDir      = NomDirStockageImagerie + NOM_DIR_IMAGES;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        ShowMessage::I()->SplashMessage(msg, 3000);
        return;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES JPG
    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString req = "SELECT " CP_ID_DOCSEXTERNES ", " CP_IDPAT_DOCSEXTERNES ", " CP_SOUSTYPEDOC_DOCSEXTERNES ", " CP_DATE_DOCSEXTERNES ", " CP_JPG_DOCSEXTERNES ", " CP_LIENFICHIER_DOCSEXTERNES ", " CP_TYPEDOC_DOCSEXTERNES " FROM " TBL_DOCSEXTERNES " where " CP_JPG_DOCSEXTERNES " is not null";
    //qDebug() << req;
    QList<QVariantList> listexportjpg = db->StandardSelectSQL(req, ok );
    if (ok)
        for (int i=0; i<listexportjpg.size(); i++)
        {
            /* si le lien vers le fichier est valide, on efface le champ jpg et on passe à la réponse suivante*/
            if (listexportjpg.at(i).at(5).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOM_DIR_IMAGES + listexportjpg.at(i).at(5).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL ("update " TBL_DOCSEXTERNES " set " CP_JPG_DOCSEXTERNES " = null where " CP_ID_DOCSEXTERNES " = " + listexportjpg.at(i).at(0).toString() );
                    continue;
                }
            }
            QDate datetransfer    = listexportjpg.at(i).at(3).toDate();
            QString CheminOKTransfrDirImg    = CheminOKTransfrDir + "/" + datetransfer.toString("yyyy-MM-dd");
            if (!Utils::mkpath(CheminOKTransfrDirImg))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDirImg + "</b></font>" + tr(" invalide");
                QStringList listmsg;
                listmsg << msg;
                ShowMessage::I()->SplashMessage(listmsg, 3000);
                return;
            }
            QString NomFileDoc = listexportjpg.at(i).at(1).toString() + "_" + listexportjpg.at(i).at(6).toString() + "-"
                    + listexportjpg.at(i).at(2).toString().replace("/",".") + "_"
                    + listexportjpg.at(i).at(3).toDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("HHmmss")
                    + "-" + listexportjpg.at(i).at(0).toString();
            QString CheminOKTransfrDoc  = CheminOKTransfrDirImg + "/" + NomFileDoc + "." JPG;
            QString CheminOKTransfrProv = CheminOKTransfrDirImg + "/" + NomFileDoc + "prov." JPG;
            QByteArray ba = listexportjpg.at(i).at(4).toByteArray();
            QPixmap pix;
            pix.loadFromData(ba);
        /*!
         * On utilise le passage par les QPixmap parce que le mèthode suivante consistant
         * à réintégrer le QByteArray directement dans le fichier aboutit à un fichier corrompu...
         * QFile prov (CheminOKTransfrProv);
            if (prov.open(QIODevice::Append))
            {
                QTextStream out(&prov);
                out << ba;
            }
        */
            if (!pix.save(CheminOKTransfrProv, "jpeg"))
            {
                //qDebug() << "erreur";
                return;
            }
            if (!Utils::CompressFileJPG(CheminOKTransfrProv, db->dirimagerie()))
            {
                db->SupprRecordFromTable(listexportjpg.at(i).at(0).toInt(), CP_ID_FACTURES, TBL_FACTURES);
                continue;
            }
            QFile prov(CheminOKTransfrProv);
            if (prov.open(QIODevice::ReadWrite))
            {
                prov.copy(CheminOKTransfrDoc);
                prov.remove();
            }
            else
                return;
            db->StandardSQL("update " TBL_DOCSEXTERNES " set "
                            CP_JPG_DOCSEXTERNES " = null,"
                            CP_LIENFICHIER_DOCSEXTERNES " = '/" + datetransfer.toString("yyyy-MM-dd") + "/" + Utils::correctquoteSQL(NomFileDoc) + "." JPG
                            "' where " CP_ID_DOCSEXTERNES " = " + listexportjpg.at(i).at(0).toString());
            faits ++;
            int nsec = debut.secsTo(QTime::currentTime());
            int min = nsec/60;
            int hour = min/60;
            min = min - (hour*60);
            nsec = nsec - (hour*3600) - (min*60);
            listmsg.clear();
            duree = QTime(hour,min,nsec).toString("HH:mm:ss");
            listmsg << "JPG - " + NomFileDoc + " - " + QString::number(faits) + "/" + QString::number(total) + " - "  + duree;
            QTime dieTime= QTime::currentTime().addMSecs(2);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            UpSystemTrayIcon::I()->showMessages(tr("Messages"),listmsg, Icons::icSunglasses(), 10);
        }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES PDF
    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString reqpdf = "SELECT " CP_ID_DOCSEXTERNES ", " CP_IDPAT_DOCSEXTERNES ", " CP_SOUSTYPEDOC_DOCSEXTERNES ", " CP_DATE_DOCSEXTERNES ", " CP_PDF_DOCSEXTERNES ", " CP_LIENFICHIER_DOCSEXTERNES ", " CP_COMPRESSION_DOCSEXTERNES ", " CP_TYPEDOC_DOCSEXTERNES " FROM " TBL_DOCSEXTERNES " where " CP_PDF_DOCSEXTERNES " is not null";
    QList<QVariantList> listexportpdf = db->StandardSelectSQL(reqpdf, ok );
    if (ok)
        for (int i=0; i<listexportpdf.size(); i++)
        {
            if (listexportpdf.at(i).at(5).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOM_DIR_IMAGES + listexportpdf.at(i).at(5).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL ("update " TBL_DOCSEXTERNES " set " CP_PDF_DOCSEXTERNES " = null where " CP_ID_DOCSEXTERNES " = " + listexportpdf.at(i).at(0).toString());
                    continue;
                }
            }
            QDate datetransfer    = listexportpdf.at(i).at(3).toDate();
            QString CheminOKTransfrDirImg      = CheminOKTransfrDir + "/" + datetransfer.toString("yyyy-MM-dd");
            if (!Utils::mkpath(CheminOKTransfrDirImg))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDirImg + "</b></font>" + tr(" invalide");
                ShowMessage::I()->SplashMessage(msg, 3000);
                return;
            }
            QString NomFileDoc = listexportpdf.at(i).at(1).toString() + "_" + listexportpdf.at(i).at(7).toString() + "-"
                    + listexportpdf.at(i).at(2).toString().replace("/",".") + "_"
                    + listexportpdf.at(i).at(3).toDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("HHmmss")
                    + "-" + listexportpdf.at(i).at(0).toString()  + "." PDF;
            QString CheminOKTransfrDoc = CheminOKTransfrDirImg + "/" + NomFileDoc;

            QByteArray bapdf;
            bapdf.append(listexportpdf.at(i).at(4).toByteArray());

            QBuffer buf(&bapdf);
            buf.open(QIODevice::ReadWrite);
            QPdfDocument document;
            document.load(&buf);
            if( document.pageCount() > 0)
            {
                QFile file(CheminOKTransfrDoc);
                if (file.open(QIODevice::NewOnly))
                {
                    QDataStream out(&file);
                    out << bapdf;
                }
            } else {

                UpSystemTrayIcon::I()->showMessages(tr("Messages"), listmsg, Icons::icSunglasses(), 3000);
                QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer.toString("yyyy-MM-dd") + ".txt";
                QFile   echectrsfer(echectrsfername);
                if (echectrsfer.open(QIODevice::Append))
                {
                    QTextStream out(&echectrsfer);
                    out << NomFileDoc << "\n" ;
                    echectrsfer.close();
                    QFile CD(CheminEchecTransfrDir + "/" + NomFileDoc);
                    if (CD.open(QIODevice::OpenModeFlag::NewOnly))
                    {
                        QDataStream out(&CD);
                        out << bapdf;
                    }
                }
                QString delreq = "delete from  " TBL_DOCSEXTERNES " where " CP_ID_DOCSEXTERNES " = " + listexportpdf.at(i).at(0).toString();
                //qDebug() << delreq;
                db->StandardSQL(delreq);
                continue;
            }

#if !defined(Q_OS_WIN)
            QFile CC(CheminOKTransfrDoc);
            CC.open(QIODevice::ReadWrite);
            CC.setPermissions( QFileDevice::ReadOther
                              | QFileDevice::ReadGroup
                              | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                              | QFileDevice::ReadUser   | QFileDevice::WriteUser);
            CC.close();
#endif
            db->StandardSQL("update " TBL_DOCSEXTERNES " set " CP_PDF_DOCSEXTERNES " = null, " CP_COMPRESSION_DOCSEXTERNES " = null,"
                            CP_LIENFICHIER_DOCSEXTERNES " = '/" + datetransfer.toString("yyyy-MM-dd") + "/" + Utils::correctquoteSQL(NomFileDoc)  + "'"
                            " where " CP_ID_DOCSEXTERNES " = " + listexportpdf.at(i).at(0).toString());
            faits ++;
            int nsec = debut.secsTo(QTime::currentTime());
            int min = nsec/60;
            int hour = min/60;
            min = min - (hour*60);
            nsec = nsec - (hour*3600) - (min*60);
            listmsg.clear();
            duree = QTime(hour,min,nsec).toString("HH:mm:ss");
            listmsg << "PDF - " + NomFileDoc + " - " + QString::number(faits) + "/" + QString::number(total) + " - "  + duree;
            QTime dieTime= QTime::currentTime().addMSecs(2);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            UpSystemTrayIcon::I()->showMessages(tr("Messages"), listmsg, Icons::icSunglasses(), 10);
        }
    int totdoc = listexportjpg.size() + listexportpdf.size();
    if (totdoc > 0)
    {
        listmsg <<  tr("export terminé") << QString::number(totdoc) + (totdoc>1? tr(" documents exportés en ") : tr(" document exporté en "))  + duree;
        UpSystemTrayIcon::I()->showMessages(tr("Messages"), listmsg, Icons::icSunglasses(), 3000);
    }



/* LES FACTURES  ============================================*/

    faits = 0;
    debut = QTime::currentTime();
    listmsg.clear();
    duree = "";
    CheminOKTransfrDir  = NomDirStockageImagerie + NOM_DIR_FACTURES;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        ShowMessage::I()->SplashMessage(msg, 3000);
        return;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES JPG
    //-----------------------------------------------------------------------------------------------------------------------------------------
    req = "SELECT " CP_ID_FACTURES ", " CP_DATEFACTURE_FACTURES ", " CP_LIENFICHIER_FACTURES ", " CP_INTITULE_FACTURES ", " CP_ECHEANCIER_FACTURES
            ", " CP_IDDEPENSE_FACTURES ", " CP_JPG_FACTURES " FROM " TBL_FACTURES
            " where " CP_JPG_FACTURES " is not null";
    //qDebug() << req;
    QList<QVariantList> listexportjpgfact = db->StandardSelectSQL(req, ok);
    if (ok)
        for (int i=0; i<listexportjpgfact.size(); i++)
        {
            /* si le lien vers le fichier est valide, on efface le champ jpg et on passe à la réponse suivante*/
            if (listexportjpgfact.at(i).at(2).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOM_DIR_FACTURES + listexportjpgfact.at(i).at(2).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL("update " TBL_FACTURES " set " CP_JPG_FACTURES " = null where " CP_ID_FACTURES " = " + listexportjpgfact.at(i).at(0).toString());
                    continue;
                }
            }
            /*!
             * nommage d'un fichier facture
             * idFacture + "_" + "ECHEANCIER ou FACTURE" + "_" + Intitule + "_" + DateFacture + ( + "_" + iddepense si facture et pas échéancier)
             */
            QDate datetransfer  = listexportjpgfact.at(i).at(1).toDate();
            QString user;

            QString NomFileDoc = listexportjpgfact.at(i).at(0).toString() + "_"
                    + (listexportjpgfact.at(i).at(4).toInt()==1? ECHEANCIER : FACTURE) + "-"
                    + listexportjpgfact.at(i).at(3).toString().replace("/",".") + "_"
                    + datetransfer.toString("yyyyMMdd");
            // on recherche le user à l'origine de cette facture
            QList<QVariantList> Listeusr;
            if (listexportjpgfact.at(i).at(4).toInt()==1)          // c'est un échéancier
                req = "select dep." CP_IDUSER_DEPENSES ", " CP_LOGIN_USR " from " TBL_DEPENSES " dep, " TBL_UTILISATEURS " usr"
                                                                                                              " where dep." CP_IDUSER_DEPENSES " = usr." CP_ID_USR
                                                                                                              " and " CP_IDFACTURE_DEPENSES " = " + listexportjpgfact.at(i).at(0).toString();
            else                                                // c'est une facture, l'iduser est dans la table
                req = "select dep." CP_IDUSER_DEPENSES ", " CP_LOGIN_USR " from " TBL_DEPENSES " dep, " TBL_UTILISATEURS " usr"
                                                                                                              " where dep." CP_IDUSER_DEPENSES " = usr." CP_ID_USR
                                                                                                              " and " CP_ID_DEPENSES " = " + listexportjpgfact.at(i).at(5).toString();
            Listeusr = db->StandardSelectSQL(req, ok);
            if (Listeusr.size()==0) // il n'y a aucune depense enregistrée pour cette facture, on la détruit
            {
                db->SupprRecordFromTable(listexportjpgfact.at(i).at(0).toInt(), CP_ID_FACTURES, TBL_FACTURES);
                continue;
            }
            user = Listeusr.at(0).at(1).toString();
            if (listexportjpgfact.at(i).at(4).toInt()!=1)
                NomFileDoc += "-"+listexportjpgfact.at(i).at(5).toString();

            QString CheminOKTransfrDirImg  = CheminOKTransfrDir + "/" + user;
            if (!Utils::mkpath(CheminOKTransfrDirImg))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDirImg + "</b></font>" + tr(" invalide");
                ShowMessage::I()->SplashMessage(msg, 3000);
                return;
            }

            QString CheminOKTransfrDoc  = CheminOKTransfrDirImg + "/" + NomFileDoc + "." JPG;
            QString CheminOKTransfrProv = CheminOKTransfrDirImg + "/" + NomFileDoc + "prov." JPG;
            QByteArray ba = listexportjpgfact.at(i).at(6).toByteArray();
            QPixmap pix;
            pix.loadFromData(ba);
        /*!
         * On utilise le passage par les QPixmap parce que le mèthode suivante consistant
         * à réintégrer le QByteArray directement dans le fichier aboutit à un fichier corrompu et je ne sais pas pourquoi
         * QFile prov (CheminOKTransfrProv);
            if (prov.open(QIODevice::Append))
            {
                QTextStream out(&prov);
                out << ba;
            }
        */
            if (!pix.save(CheminOKTransfrProv, "jpeg"))
            {
                //qDebug() << "erreur";
                return;
            }
            if (!Utils::CompressFileJPG(CheminOKTransfrProv, db->dirimagerie()))
            {
                db->SupprRecordFromTable(listexportjpgfact.at(i).at(0).toInt(), CP_ID_FACTURES, TBL_FACTURES);
                continue;
            }
            QFile prov(CheminOKTransfrProv);
            if (prov.open(QIODevice::ReadWrite))
            {
                prov.copy(CheminOKTransfrDoc);
                prov.remove();
            }
            else
                return;
            db->StandardSQL("update " TBL_FACTURES " set " CP_JPG_FACTURES " = null, " CP_LIENFICHIER_FACTURES " = '/" + user + "/" + Utils::correctquoteSQL(NomFileDoc) + "." JPG "'"
                            " where " CP_ID_FACTURES " = " + listexportjpgfact.at(i).at(0).toString());
            faits ++;
            int nsec = debut.secsTo(QTime::currentTime());
            int min = nsec/60;
            int hour = min/60;
            min = min - (hour*60);
            nsec = nsec - (hour*3600) - (min*60);
            listmsg.clear();
            duree = QTime(hour,min,nsec).toString("HH:mm:ss");
            listmsg << "JPG - " + NomFileDoc + " - " + QString::number(faits) + "/" + QString::number(total) + " - "  + duree;
            QTime dieTime= QTime::currentTime().addMSecs(2);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            UpSystemTrayIcon::I()->showMessages(tr("Messages"),listmsg, Icons::icSunglasses(), 10);
        }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES PDF
    //-----------------------------------------------------------------------------------------------------------------------------------------
    reqpdf = "SELECT "
        CP_ID_FACTURES ", "
        CP_DATEFACTURE_FACTURES ", "
        CP_LIENFICHIER_FACTURES ", "
        CP_INTITULE_FACTURES ", "
        CP_ECHEANCIER_FACTURES ", "
        CP_IDDEPENSE_FACTURES ", "
        CP_PDF_FACTURES
             " FROM " TBL_FACTURES
             " where " CP_PDF_FACTURES " is not null";
    QList<QVariantList> listexportpdffact = db->StandardSelectSQL(reqpdf, m_ok );
    if (m_ok)
        for (int i=0; i<listexportpdffact.size(); i++)
        {
            if (listexportpdffact.at(i).at(2).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOM_DIR_FACTURES + listexportpdffact.at(i).at(2).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL("update " TBL_FACTURES " set " CP_PDF_FACTURES " = null where " CP_ID_FACTURES " = " + listexportpdffact.at(i).at(0).toString());
                    continue;
                }
            }
            QDate datetransfer  = listexportpdffact.at(i).at(1).toDate();
            QString user;

            QString NomFileDoc = listexportpdffact.at(i).at(0).toString() + "_"
                                 + (listexportpdffact.at(i).at(4).toInt()==1? ECHEANCIER : FACTURE) + "-"
                                 + listexportpdffact.at(i).at(3).toString().replace("/",".") + "_"
                                 + datetransfer.toString("yyyyMMdd");
            // on recherche le user à l'origine de cette facture
            QList<QVariantList> Listeusr;
            if (listexportpdffact.at(i).at(4).toInt()==1)          // c'est un échéancier
                req = "select dep." CP_IDUSER_DEPENSES ", " CP_LOGIN_USR " from " TBL_DEPENSES " dep, " TBL_UTILISATEURS " usr"
                      " where dep." CP_IDUSER_DEPENSES "  = usr." CP_ID_USR
                      " and " CP_IDFACTURE_DEPENSES " = " + listexportpdffact.at(i).at(0).toString();
            else                                                // c'est une facture, l'iduser est dans la table
                req = "select dep." CP_IDUSER_DEPENSES ", " CP_LOGIN_USR " from " TBL_DEPENSES " dep, " TBL_UTILISATEURS " usr"
                      " where dep." CP_IDUSER_DEPENSES "  = usr." CP_ID_USR
                      " and " CP_ID_DEPENSES " = " + listexportpdffact.at(i).at(5).toString();
            Listeusr = db->StandardSelectSQL(req, m_ok);
            if (Listeusr.size()==0) // il n'y a aucune depense enregistrée pour cette facture, on la détruit
            {
                db->SupprRecordFromTable(listexportpdffact.at(i).at(0).toInt(), CP_ID_FACTURES, TBL_FACTURES);
                continue;
            }
            user = Listeusr.at(0).at(1).toString();
            if (listexportpdffact.at(i).at(4).toInt()!=1)
                NomFileDoc += "-"+listexportpdffact.at(i).at(5).toString();

            QString CheminOKTransfrDirImg  = CheminOKTransfrDir + "/" + user;
            if (!QDir(CheminOKTransfrDirImg).exists())
                if (!Utils::mkpath(CheminOKTransfrDirImg))
                {
                    QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDirImg + "</b></font>" + tr(" invalide");
                    ShowMessage::I()->SplashMessage(msg, 3000);
                    return;
                }
            QString CheminOKTransfrDoc      = CheminOKTransfrDirImg + "/" + NomFileDoc + "." PDF;

            QByteArray bapdf;
            bapdf.append(listexportpdffact.at(i).at(6).toByteArray());

            QBuffer buf(&bapdf);
            buf.open(QIODevice::ReadWrite);
            QPdfDocument document;
            document.load(&buf);
            if( document.pageCount() > 0)
            {
                QFile file(CheminOKTransfrDoc);
                if (file.open(QIODevice::NewOnly))
                {
                    QDataStream out(&file);
                    out << bapdf;
                }
            } else {
                QStringList listmsg;
                listmsg << tr("Impossible de charger le document ") + NomFileDoc;
                UpSystemTrayIcon::I()->showMessages(tr("Messages"), listmsg, Icons::icSunglasses(), 3000);
                QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer.toString("yyyy-MM-dd") + ".txt";
                QFile   echectrsfer(echectrsfername);
                if (echectrsfer.open(QIODevice::Append))
                {
                    QTextStream out(&echectrsfer);
                    out << NomFileDoc << "\n" ;
                    echectrsfer.close();
                    QFile CD(CheminEchecTransfrDir + "/" + NomFileDoc);
                    if (CD.open(QIODevice::OpenModeFlag::NewOnly))
                    {
                        QDataStream out(&CD);
                        out << bapdf;
                    }
                }
                QString delreq = "delete from  " TBL_DOCSEXTERNES " where " CP_ID_DOCSEXTERNES " = " + listexportpdf.at(i).at(0).toString();
                //qDebug() << delreq;
                db->StandardSQL(delreq);
                continue;
            }

            QFile CC(CheminOKTransfrDoc);
            CC.open(QIODevice::ReadWrite);
            CC.setPermissions(QFileDevice::ReadOther
                              | QFileDevice::ReadGroup
                              | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                              | QFileDevice::ReadUser   | QFileDevice::WriteUser);
            CC.close();
            db->StandardSQL("update " TBL_FACTURES " set " CP_PDF_FACTURES " = null, " CP_LIENFICHIER_FACTURES " = '/" + user + "/" + Utils::correctquoteSQL(NomFileDoc)  + "." PDF "'"
                                                                                                                                                                           " where " CP_ID_FACTURES " = " + listexportpdffact.at(i).at(0).toString());
            faits ++;
            int nsec = debut.secsTo(QTime::currentTime());
            int min = nsec/60;
            int hour = min/60;
            min = min - (hour*60);
            nsec = nsec - (hour*3600) - (min*60);
            listmsg.clear();
            duree = QTime(hour,min,nsec).toString("HH:mm:ss");
            listmsg << "PDF - " + NomFileDoc + " - " + QString::number(faits) + "/" + QString::number(total) + " - "  + duree;
            QTime dieTime= QTime::currentTime().addMSecs(2);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            UpSystemTrayIcon::I()->showMessages(tr("Messages"), listmsg, Icons::icSunglasses(), 10);
        }
    int totfac = listexportjpgfact.size() + listexportpdffact.size();
    if (totfac > 0)
    {
        listmsg <<  tr("export terminé") << QString::number(totfac) + (totfac>1? tr(" documents comptables exportés en ") :tr(" document comptable exporté en ")) + duree;
        UpSystemTrayIcon::I()->showMessages(tr("Messages"), listmsg, Icons::icSunglasses(), 3000);
    }
}

void RufusAdmin::GestionBanques()
{
    DisconnectTimerInactive();
    dlg_gestionbanques *Dlg_Banq = new dlg_gestionbanques(this);
    Dlg_Banq->exec();
    delete Dlg_Banq;
    ConnectTimerInactive();
}

void RufusAdmin::GestionLieux()
{
    DisconnectTimerInactive();
    dlg_listelieux *gestLieux = new dlg_listelieux(this);
    gestLieux->exec();
    delete gestLieux;
    ReconstruitListeLieuxExercice();
    ConnectTimerInactive();

}

void RufusAdmin::GestionMotifs()
{
    DisconnectTimerInactive();
    dlg_motifs *Dlg_motifs = new dlg_motifs(this);
    Dlg_motifs->setWindowTitle(tr("Motifs de consultations"));
    Dlg_motifs->exec();
    delete Dlg_motifs;
    ConnectTimerInactive();
}

void RufusAdmin::GestionUsers()
{
    DisconnectTimerInactive();
    dlg_gestionusers *Dlg_GestUsr = new dlg_gestionusers(ui->EmplacementServeurupComboBox->currentData().toInt(), dlg_gestionusers::ADMIN, true, this);
    Dlg_GestUsr->setWindowModality(Qt::WindowModal);
    if(Dlg_GestUsr->exec() == QDialog::Accepted)
    {
        Datas::I()->users->initListe();
        EnvoieTCPMessage(TOUS, TCPMSG_MAJListeUsers);
        UpMessageBox::Watch(this, tr("Donnes utilisateurs modifiées?"),
                                  tr("Si vous avez modifié des données d'utilisateurs actuellement connectés,\n"
                                     "chacun de ces utilisateurs doit relancer le programme\n"
                                     "pour pouvoir prendre en compte les modifications apportées!"));
    }
    delete Dlg_GestUsr;
    ConnectTimerInactive();
}

void RufusAdmin::MasqueAppli()
{
    setEnabled(false);
    foreach (QDialog* d , findChildren<QDialog*>())
        d->hide();
    hide();
}

void RufusAdmin::MetAJourLaConnexion()
{
    QString macadress =  Utils::MACAdress() + " - " + NOM_ADMINISTRATEUR;
    QString MAJConnexionRequete;

    //! le poste remet à jour sa propre connexion
    QString req = "select " CP_IDUSER_USRCONNECT " from " TBL_USERSCONNECTES
            " where " CP_MACADRESS_USRCONNECT " = '" + macadress + "'"
            " and " CP_IDLIEU_USRCONNECT " = " + QString::number(Datas::I()->sites->idcurrentsite());
    QList<QVariantList> listusers = db->StandardSelectSQL(req,m_ok);
    if (listusers.size()>0)
    {
        MAJConnexionRequete = "UPDATE " TBL_USERSCONNECTES " SET " CP_HEUREDERNIERECONNECTION_USRCONNECT " = NOW(), "
                                                                 CP_IDUSER_USRCONNECT " = " + QString::number(Admin()->id()) + ","
                                                                 CP_NOMPOSTE_USRCONNECT " = '" + QHostInfo::localHostName().left(60) + "'"
                                                                 " where " CP_MACADRESS_USRCONNECT " = '" + macadress + "'"
                                                                 " and " CP_IDLIEU_USRCONNECT " = " + QString::number(Datas::I()->sites->idcurrentsite());
        db->StandardSQL(MAJConnexionRequete);
    }
    else
        Datas::I()->postesconnectes->CreationPosteConnecte(Admin(), Datas::I()->sites->idcurrentsite());


    //! Deconnecter les users débranchés accidentellement
    /*! Cette fonction sert à déconnecter et lever les verrous d'un utilisateur qui se serait déconnecté accidentellement
     * elle est utilisée pour les postes distants - pour les autres, la détection se fait par la rupture du tcp
     on fait la liste des utilisateurs qui n'ont pas remis à jour leur connexion depuis plus de 60 secondes,
     on retire les verrous qu'ils auraient pu poser et on les déconnecte */
    Datas::I()->postesconnectes->initListe();
    Datas::I()->patientsencours->initListeAll();
    QDateTime timenow = db->ServerDateTime();
    QList<PosteConnecte*> listpostsAEliminer = QList<PosteConnecte*>();
    foreach(PosteConnecte* post, *Datas::I()->postesconnectes->postesconnectes())
    {
        qint64 tempsecouledepuisactualisation = post->dateheurederniereconnexion().secsTo(timenow);
        if (tempsecouledepuisactualisation > 90)
        {
            qDebug() << "Suppression d'un poste débranché accidentellement" << "RufusAdmin::MetAJourLaConnexion()";
            qDebug() << "nom du poste" << post->nomposte();
            qDebug() << "stringid du poste" << post->stringid();
            qDebug() << "timenow = " << timenow;
            qDebug() << "heure dernière connexion = " << post->dateheurederniereconnexion();
            qDebug() << "temps ecoule depuis actualisation = " << tempsecouledepuisactualisation;
            qDebug() << "user = " << (Datas::I()->users->getById(post->id())? Datas::I()->users->getById(post->id())->login() : tr("inconnu"));
            //! l'utilisateur n'a pas remis sa connexion à jour depuis plus de 120 secondes
            //! on déverrouille les dossiers verrouillés par cet utilisateur et on les remet en salle d'attente
            QString blabla              = ENCOURSEXAMEN;
            int length                  = blabla.size();
            for (auto it = Datas::I()->patientsencours->patientsencours()->constBegin(); it != Datas::I()->patientsencours->patientsencours()->constEnd(); ++it)
            {
                PatientEnCours *pat = const_cast<PatientEnCours*>(it.value());
                if (pat != Q_NULLPTR)
                    if (pat->iduserencoursexam() == post->id() && pat->statut().left(length) == ENCOURSEXAMEN && pat->posteexamen() == post->nomposte())
                    {
                        ItemsList::update(pat, CP_STATUT_SALDAT, ARRIVE);
                        ItemsList::update(pat, CP_POSTEEXAMEN_SALDAT);
                        ItemsList::update(pat, CP_IDUSERENCOURSEXAM_SALDAT);
                    }
            }
            if (!listpostsAEliminer.contains(post))
                listpostsAEliminer << post;
        }
    }
    if (listpostsAEliminer.size() > 0)
    {
       foreach (PosteConnecte* post, listpostsAEliminer)
       {
          QString nomposte = (post->isadmin()? tr("administrateur") + " " : "") + post->nomposte();
          Datas::I()->postesconnectes->SupprimePosteConnecte(post);
          UpSystemTrayIcon::I()->showMessage(tr("Messages"), tr("Le poste ") + nomposte + tr(" a été retiré de la liste des postes connectés actuellement au serveur"), Icons::icSunglasses(), 3000);
       }
       if (m_utiliseTCP)
           if (TCPServer->islaunched())
               TCPServer->envoieListeSockets();
    }

    if (Datas::I()->patientsencours->patientsencours()->size() == 0)
        return;
    bool mettreajourlasalledattente = false;

    // on redonne le statut ARRIVE aux patients dont l'utilisateur s'est déconnecté
    for (auto it = Datas::I()->patientsencours->patientsencours()->constBegin(); it != Datas::I()->patientsencours->patientsencours()->constEnd(); ++it)
    {
        PatientEnCours *pat = const_cast<PatientEnCours*>(it.value());
        if (pat != Q_NULLPTR)
            if (pat->statut().contains(ENCOURSEXAMEN))
            {
                bool posttrouve = false;
                foreach(PosteConnecte* post, *Datas::I()->postesconnectes->postesconnectes())
                    if (post->id() == pat->iduserencoursexam() && post->nomposte() == pat->posteexamen())
                    {
                        posttrouve = true;
                        break;
                    }
                if (!posttrouve)
                {
                    ItemsList::update(pat, CP_STATUT_SALDAT, ARRIVE);
                    ItemsList::update(pat, CP_POSTEEXAMEN_SALDAT);
                    ItemsList::update(pat, CP_IDUSERENCOURSEXAM_SALDAT);
                    mettreajourlasalledattente = true;
                }
            }
    }

    // on retire de la salle d'attente les patients qui n'existent pas
    req = "delete from " TBL_SALLEDATTENTE " where idpat not in (select idpat from " TBL_PATIENTS ")";
    db->StandardSQL(req);
    QVariantList ndel = db->getFirstRecordFromStandardSelectSQL("SELECT ROW_COUNT() as DelRowCount;", m_ok);
    if (m_ok)
        if (ndel.size() > 0 && ndel.at(0).toInt() > 0)
        {
            Datas::I()->patientsencours->initListeAll();
            mettreajourlasalledattente = true;
        }

    if (mettreajourlasalledattente)
    {
        flags->MAJFlagSalleDAttente();
        EnvoieTCPMessage(TOUS, TCPMSG_MAJSalAttente);
    }
}

void RufusAdmin::ModifMDP()
{
    DisconnectTimerInactive();
    dlg_askMDP    = new UpDialog(this);
    dlg_askMDP    ->setWindowModality(Qt::WindowModal);
    QRegularExpression  rxMdp           = QRegularExpression("^[a-zA-Z0-9]{3,15}$");

    UpLineEdit *ConfirmMDP = new UpLineEdit(dlg_askMDP);
    ConfirmMDP->setEchoMode(QLineEdit::Password);
    ConfirmMDP->setObjectName(m_confirmMDP);
    ConfirmMDP->setValidator(new QRegularExpressionValidator(rxMdp,this));
    ConfirmMDP->setAlignment(Qt::AlignCenter);
    dlg_askMDP->dlglayout()->insertWidget(0,ConfirmMDP);

    UpLabel *labelConfirmMDP = new UpLabel();
    labelConfirmMDP->setText(tr("Confirmez le nouveau mot de passe"));
    dlg_askMDP->dlglayout()->insertWidget(0,labelConfirmMDP);

    UpLineEdit *NouvMDP = new UpLineEdit(dlg_askMDP);
    NouvMDP->setEchoMode(QLineEdit::Password);
    NouvMDP->setObjectName(m_nouvMDP);
    NouvMDP->setValidator(new QRegularExpressionValidator(rxMdp,this));
    NouvMDP->setAlignment(Qt::AlignCenter);
    dlg_askMDP->dlglayout()->insertWidget(0,NouvMDP);

    UpLabel *labelNewMDP = new UpLabel();
    labelNewMDP->setText(tr("Entrez le nouveau mot de passe"));
    dlg_askMDP->dlglayout()->insertWidget(0,labelNewMDP);

    UpLineEdit *AncMDP = new UpLineEdit(dlg_askMDP);
    AncMDP->setEchoMode(QLineEdit::Password);
    AncMDP->setAlignment(Qt::AlignCenter);
    AncMDP->setValidator(new QRegularExpressionValidator(rxMdp,this));
    AncMDP->setObjectName(m_ancMDP);
    dlg_askMDP->dlglayout()->insertWidget(0,AncMDP);

    UpLabel *labelOldMDP = new UpLabel();
    labelOldMDP->setText(tr("Ancien mot de passe"));
    dlg_askMDP->dlglayout()->insertWidget(0,labelOldMDP);
    AncMDP->setFocus();

    dlg_askMDP->AjouteLayButtons(UpDialog::ButtonCancel | UpDialog::ButtonOK);
    QList <QWidget*> ListTab;
    ListTab << AncMDP << NouvMDP << ConfirmMDP << dlg_askMDP->OKButton;
    for (int i = 0; i<ListTab.size()-1 ; i++ )
        dlg_askMDP->setTabOrder(ListTab.at(i), ListTab.at(i+1));
    dlg_askMDP    ->setWindowTitle(tr("Mot de passe utilisateur"));
    connect(dlg_askMDP->OKButton,    &QPushButton::clicked, this, &RufusAdmin::EnregistreNouvMDPAdmin);
    dlg_askMDP->dlglayout()->setSizeConstraint(QLayout::SetFixedSize);

    dlg_askMDP->exec();
    delete dlg_askMDP;
    dlg_askMDP = Q_NULLPTR;
    ConnectTimerInactive();
}

void RufusAdmin::RestaureBase()
{
    if (AutresPostesConnectes())
        return;

    DisconnectTimers();

    /*! 1 - choix du dossier où se situe la sauvegarde */
    UpMessageBox::Information(this, tr("Choix du dossier de sauvegarde"),
                              tr("Dans la fiche suivante, choisissez le dossier "
                              "contenant la sauvegarde de la base.\n\n"
                              "Une fois le dossier sélectionné, "
                              "la sauvegarde commencera automatiquement.\n"
                              "Ce processus est long et peut durer plusieurs minutes.\n"
                              "(environ 1' pour 2 Go)\n"));
    QString dir = PATH_DIR_RUFUS;
    QUrl url = Utils::getExistingDirectoryUrl(this, tr("Restaurer à partir du dossier"), QUrl::fromLocalFile(dir));
    if (url == QUrl())
    {
        ConnectTimers();
        return;
    }
    QDir dirtorestore = url.path();

    /*! --------------------------------------------------------------------------------------------------------------------------------------------------------
    * Restauration ---------------------------------------------------------------------------------------------------------------------------------------------
        * de la base de données --------------------------------------------------------------------------------------------------------------------------------
        * des fichiers de ressources ---------------------------------------------------------------------------------------------------------------------------
        * de Rufus.ini -----------------------------------------------------------------------------------------------------------------------------------------
        * du dossier d'imagerie --------------------------------------------------------------------------------------------------------------------------------
        * des videos -------------------------------------------------------------------------------------------------------------------------------------------
    * -------------------------------------------------------------------------------------------------------------------------------------------------------*/
    bool OKini      = false;
    bool OKImages   = false;
    bool OKVideos   = false;
    bool OKFactures = false;

    QString msg;

    /*! 2 - détermination des éléments pouvant être restaurés */
    if (QFile(dirtorestore.absolutePath() + NOM_ADMINFILE_INI).exists())
        OKini = true;
    QDir rootimg = dirtorestore;
     if (rootimg.cdUp())
    {
        if (QDir(rootimg.absolutePath() + NOM_DIR_IMAGES).exists())
            if (QDir(rootimg.absolutePath() + NOM_DIR_IMAGES).entryList(QDir::Dirs).size()>0)
                OKImages = true;
        if (QDir(rootimg.absolutePath() + NOM_DIR_VIDEOS).exists())
            if (QDir(rootimg.absolutePath() + NOM_DIR_VIDEOS).entryList(QDir::Files | QDir::NoDotAndDotDot).size()>0)
                OKVideos = true;
        if (QDir(rootimg.absolutePath() + NOM_DIR_FACTURES).exists())
            if (QDir(rootimg.absolutePath() + NOM_DIR_FACTURES).entryList(QDir::Dirs | QDir::NoDotAndDotDot).size()>0)
                OKFactures = true;
    }
    /*! 3 - détermination de l'emplacement de destination des fichiers d'imagerie */
    if (db->dirimagerie() == QString())
    {
        ConnectTimers();
        return;
    }
    if (!QDir(db->dirimagerie()).exists())
        if (!Utils::mkpath(db->dirimagerie()))
        {
            ConnectTimers();
            return;
        }

    /*! 4 - choix des éléments à restaurer */
    AskBupRestore(RestoreOp, dirtorestore.absolutePath(), db->dirimagerie(), OKini, OKImages, OKVideos, OKFactures);
    if (dlg_buprestore->exec() == QDialog::Accepted)
    {
        bool okrestorebase = false;
        QFileDevice::Permissions permissions = QFileDevice::ReadOther  | QFileDevice::WriteOther
                                               | QFileDevice::ReadGroup  | QFileDevice::WriteGroup
                                               | QFileDevice::ReadOwner  | QFileDevice::WriteOwner | QFileDevice::ExeOwner;
        foreach (UpCheckBox *chk, dlg_buprestore->findChildren<UpCheckBox*>())
        {
            /*! 4a - restauration de la base de données tout d'abord */
            if (chk->objectName() == "base")
            {
                if (chk->isChecked())
                {
                    UpMessageBox msgbox(this);
                    QStringList listnomsfilestorestore;
                    UpSmallButton *AnnulBouton  = new UpSmallButton();
                    UpSmallButton *OKBouton     = new UpSmallButton;
                    msgbox.setIcon(UpMessageBox::Warning);

                    msgbox.setText(tr("Attention"));
                    msgbox.setInformativeText(tr("Vous avez choisi de restaurer la base à partir du dosssier") + "\n"
                                              + dirtorestore.absolutePath() + ".\n" +
                                              tr("Si une base de données Rufus existe sur ce serveur, "
                                                 "elle sera définitivement effacée pour être remplacée par cette sauvegarde.\n"
                                                 "Confirmez-vous la suppression des anciennes données?"));
                    AnnulBouton->setText(tr("Annuler"));
                    OKBouton->setText("J'ai compris\nJe confirme");
                    msgbox.addButton(AnnulBouton, UpSmallButton::CANCELBUTTON);
                    msgbox.addButton(OKBouton, UpSmallButton::STARTBUTTON);
                    msgbox.exec();
                    if (msgbox.clickedButton() != OKBouton)
                    {
                        msg += tr("Base non restaurée");
                        break;
                    }
                    QString mdp ("");
                    if (!Utils::VerifMDP(db->getMDPAdmin(),tr("Saisissez le mot de passe Administrateur"), mdp))
                    {
                        msg += tr("Base non restaurée");
                        break;
                    }
                    bool echecfile = true;
                    qintptr handledlg = 0;
                    ShowMessage::I()->PriorityMessage(tr("Restauration de la base en cours"),handledlg);
                    QStringList filters;
                    filters << "*.sql";
                    QStringList listfichiers = dirtorestore.entryList(filters);
                    listfichiers.removeAt(listfichiers.indexOf("user.sql"));
                    for (int j=0; j<listfichiers.size(); j++)
                        listnomsfilestorestore << dirtorestore.absolutePath() + "/" + listfichiers.at(j);
                    for (int i=0; i<listnomsfilestorestore.size(); i++)
                    {
                        if (!QFile(listnomsfilestorestore.at(i)).open(QIODevice::ReadOnly))
                            msg = tr("Echec de la restauration") + "\n" + tr("Le fichier ") + listnomsfilestorestore.at(i) + tr(" n'a pas été trouvé!");
                        else if (QFile(listnomsfilestorestore.at(i)).size() == 0)
                            msg = tr("Echec de la restauration") + "\n" + tr("Le fichier ") + listnomsfilestorestore.at(i) + tr(" est vide!");
                        else if (listnomsfilestorestore.at(i).contains(" "))
                            msg = tr("Echec de la restauration") + "\n" + tr("Le chemin vers le fichier ") + listnomsfilestorestore.at(i) + tr(" contient des espaces!");
                        else echecfile = false;
                        if (echecfile)
                        {
                            msg += tr("Base non restaurée") + "\n" + msg;
                            break;
                        }
                    }
                    if (!echecfile)
                    {
                        //! Suppression de toutes les tables
                        QString Msg = tr("Suppression de l'ancienne base Rufus en cours");
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                        db->VideDatabases();

                        //! Restauration à partir du dossier sélectionné
                        ExecuteSQLScript(listnomsfilestorestore);
                        msg += tr("Base de données Rufus restaurée\n");
                    }
                    ShowMessage::I()->ClosePriorityMessage(handledlg);
                }
            }
        }
        foreach (UpCheckBox *chk, dlg_buprestore->findChildren<UpCheckBox*>())
        {
            /*! 4b - restauration du fichier ini */
            if (chk->objectName() == "ini")
            {
                if (chk->isChecked())
                {
                    QString fileini = dirtorestore.absolutePath() + NOM_ADMINFILE_INI;
                    QFile FichierIni(PATH_FILE_INI);
                    if (FichierIni.exists())
                        FichierIni.remove();
                    QFile rufusini(fileini);
                    rufusini.copy(PATH_FILE_INI);
                    msg += tr("Fichier de paramétrage Rufus.ini restauré\n");
                    UpSystemTrayIcon::I()->showMessage(tr("Messages"), tr("Fichier de paramétrage Rufus.ini restauré"), Icons::icSunglasses(), 3000);
                }
            }
            /*! 4c - restauration des images */
            else if (chk->objectName() == "images")
            {
                if (chk->isChecked())
                {
                    QString dirdestinationimg   = db->dirimagerie() + NOM_DIR_IMAGES;
                    QDir DirDestImg(dirdestinationimg);
                    if (DirDestImg.exists())
                        DirDestImg.removeRecursively();
                    DirDestImg.mkdir(dirdestinationimg);
                    if (!DirDestImg.exists())
                    {
                        QString Msg = tr("le dossier de destination de l'imagerie n'existe pas");
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                    }
                    else
                    {
                        QString Msg = (tr("Restauration des fichiers d'imagerie\n")
                                       + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base d'images"));
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                        QString dirrestaureimagerie    = rootimg.absolutePath() + NOM_DIR_IMAGES;
                        int t = 0;
                        Utils::countFilesInDirRecursively(dirrestaureimagerie, t);
                        UpProgressDialog *progdial = new UpProgressDialog(0,0,this);
                        QLabel *label = new QLabel();
                        label->setAlignment(Qt::AlignLeft);
                        progdial->setLabel(label);
                        QProgressBar *bar = new QProgressBar();
                        progdial->setBar(bar);
                        progdial->setRange(0,t);
                        progdial->setCancelButton(Q_NULLPTR);
                        progdial->show();
                        progdial->setWindowModality(Qt::WindowModal);
                        int n = 0;
                        Utils::copyfolderrecursively(dirrestaureimagerie, dirdestinationimg, n, tr("Restauration des fichiers d'imagerie"), progdial, permissions);
                        delete progdial;
                        msg += tr("Fichiers d'imagerie restaurés\n");                    }
                }
            }
            /*! 4d - restauration factures */
            else if (chk->objectName() == "factures")
            {
                if (chk->isChecked())
                {
                    QString dirdestinationfact  = db->dirimagerie() + NOM_DIR_FACTURES;
                    QDir DirDestFact(dirdestinationfact);
                    if (DirDestFact.exists())
                        DirDestFact.removeRecursively();
                    DirDestFact.mkdir(dirdestinationfact);
                    if (!DirDestFact.exists())
                    {
                        QString Msg = tr("le dossier de destination des factures n'existe pas");
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                    }
                    else
                    {
                        QString Msg = (tr("Restauration des factures\n")
                                       + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de factures"));
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                        QString dirrestaurefactures    = rootimg.absolutePath() + NOM_DIR_FACTURES;
                        int t = 0;
                        Utils::countFilesInDirRecursively(dirrestaurefactures, t);
                        UpProgressDialog *progdial = new UpProgressDialog(0,0,this);
                        QLabel *label = new QLabel();
                        label->setAlignment(Qt::AlignLeft);
                        progdial->setLabel(label);
                        QProgressBar *bar = new QProgressBar();
                        progdial->setBar(bar);
                        progdial->setRange(0,t);
                        progdial->setCancelButton(Q_NULLPTR);
                        progdial->show();
                        progdial->setWindowModality(Qt::WindowModal);
                        int n = 0;
                        Utils::copyfolderrecursively(dirrestaurefactures, dirdestinationfact, n, tr("Restauration des factures"), progdial);
                        delete progdial;
                        msg += tr("Fichiers factures restaurés\n");                    }
                }
            }
            /*! 4e - restauration des videos */
            if (chk->objectName() == "videos")
            {
                if (chk->isChecked())
                {
                    QString dirdestinationvid   =  db->dirimagerie() + NOM_DIR_VIDEOS;
                    QDir DirDestVid(dirdestinationvid);
                    if (DirDestVid.exists())
                        DirDestVid.removeRecursively();
                    DirDestVid.mkdir(dirdestinationvid);
                    if (!DirDestVid.exists())
                    {
                        QString Msg = tr("le dossier de destination des videos n'existe pas");
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                    }
                    else
                    {
                        QString Msg = (tr("Restauration des fichiers videos\n")
                                       + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
                        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
                        QString dirrestaurevideo = rootimg.absolutePath() + NOM_DIR_VIDEOS;
                        int t = 0;
                        Utils::countFilesInDirRecursively(dirrestaurevideo, t);
                        UpProgressDialog *progdial = new UpProgressDialog(0,0,this);
                        QLabel *label = new QLabel();
                        label->setAlignment(Qt::AlignLeft);
                        progdial->setLabel(label);
                        QProgressBar *bar = new QProgressBar();
                        progdial->setBar(bar);
                        progdial->setRange(0,t);
                        progdial->setCancelButton(Q_NULLPTR);
                        progdial->show();
                        progdial->setWindowModality(Qt::WindowModal);
                        int n = 0;
                        Utils::copyfolderrecursively(dirrestaurevideo, dirdestinationvid, n, tr("Sauvegarde des videos"), progdial, permissions);
                        progdial->close();
                        delete progdial;
                        msg += tr("Fichiers videos restaurés\n");
                    }
                }
            }
        }
        //qDebug() << msg;
        UpMessageBox::Watch(this,tr("restauration terminée"),msg);
        if (okrestorebase)
        {
            UpMessageBox::Watch(this,tr("Fermeture du programme"));
            exit(0);
        }
    }
    delete dlg_buprestore;
    dlg_buprestore = Q_NULLPTR;
    ConnectTimers();
}

/*!
 * utilisé par RufusAdmin pour supprimer les documents et les factures
 * ayant été inscrits dans les tables DocsASupprimer et FacturesASupprimer
 * par les autres postes
 */

void RufusAdmin::SupprimerDocsEtFactures()
{
    bool ok = true;
    QString NomDirStockageImagerie = db->dirimagerie();

    /* Supprimer les documents en attente de suppression*/
    QString req = "Select " CP_FILEPATH_DOCSASUPPR " from " TBL_DOCSASUPPRIMER;
    QList<QVariantList> ListeDocs = db->StandardSelectSQL(req, ok);
    for (int i=0; i<ListeDocs.size(); i++)
    {
        QString CheminFichier = NomDirStockageImagerie + ListeDocs.at(i).at(0).toString();
        if (!QFile(CheminFichier).remove())
            UpMessageBox::Watch(this, tr("Fichier introuvable!"), CheminFichier);
        db->StandardSQL("delete from " TBL_DOCSASUPPRIMER " where " CP_FILEPATH_DOCSASUPPR " = '" + Utils::correctquoteSQL(ListeDocs.at(i).at(0).toString()) + "'");
    }

    /* Supprimer les factures en attente de suppression - même démarche mais on fait une copie de la facture dans le dossier FACTURESSANSLIEN avant de la supprimer*/
    QString CheminOKTransfrDir = NomDirStockageImagerie + NOM_DIR_FACTURESSANSLIEN;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        ShowMessage::I()->SplashMessage(msg, 3000);
        return;
    }
    req = "select " CP_LIENFICHIER_FACTASUPPR " from " TBL_FACTURESASUPPRIMER;
    QList<QVariantList> ListeFactures = db->StandardSelectSQL(req, ok);
    for (int i=0; i<ListeFactures.size(); i++)
    {
        QString lienfichier = ListeFactures.at(i).at(0).toString();
        /*  on copie le fichier dans le dossier facturessanslien*/
        QString user = lienfichier.split("/").at(1);
        CheminOKTransfrDir = CheminOKTransfrDir + "/" + user;
        if (!Utils::mkpath(CheminOKTransfrDir))
        {
            QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
            ShowMessage::I()->SplashMessage(msg, 3000);
            continue;
        }
        QFile(NomDirStockageImagerie + NOM_DIR_FACTURES + lienfichier).copy(NomDirStockageImagerie + NOM_DIR_FACTURESSANSLIEN + lienfichier);
        /*  on l'efface du dossier de factures*/
        QFile(NomDirStockageImagerie + NOM_DIR_FACTURES + lienfichier).remove();
        /* on détruit l'enregistrement dans la table FacturesASupprimer*/
        db->StandardSQL("delete from " TBL_FACTURESASUPPRIMER " where " CP_LIENFICHIER_FACTASUPPR " = '" + Utils::correctquoteSQL(lienfichier) + "'");
    }
}

void RufusAdmin::ChoixMenuSystemTray(QString txt)
{
    //! il faut montrer la fiche d'abord sinon la fermeture du QInputDialog de VerifMDP()
    //! provoque la fermeture du programme quoiqu'il arrive (???)
    bool visible = isVisible();
    if (!visible)
        showNormal();
    QString mdp ("");
    if (!Utils::VerifMDP(db->getMDPAdmin(),tr("Saisissez le mot de passe Administrateur"),mdp))
    {
        if (!visible)
            MasqueAppli();
        return;
    }
    if (txt == tr("Ouvir RufusAdmin"))
        setEnabled(true);
    else if (txt == tr("Quitter RufusAdmin"))
        SortieAppli();
}

void RufusAdmin::TrayIconMenu()
{
    trayIconMenu->clear();
    if (! isVisible())
    {
        QString txt = tr("Ouvir RufusAdmin");
        QAction *pAction_VoirAppli = trayIconMenu->addAction(txt);
        connect (pAction_VoirAppli, &QAction::triggered, this, [=] {ChoixMenuSystemTray(txt);});
    }
    QString txt = tr("Quitter RufusAdmin");
    QAction *pAction_QuitAppli = trayIconMenu->addAction(txt);
    connect (pAction_QuitAppli, &QAction::triggered, this, [=] {ChoixMenuSystemTray(txt);});
}

void RufusAdmin::VerifPosteImport()
{
    if (db->ModeAccesDataBase() == Utils::Distant)
        return;

    //On recherche si le poste défini comme importateur des docs externes n'est pas celui sur lequel s'éxécute cette session de RufusAdmin et on prend sa place dans ce cas
    QString A, PostImport;    // l'importateur des docs externes
    QString rep = "";
    QString req = "";
    bool isMysql8 = false;
    if (db->version().split(".").size() > 0)
        isMysql8 = (db->version().split(".").at(0).toInt() == 8);
    //qDebug() << "Mysql = " << db->version() << " - Mysql version = " << db->version().split(".").at(0).toInt();

    /*! Il n'y pas de variables utilisateur globale dans MySQL, on est donc obligé de passer par une procédure stockée pour en simuler une
     * pour créer une procédure avec Qt, séparer le drop du create, ne pas utiliser les délimiteurs et utiliser les retours à la ligne \n\.......
     * if (gsettingsIni->value(Utils::getBaseFromMode(Utils::ReseauLocal) + PrioritaireGestionDocs).toString() ==  "YES")

     * si a = true, on se met en poste importateur +/_ prioritaire à la fin suivant le contenu de rufus.ini
     * si a = false, on retire le poste en cours et on met NULL à la place. */

    if (isMysql8)
        req = "SELECT ROUTINE_SCHEMA, ROUTINE_NAME FROM information_schema.routines WHERE ROUTINE_SCHEMA = '" DB_RUFUS "' AND ROUTINE_NAME = '" MYSQL_PROC_POSTEIMPORTDOCS "'";
    else
        req = "SELECT name FROM mysql.proc p WHERE db = '" DB_RUFUS "' AND name = '" MYSQL_PROC_POSTEIMPORTDOCS "'";
    QVariantList imptdata = db->getFirstRecordFromStandardSelectSQL(req, m_ok);
    if (m_ok && imptdata.size()>0)
    {
        QVariantList procdata = db->getFirstRecordFromStandardSelectSQL("CALL " DB_RUFUS "." MYSQL_PROC_POSTEIMPORTDOCS, m_ok);
        if(!m_ok || procdata.size()==0)
        {
            ui->PosteImportDocslabel->setText(tr("Pas de poste paramétré"));
            ui->PosteImportDocsPrioritairelabel->setText("");
        }
        else
        {
            //qDebug() << "nbre reponses = " + QString::number(quer.size()) << NOM_POSTEIMPORTDOCS " = " + quer.value(0).toString();
            PostImport = procdata.at(0).toString();
            A = PostImport;
            A = "<font color=\"green\"><b>" + A.remove(".local") + "</b></font>";
            QString B;
            if (A.contains(" - " NOM_ADMINISTRATEUR))
                B = tr("Administrateur");
            else
                B = (A.contains(" - prioritaire")? tr("prioritaire") : tr("non prioritaire"));
            A.remove(" - prioritaire");
            A.remove(" - " NOM_ADMINISTRATEUR);
            ui->PosteImportDocslabel->setText(A);

            if (B == tr("non prioritaire"))
                B = "<font color=\"red\"><b>" + B + "</b></font>";
            else
                B = "<font color=\"green\"><b>" + B + "</b></font>";
            ui->PosteImportDocsPrioritairelabel->setText(B);
        }
    }
    QString macAdr = Utils::MACAdress() + " - " NOM_ADMINISTRATEUR;
    if (PostImport != "NULL" && PostImport != macAdr)
        setPosteImportDocs();
}

void RufusAdmin::VerifVersionBase()
{
    int version = m_parametres->versionbase();
    if (version != VERSION_BASE)
    {
        QString msg =
                tr("Vous utilisez une version de RufusAdmin prévue\npour être utilisée avec la version de la base Rufus n°") + QString::number(VERSION_BASE)+ "\n" +
                tr("La version de la base Rufus en cours d'utilisation est la n° ") + QString::number(version) + "\n" +
                tr("Faites une mise à jour de Rufus et Rufus Admin pour qu'ils utilisent les dernières versions de la base") + "\n\n-> " +
                tr("Dans le cas contraire, des dysfonctionnements peuvent survenir");
        qintptr z = 0;
        ShowMessage::I()->PriorityMessage(msg, z, 5000);
    }
}

void RufusAdmin::ReconstruitListeLieuxExercice()
{
    disconnect(ui->EmplacementServeurupComboBox,    QOverload<int>::of(&QComboBox::currentIndexChanged),   this,   &RufusAdmin::EnregistreEmplacementServeur);
    ui->EmplacementServeurupComboBox->clear();
    foreach (Site* site, *Datas::I()->sites->sites())
    {
        ui->EmplacementServeurupComboBox->addItem(site->nom(), site->id());
        if (m_parametres->idlieupardefaut() > 0)
            ui->EmplacementServeurupComboBox->setCurrentIndex(ui->EmplacementServeurupComboBox->findData(m_parametres->idlieupardefaut()));
        else
        {
            ui->EmplacementServeurupComboBox->setCurrentIndex(0);
            EnregistreEmplacementServeur(0);
        }
    }
    connect(ui->EmplacementServeurupComboBox,       QOverload<int>::of(&QComboBox::currentIndexChanged),   this,   &RufusAdmin::EnregistreEmplacementServeur);
}

bool RufusAdmin::VerifBase()
{
    int Versionencours  = 37; //correspond aux premières versions de MAJ de la base
    int Version         = VERSION_BASE;
    Versionencours = m_parametres->versionbase();
    bool BupDone = false;
    if (Versionencours < Version)
    {
        int nbreMAJ = Version - Versionencours;
        for (int i=1; i< nbreMAJ+1; i++)
        {
            Version = Versionencours + i;
            if (!BupDone)
            {
                UpMessageBox msgbox(this);
                msgbox.setText(tr("Mise à jour de la base nécessaire"));
                msgbox.setInformativeText(tr("Pour éxécuter cette version de Rufus, la base de données doit être mise à jour"
                                          "\net une sauvegarde de la base actuelle est fortement conseillée"));
                msgbox.setIcon(UpMessageBox::Warning);
                UpSmallButton *OKBouton = new UpSmallButton();
                UpSmallButton *BackupBouton = new UpSmallButton();
                UpSmallButton *ExitBouton = new UpSmallButton();
                OKBouton->setText(tr("Pousuivre, la sauvegarde a été faite"));
                BackupBouton->setText(tr("OK, je vais sauvegarder la base d'abord"));
                ExitBouton->setText(tr("Annuler et fermer"));
                msgbox.addButton(ExitBouton, UpSmallButton::CLOSEBUTTON);
                msgbox.addButton(BackupBouton, UpSmallButton::CANCELBUTTON);
                msgbox.addButton(OKBouton, UpSmallButton::STARTBUTTON);
                msgbox.exec();
                if (msgbox.clickedButton() == BackupBouton)
                {
                    if (!ImmediateBackup())
                        return false;
                }
                else if (msgbox.clickedButton() == ExitBouton)
                    return false;
                BupDone = true;
            }
            ShowMessage::I()->SplashMessage(tr("Mise à jour de la base vers la version ") + "<font color=\"red\"><b>" + QString::number(Version) + "</b></font>", 5000);
            QString Nomfic = "://majbase" + QString::number(Version) + ".sql";
            QFile DumpFile(Nomfic);
            int a = 99;
            if (DumpFile.exists())
            {
                QString NomDumpFile = PATH_DIR_RESSOURCES "/majbase" + QString::number(Version) + ".sql";
                QFile::remove(NomDumpFile);
                DumpFile.copy(NomDumpFile);
                a = ExecuteSQLScript(QStringList() << NomDumpFile);
                QFile::remove(NomDumpFile);
                if (a == 0)
                {
                    UpMessageBox::Watch(Q_NULLPTR,tr("Mise à jour effectuée de la base vers la version ") + QString::number(Version));
                    db->initParametresSysteme();
                }
                else
                {
                    Utils::playAlarm();
                    UpMessageBox::Watch(Q_NULLPTR,tr("Echec de la mise à jour vers la version ") + QString::number(Version) + "\n" + tr("Le programme de mise à jour n'a pas pu effectuer la tâche!"));
                    return false;
                }
            }
            if (Version == 66 && a == 0)
            {
                QString req = " select " CP_ID_MANUFACTURER ", CorNom, CorPrenom, CorStatut, CorMail, CorTelephone from " TBL_MANUFACTURERS
                                " where CorNom is not null and CorNom <> ''";
                bool ok;
                QList<QVariantList> listcom = DataBase::I()->StandardSelectSQL(req,ok);
                if (ok && listcom.size()>0)
                    for (int i= 0; i<listcom.size(); ++i)
                    {
                        req = "insert into " TBL_COMMERCIALS "(" CP_NOM_COM ", " CP_PRENOM_COM ", " CP_STATUT_COM ", " CP_MAIL_COM ", " CP_TELEPHONE_COM ", " CP_IDMANUFACTURER_COM ")"
                              " VALUES ( '" + listcom.at(i).at(1).toString() + "', '"  + listcom.at(i).at(2).toString() + "', '"
                                + listcom.at(i).at(3).toString() + "', '"  + listcom.at(i).at(4).toString()
                                + "', '"  + listcom.at(i).at(5).toString() + "', '"  + listcom.at(i).at(0).toString() + "')";
                        DataBase::I()->StandardSQL(req);
                    }
                req = "update " TBL_MANUFACTURERS " set CorNom = null, CorPrenom = null, CorStatut = null, CorMail = null, CorTelephone = null";
                DataBase::I()->StandardSQL(req);
            }
            if (Version == 74)
            {
                QSettings settings(PATH_FILE_INI, QSettings::IniFormat);
                if (settings.contains("Param_Poste/Utilise_BasedeDonnees_Villes"))
                {
                    if (settings.value("Param_Poste/Utilise_BasedeDonnees_Villes").toBool() == false)
                        db->setvillesfrance(false);
                    settings.remove("Param_Poste/Utilise_BasedeDonnees_Villes");
                }
            }
        }
    }
    else if (Versionencours > Version)
    {
        QString text = QObject::tr("Vous utilisez sur ce poste une version de Rufus prévue pour la version") + " " + QString::number(Version) + " " + QObject::tr("de la base de données");
        text += "<br/>" + QObject::tr("Cette version est peut-être incompatible avec la version") + " " + QString::number(Versionencours) + " " + tr("actuellement installée sur ce poste");
        text += "<br/>" + QObject::tr("Il est fortement conseillé de faire une mise à jour de Rufus");
        text += "<br/>" + QObject::tr("pour éviter des dysfonctionnements ou une altération de votre base de données Rufus");
        text += "<br/>" + QObject::tr("Vous pouvez télécharger la nouvelle version sur la page Téléchargements du site") + " <a href=\"https://www.rufusvision.org\">www.rufusvision.org</a>";
        UpMessageBox::Watch(Q_NULLPTR, tr("Version de Rufus trop ancienne"), text, UpDialog::ButtonOK, "https://www.rufusvision.org");
    }
    return true;
}

void RufusAdmin::ModifDirBackup()
{
    QString dirsauvorigin   = ui->DirBackupuplineEdit->text();
    DisconnectTimerInactive();
    QUrl url = Utils::getExistingDirectoryUrl(this, tr("Choisissez le dossier dans lequel vous voulez sauvegarder la base\n"
                                                       "Le nom de dossier ne doit pas contenir d'espace"), QUrl::fromLocalFile(dirsauvorigin));
    if (url == QUrl())
    {
        ConnectTimerInactive();
        return;
    }
    ui->DirBackupuplineEdit ->setText(url.path());
    if (dirsauvorigin != url.path())
    {
        db->setdirbkup(url.path());
        ParamAutoBackup();
        ui->EffacePrgSauvupPushButton->setEnabled(m_parametres->daysbkup()
                                               && QDir(m_parametres->dirbkup()).exists()
                                               && m_parametres->dirbkup() != ""
                                               && m_parametres->heurebkup() != QTime());
    }
    ConnectTimerInactive();
}

void RufusAdmin::ModifBDDVilles(Villes::TownsFrom from)
{
    db                  ->setvillesfrance(from == Villes::DATABASE);
    Datas::I()->villes  ->initListe(from);
}

void RufusAdmin::ModifDateBackup()
{
    Utils::Days days;
    days.setFlag(Utils::Lundi,      ui->LundiradioButton->isChecked());
    days.setFlag(Utils::Mardi,      ui->MardiradioButton->isChecked());
    days.setFlag(Utils::Mercredi,   ui->MercrediradioButton->isChecked());
    days.setFlag(Utils::Jeudi,      ui->JeudiradioButton->isChecked());
    days.setFlag(Utils::Vendredi,   ui->VendrediradioButton->isChecked());
    days.setFlag(Utils::Samedi,     ui->SamediradioButton->isChecked());
    days.setFlag(Utils::Dimanche,   ui->DimancheradioButton->isChecked());
    db->setdaysbkup(days);
    ParamAutoBackup();
    ui->EffacePrgSauvupPushButton->setEnabled(m_parametres->daysbkup()
                                           && QDir(m_parametres->dirbkup()).exists()
                                           && m_parametres->dirbkup() != ""
                                           && m_parametres->heurebkup() != QTime());
}

void RufusAdmin::ModifHeureBackup()
{
    db->setheurebkup(ui->HeureBackuptimeEdit->time());
    ParamAutoBackup();
    ui->EffacePrgSauvupPushButton->setEnabled(m_parametres->daysbkup()
                                           && QDir(m_parametres->dirbkup()).exists()
                                           && m_parametres->dirbkup() != ""
                                           && m_parametres->heurebkup() != QTime());
}

void RufusAdmin::BackupWakeUp()
{
    //Logs::trace("BKUP", "currentTime() = " + QTime::currentTime().toString("HH:mm:ss") + " - m_parametres->heurebkup() = " + m_parametres->heurebkup().toString("HH:mm:ss"));
    if (QTime::currentTime().toString("HH:mm") == m_parametres->heurebkup().toString("HH:mm") && QDate::currentDate() > m_lastbackupdate)
    {
        Utils::Day daybkup = Utils::Lundi;
        switch (QDate::currentDate().dayOfWeek()) {
        case 1: daybkup = Utils::Lundi; break;
        case 2: daybkup = Utils::Mardi; break;
        case 3: daybkup = Utils::Mercredi; break;
        case 4: daybkup = Utils::Jeudi; break;
        case 5: daybkup = Utils::Vendredi; break;
        case 6: daybkup = Utils::Samedi; break;
        case 7: daybkup = Utils::Dimanche;
        }
        if (m_parametres->daysbkup().testFlag(daybkup))
            if (!AutresPostesConnectes())
            {
                Logs::trace("LANCEMENT DU BKUP", "currentTime() = " + QTime::currentTime().toString("HH:mm:ss") + " - m_parametres->heurebkup() = " + m_parametres->heurebkup().toString("HH:mm:ss"));
                m_lastbackupdate = QDate::currentDate();
                Backup(m_parametres->dirbkup());
            }
    }
}

void RufusAdmin::ParamAutoBackup()
{
    if (m_parametres->dirbkup() == "" || !QDir(m_parametres->dirbkup()).exists() || !m_parametres->heurebkup().isValid() || !m_parametres->daysbkup())
    {
        EffaceProgrammationBackup();
        return;
    }
    t_timerbackup.disconnect(SIGNAL(timeout()));
    t_timerbackup.stop();
    t_timerbackup.start(30000); /*! le timer de déclenchement de la sauvegrade est lancé plus d'une fois par mintue à cause de la grande imprécision des QTimer
                                  * si on le lance toutes les 60", il est possible que le timer ne soit pas lancé dans la minute définie pour la sauvegarde.
                                  * En le lançant toutes les 30", ça marche.
                                  * C'est de la bidouille, je sais */
    connect(&t_timerbackup, &TimerController::timeout, this, [=] {BackupWakeUp();});

    /*! la suite n'est plus utilisée depuis OsX Catalina parce que OsX Catalina n'accepte plus les launchagents
#ifdef Q_OS_MACOS
    // elaboration de rufus.bup.plist
    DefinitScriptBackup(m_parametres->dirbkup());
    QString heure   = m_parametres->heurebkup().toString("H");
    QString minute  = m_parametres->heurebkup().toString("m");
    QString jourprg = "\t\t<array>\n";
    QString debutjour =
        "\t\t\t<dict>\n"
        "\t\t\t\t<key>Weekday</key>\n"
        "\t\t\t\t<integer>";
    QString finjour =
        "</integer>\n"
        "\t\t\t\t<key>Hour</key>\n"
        "\t\t\t\t<integer>"+ heure + "</integer>\n"
        "\t\t\t\t<key>Minute</key>\n"
        "\t\t\t\t<integer>" + minute + "</integer>\n"
        "\t\t\t</dict>\n";
    if (m_parametres->daysbkup().testFlag(Utils::Lundi))
        jourprg += debutjour + "1" + finjour;
    if (m_parametres->daysbkup().testFlag(Utils::Mardi))
        jourprg += debutjour + "2" + finjour;
    if (m_parametres->daysbkup().testFlag(Utils::Mercredi))
        jourprg += debutjour + "3" + finjour;
    if (m_parametres->daysbkup().testFlag(Utils::Jeudi))
        jourprg += debutjour + "4" + finjour;
    if (m_parametres->daysbkup().testFlag(Utils::Vendredi))
        jourprg += debutjour + "5" + finjour;
    if (m_parametres->daysbkup().testFlag(Utils::Samedi))
        jourprg += debutjour + "6" + finjour;
    if (m_parametres->daysbkup().testFlag(Utils::Dimanche))
        jourprg += debutjour + "7" + finjour;
    jourprg += "\t\t</array>\n";

    QString plist = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                    "<plist version=\"1.0\">\n"
                        "\t<dict>\n"
                            "\t\t<key>Label</key>\n"
                            "\t\t<string>rufus.backup</string>\n"
                            "\t\t<key>disabled</key>\n"
                            "\t\t<false/>\n"
                            "\t\t<key>ProgramArguments</key>\n"
                            "\t\t<array>\n"
                                "\t\t\t<string>bash</string>\n"
                                "\t\t\t<string>" + DIR_RUFUSADMIN SCRIPTBACKUPFILE + "</string>\n"
                            "\t\t</array>\n"
                            "\t\t<key>StartCalendarInterval</key>\n"
                            + jourprg +
                        "\t</dict>\n"
                    "</plist>\n";
    if (QFile::exists(SCRIPT_MACOS_PLIST_FILE))
        QFile::remove(SCRIPT_MACOS_PLIST_FILE);
    QFile fplist(SCRIPT_MACOS_PLIST_FILE);
    if (fplist.open(QIODevice::ReadWrite))
    {
        QTextStream out(&fplist);
        out << plist;
        fplist.close();
    }

    // relance du launchd
    QString unload  = "bash -c \"/bin/launchctl unload \"" + QDir::homePath();
    unload += SCRIPT_MACOS_PLIST_FILE "\"\"";
    QString load    = "bash -c \"/bin/launchctl load \""   + QDir::homePath();
    load += SCRIPT_MACOS_PLIST_FILE "\"\"";
    QProcess dumpProcess(parent());
    dumpProcess.start(unload);
    dumpProcess.waitForFinished();
    dumpProcess.start(load);
    dumpProcess.waitForFinished();
#endif
    */
}

void RufusAdmin::ProgrammeSQLVideImagesTemp(QTime timebackup) /*!  - abandonné parce qu'il continue à fonctionner même en cas de plantage du programme */
{
    //programmation de l'effacement du contenu de la table ImagesEchange
    db->StandardSQL("Use " DB_IMAGES);
    db->StandardSQL("DROP EVENT IF EXISTS VideImagesEchange");
    QString req =   "CREATE EVENT VideImagesEchange "
            "ON SCHEDULE EVERY 1 DAY STARTS '2018-03-23 " + timebackup.addSecs(-60).toString("HH:mm:ss") + "' "
            "DO DELETE FROM " TBL_ECHANGEIMAGES;
    db->StandardSQL(req);
    //programmation de l'effacement des pdf et jpg contenus dans Factures
    db->StandardSQL("Use " DB_COMPTA);
    db->StandardSQL("DROP EVENT IF EXISTS VideFactures");
    req =   "CREATE EVENT VideFactures "
            "ON SCHEDULE EVERY 1 DAY STARTS '2018-03-23 " + timebackup.addSecs(-60).toString("HH:mm:ss") + "' "
            "DO UPDATE " TBL_FACTURES " SET " CP_JPG_FACTURES " = null, " CP_PDF_FACTURES " = null";
    db->StandardSQL(req);
}

void RufusAdmin::DefinitScriptBackup(QString pathbackupbase)
{
    if (!QDir(pathbackupbase).exists())
        if (!Utils::mkpath(pathbackupbase))
            return;
    if (!QDir(pathbackupbase).exists())
        return;
    QString CRLF="\n";
    QString executabledump = QDir::toNativeSeparators(dirSQLExecutable() + "/mysqldump");
    QString scriptbackup= "#!/bin/bash";
    scriptbackup += CRLF;
    // Sauvegarde des 4 bases de Rufus
    scriptbackup += executabledump + " --force --opt --user=\"" LOGIN_SQL "\" -p\"" MDP_SQL "\" --skip-lock-tables --events --databases " DB_RUFUS " > \"" + QDir::toNativeSeparators(pathbackupbase + "/" DB_RUFUS ".sql") + "\"";
    scriptbackup += CRLF;
    scriptbackup += executabledump + " --force --opt --user=\"" LOGIN_SQL "\" -p\"" MDP_SQL "\" --skip-lock-tables --events --databases " DB_COMPTA " > \"" + QDir::toNativeSeparators(pathbackupbase + "/" DB_COMPTA ".sql") + "\"";
    scriptbackup += CRLF;
    scriptbackup += executabledump + " --force --opt --user=\"" LOGIN_SQL "\" -p\"" MDP_SQL "\" --skip-lock-tables --events --databases " DB_IMAGES " > \"" + QDir::toNativeSeparators(pathbackupbase + "/" DB_IMAGES ".sql") + "\"";
    scriptbackup += CRLF;
    scriptbackup += executabledump + " --force --opt --user=\"" LOGIN_SQL "\" -p\"" MDP_SQL "\" --skip-lock-tables --events --databases " DB_OPHTA " > \"" + QDir::toNativeSeparators(pathbackupbase + "/" DB_OPHTA ".sql") + "\"";
    scriptbackup += CRLF;
    // Sauvegarde de la table des utilisateurs
    scriptbackup += executabledump + " --force --opt --user=\"" LOGIN_SQL "\" -p\"" MDP_SQL "\" mysql user > \"" + QDir::toNativeSeparators(pathbackupbase + "/user.sql") + "\"";
    scriptbackup += CRLF;

    QFile fbackup(PATH_FILE_SCRIPTBACKUP);
    if (fbackup.exists())
        Utils::removeWithoutPermissions(fbackup);
    if (fbackup.open(QIODevice::ReadWrite))
    {
        QTextStream out(&fbackup);
        out << scriptbackup ;
        fbackup.close();
    }
}

int RufusAdmin::ExecuteSQLScript(QStringList ListScripts)
{
    QStringList listpaths;
    int a = 99;
    QString host;
    if( db->ModeAccesDataBase() == Utils::Poste )
        host = "localhost";
    else
        host = m_settings->value(Utils::getBaseFromMode(db->ModeAccesDataBase()) + Param_Serveur).toString();
    bool useSSL = (db->ModeAccesDataBase() == Utils::Distant);
    QString login = LOGIN_SQL;
    if (useSSL)
        login += "SSL";
    QString dirkey = "/etc/mysql";
    QString keys = "";
    if (useSSL)
    {
        if (m_settings->value(Utils::getBaseFromMode(Utils::Distant) + Dossier_ClesSSL).toString() != "")
            dirkey = m_settings->value(Utils::getBaseFromMode(Utils::Distant) + Dossier_ClesSSL).toString();
        else
            m_settings->setValue(Utils::getBaseFromMode(Utils::Distant) + Dossier_ClesSSL,dirkey);
        keys += " --ssl-ca=" + dirkey + "/ca-cert.pem --ssl-cert=" + dirkey + "/client-cert.pem --ssl-key=" + dirkey + "/client-key.pem";
    }
    QStringList args = QStringList()
                       << "-u" << login
                       << "-p" MDP_SQL
                       << "-h" << host
                       << "-P" << QString::number(db->port());
    QString sqlCommand = QDir::toNativeSeparators(dirSQLExecutable()+ m_executable);
#ifndef Q_OS_WIN
    for (int i = 0; i < args.size() ; ++i)
        sqlCommand += " " + args.at(i);
#endif

    for (int i=0; i<ListScripts.size(); i++)
        if (QFile(ListScripts.at(i)).exists())
        {
            QString path = ListScripts.at(i);
            QProcess dumpProcess(parent());
#ifdef Q_OS_WIN
            dumpProcess.setStandardInputFile(path);
            dumpProcess.start(sqlCommand, args);
#else
            /*! les commandes start ou startDetached ne fonctionnent pas sous MacOS ou Ubuntu */
            /*! https://www.qtcentre.org/threads/23460-QProcess-and-mysql-lt-backup-sql
             *  dumpProcess.setStandardInputFile(path);
             *  dumpProcess.start(sqlCommand, args);                    NE MARCHE PLUS DEPUIS Qt6 sous MacOS ou Linux */
            QString command = sqlCommand + " < " + path;
            QString bat = "bash -c \"" + command + "\"";
            dumpProcess.startCommand(bat);
#endif
            dumpProcess.waitForFinished(1000000);


            if (dumpProcess.error() == QProcess::FailedToStart)
            {
                Logs::ERROR(tr("Impossible de lancer le processus de chargement de la base de données à partir du fichier \"%1\"").arg(path));
                a = 99;
                break;
            }
            if (dumpProcess.exitStatus() == QProcess::NormalExit)
                a = dumpProcess.exitCode();
            else
            {
                Logs::ERROR(tr("Le processus de chargement de la base de données à partir du fichier \"%1\" a échoué").arg(path));
                break;
            }

            if (a != 0)
                break;
        }
    return a;
}

void RufusAdmin::EffaceBDDDataBackup()
{
    QList<QRadioButton*> listbutton2 = ui->JourSauvegardeframe->findChildren<QRadioButton*>();
    for (int i=0; i<listbutton2.size(); i++)
       listbutton2.at(i)->setChecked(false);
    ui->DirBackupuplineEdit->setText("");
    ui->HeureBackuptimeEdit->setTime(QTime(0,0));

    db->setdaysbkup(Utils::Day::Aucun);
    db->setheurebkup();
    db->setdirbkup();
    EffaceProgrammationBackup();
}

void RufusAdmin::EffaceProgrammationBackup()
{
    if (QFile::exists(PATH_FILE_SCRIPTBACKUP))
        QFile::remove(PATH_FILE_SCRIPTBACKUP);
    t_timerbackup.disconnect(SIGNAL(timeout()));
    t_timerbackup.stop();
    /*! la suite n'est plus utilisée depuis OsX Catalina parce que OsX Catalina n'accepte plus les launchagents
#ifdef Q_OS_MACOS
    QString file = SCRIPT_MACOS_PLIST_FILE;                          // file = "/Users/xxxx/Library/LaunchAgents/rufus.bup.plist"
    if (!QFile::exists(file))
        return;
    QString unload  = "bash -c \"/bin/launchctl unload \"" + file + "\"\"";             // unload = bash -c "/bin/launchctl unload "/Users/xxxx/Library/LaunchAgents/rufus.bup.plist""
    QProcess dumpProcess(parent());
    dumpProcess.start(unload);
    dumpProcess.waitForFinished();
    QFile::remove(file);
#endif
    */
}

void RufusAdmin::startImmediateBackup()
{
    if (AutresPostesConnectes())
        return;
    QString dirsauvorigin   = ui->DirBackupuplineEdit->text();
    if (dirsauvorigin == "" || !QDir(dirsauvorigin).exists())
        dirsauvorigin = PATH_DIR_RUFUS;
    QUrl url = Utils::getExistingDirectoryUrl(this, tr("Choisissez le dossier dans lequel vous voulez sauvegarder la base"), QUrl::fromLocalFile(dirsauvorigin), QStringList(), false);
    if (url != QUrl())
        ImmediateBackup(url.path());
}

bool RufusAdmin::ImmediateBackup(QString dirdestination, bool verifposteconnecte)
{
    if (verifposteconnecte)
        if (AutresPostesConnectes())
            return false;

    if (dirdestination == "")
    {
        QString dir = QDir(m_parametres->dirbkup()).exists()? m_parametres->dirbkup() : QDir::homePath();
        QUrl url = Utils::getExistingDirectoryUrl(this, tr("Choisissez le dossier dans lequel vous voulez sauvegarder la base"), QUrl::fromLocalFile(dir), QStringList(), false);
        if (url == QUrl())
            return false;
        dirdestination = url.path();
    }
    if (!QDir(dirdestination).exists())
        return false;

    bool OKbase     = false;
    bool OKImages   = false;
    bool OKVideos   = false;
    bool OKFactures = false;

    AskBupRestore(BackupOp, db->dirimagerie(), dirdestination);
    if (dlg_buprestore->exec() != QDialog::Accepted)
        return false;
    QList<UpCheckBox*> listchk = dlg_buprestore->findChildren<UpCheckBox*>();
    for (int i= 0; i<listchk.size(); i++)
    {
        if (listchk.at(i)->objectName() == "base")
            OKbase = listchk.at(i)->isChecked();
        else if (listchk.at(i)->objectName() == "images")
            OKImages = listchk.at(i)->isChecked();
        else if (listchk.at(i)->objectName() == "videos")
            OKVideos = listchk.at(i)->isChecked();
        else if (listchk.at(i)->objectName() == "factures")
            OKFactures = listchk.at(i)->isChecked();
    }

    if (!OKbase && !OKImages && !OKVideos && !OKFactures)
        return false;
    delete dlg_buprestore;
    dlg_buprestore = Q_NULLPTR;
    return Backup(dirdestination, OKbase, OKImages, OKVideos, OKFactures);
}

void RufusAdmin::ImportNouveauDocExterne(AppareilImagerie *appareil)
{
    QStringList filters, listnomsfiles;
    filters << "*.pdf" << "*.jpg";
    listnomsfiles = QDir(appareil->nomdossierechange()).entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
    for (int it=0; it<listnomsfiles.size(); it++)
    {
        QString nomfiledoc = listnomsfiles.at(it);
        if (!nomfiledoc.contains("smbtest"))
            m_importdocsexternesthread->RapatrieDocumentsThread(appareil, nomfiledoc);
    }

}

bool RufusAdmin::Backup(QString pathdirdestination, bool OKBase,  bool OKImages, bool OKVideos, bool OKFactures)
{
    auto result = [] (qintptr handle, RufusAdmin *radm)
    {
        ShowMessage::I()->ClosePriorityMessage(handle);
        radm->ConnectTimerInactive();
    };
    if (QDir(db->dirimagerie()).exists())
    {
        Utils::cleanfolder(db->dirimagerie() + NOM_DIR_IMAGES);
        Utils::cleanfolder(db->dirimagerie() + NOM_DIR_FACTURES);
        Utils::cleanfolder(db->dirimagerie() + NOM_DIR_VIDEOS);
    }
    else
    {
        OKImages = false;
        OKVideos = false;
        OKFactures = false;
    }

    QString msgEchec = tr("Incident pendant la sauvegarde");
    QString msg = "";
    qintptr handledlg = 0;
    ShowMessage::I()->PriorityMessage(tr("Sauvegardede la base de données Rufus"),handledlg);
    DisconnectTimerInactive();

    //On vide les champs blob de la table factures et la table EchangeImages
    db->StandardSQL("UPDATE " TBL_FACTURES " SET " CP_JPG_FACTURES " = null, " CP_PDF_FACTURES " = null");
    db->StandardSQL("DELETE FROM " TBL_ECHANGEIMAGES);
    Utils::mkpath(pathdirdestination);

    if (OKBase)
    {
        QString pathbackupbase = pathdirdestination + "/" + QDateTime::currentDateTime().toString("yyyyMMdd-HHmm");
        Utils::mkpath(pathbackupbase);

        /*! sauvegarde de la base */
        QFile::remove(PATH_FILE_SCRIPTBACKUP);
        DefinitScriptBackup(pathbackupbase);
        QString Msg = (tr("Sauvegarde de la base de données\n")
                       + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
        UpSystemTrayIcon::I()->showMessage(tr("Messages"), Msg, Icons::icSunglasses(), 3000);
#ifdef Q_OS_WIN
        const QString task = QDir::toNativeSeparators(PATH_FILE_SCRIPTBACKUP);
#else
        const QString task = "sh " + PATH_FILE_SCRIPTBACKUP;
#endif
        msg += tr("Base de données sauvegardée!\n");
        m_ostask.disconnect(SIGNAL(result(const int &)));
        connect(&m_ostask, &OsTask::result, this, [=](int a) {
            UpSystemTrayIcon::I()->showMessage(tr("Messages"), (a == 0? msg : msgEchec), Icons::icSunglasses(), 3000);
            result(handledlg, this);
            QFile::remove(PATH_FILE_SCRIPTBACKUP);
            return true;
        });
        m_ostask.execute(task);

        /*! élimination des anciennes sauvegardes */
        QDir dir(pathdirdestination);
        dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList list = dir.entryInfoList();
        for(int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            if (fileInfo.fileName().split("-").size()>0)
            {
                const QString date    = fileInfo.fileName().split("-").at(0);
                int year        = date.left(4).toInt();
                int month       = date.mid(4,2).toInt();
                int day         = date.right(2).toInt();
                QDate birthtime = QDate(year,month,day);
                if (birthtime.isValid())
                    if (birthtime.daysTo(QDateTime::currentDateTime().date()) > 14)
                    {
                        if (fileInfo.isDir())
                            QDir(fileInfo.absoluteFilePath()).removeRecursively();
                        else if (fileInfo.isFile())
                            QFile(fileInfo.absoluteFilePath()).remove();
                    }
            }
        }

        /*! sauvegarde de AdminRufus.ini et des fichiers ressources */
        QFile file(PATH_FILE_INI);
        Utils::copyWithPermissions(file, pathbackupbase + "/" NOM_ADMINFILE_INI);
        msg += tr("Fichier de paramétrage Rufus.ini sauvegardé\n");
    }

    if (OKImages || OKVideos || OKFactures)
    {
        QString dirNomSource;
        QString dirNomDest;

        if (OKFactures) {
            dirNomSource = db->dirimagerie() + NOM_DIR_FACTURES;
            dirNomDest = pathdirdestination + NOM_DIR_FACTURES;
            int t = 0;
            Utils::countFilesInDirRecursively(dirNomSource, t);
            UpProgressDialog *progdial = new UpProgressDialog(0,t, this);
            progdial->show();
            int n = 0;
            Utils::copyfolderrecursively(dirNomSource, dirNomDest, n, tr("Sauvegarde des factures"), progdial);
            delete progdial;
            msg += tr("Factures sauvegardées\n");
        }
        if (OKImages) {
            dirNomSource = db->dirimagerie() + NOM_DIR_IMAGES;
            dirNomDest = pathdirdestination + NOM_DIR_IMAGES;
            int t = 0;
            Utils::countFilesInDirRecursively(dirNomSource, t);
            UpProgressDialog *progdial = new UpProgressDialog(0,t, this);
            progdial->show();
            int n = 0;
            Utils::copyfolderrecursively(dirNomSource, dirNomDest, n, tr("Sauvegarde des fichiers d'imagerie"), progdial);
            delete progdial;
            msg += tr("Fichiers imagerie sauvegardés\n");
        }
        if (OKVideos) {
            dirNomSource = db->dirimagerie() + NOM_DIR_VIDEOS;
            dirNomDest = pathdirdestination + NOM_DIR_VIDEOS;
            int t = 0;
            Utils::countFilesInDirRecursively(dirNomSource, t);
            UpProgressDialog *progdial = new UpProgressDialog(0,t, this);
            progdial->show();
            int n = 0;
            Utils::copyfolderrecursively(dirNomSource, dirNomDest, n, tr("Sauvegarde des videos"), progdial);
            delete progdial;
            msg += tr("Fichiers video sauvegardés");
        }
    }
    else
    {
        result(handledlg, this);
        return false;
    }
    qintptr z = 0;
    ShowMessage::I()->PriorityMessage(msg,z, 10000);
    return true;
}

/*!
 * \brief RufusAdmin::sqlExecutable
 * \return le chemin vers les éxécutable mysql et mysqldump
 * Le chemin est stocké dans Rufus.ini au format Unix avec des "/"
 * Pour le retrouver au format natif, on lui applique la fonction QDir::toNativeSeparators()
 */
QString RufusAdmin::dirSQLExecutable()
{
    if (m_dirSQLExecutable == "")
        setDirSQLExecutable();
    return m_dirSQLExecutable;
}

/*!
 * \brief RufusAdmin::setDirSQLExecutable
 * La fonction recherche les éxécutables SQL: mysql et mysqldump
 * Elle les recherche d'abord dans le package logiciel
 * puis dans Rufus.ini
 * puis dans les standardpaths du système
 * Si elle ne les trouve dans aucun de ces 3 endroits, elle interroge l'utilisateur
 * et si l'utilisateur l'informe qu'il ne peut pas trouver les executables
 * le programme est quand même lancé en informant l'utilisateur qu'il ne pourra faire aucune opération de restauration, sauvegarde ou mise à jour de la base
*/
void RufusAdmin::setDirSQLExecutable()
{
    QString dirdefaultsqlexecutable ("");
    QString dirsqlexecutable ("");
    m_executable        = "/mysql";
    m_dumpexecutable    = "/mysqldump";
    bool a = false;

/*! 1. On recherche dans le package logiciel */
/*! ne marche pas sous Mac Silicon pour le moment */
#ifdef Q_OS_MACOS
    QDir mysqldir           = QDir(QCoreApplication::applicationDirPath());
    mysqldir.cdUp();
    dirdefaultsqlexecutable = mysqldir.absolutePath() + "/Applications";
    a                       = QFile(dirdefaultsqlexecutable + m_executable).exists();
#endif

#ifdef Q_OS_WIN
    m_executable            = "/mysql.exe";
    m_dumpexecutable        = "/mysqldump.exe";
    QDir mysqldir           = QDir(QCoreApplication::applicationDirPath());
    dirdefaultsqlexecutable = mysqldir.absolutePath() + "/Applications";
    a                       = QFile(dirdefaultsqlexecutable + m_executable).exists();
#endif
    if (a)
    {
        if (dirdefaultsqlexecutable != "")
            m_settings->setValue(Param_SQLExecutable, dirdefaultsqlexecutable);
        m_dirSQLExecutable = dirdefaultsqlexecutable;
        return;
    }

/*! 2. on recherche dans les chemins habituels du système */
#ifdef Q_OS_MACOS
    dirsqlexecutable = "/usr/local/mysql/bin";
    if (!QFile(dirsqlexecutable + "/mysql").exists())
    {
        dirsqlexecutable = "/usr/local/bin";
        if (!QFile(dirsqlexecutable + "/mysql").exists())
            dirsqlexecutable = "/opt/homebrew/opt/mariadb/bin";
    }
    a = (QFile(dirsqlexecutable + "/mysql").exists());
#endif
#ifdef Q_OS_LINUX
    dirsqlexecutable = "/usr/bin";
    a = (QFile(dirsqlexecutable + m_executable).exists());
#endif
#ifdef Q_OS_WIN
    QDir programs = QDir("C:/Program Files");
    if (programs.exists())
    {
        QStringList listmariadbdirs = programs.entryList(QStringList() << "MariaDB *", QDir::Dirs);
        if (listmariadbdirs.size() > 0)
        {
            dirsqlexecutable = programs.absolutePath() + "/"  + listmariadbdirs.at(0) + "/bin";
            a = (QFile(dirsqlexecutable + m_executable).exists());
        }
        if (!a)
        {
            programs = QDir("C:/Program Files/MySQL");
            if (programs.exists())
            {
                QStringList listmysqldirs = programs.entryList(QStringList() << "MySQL Server *", QDir::Dirs);
                if (listmysqldirs.size() > 0)
                {
                    dirsqlexecutable = programs.absolutePath() + "/"  + listmysqldirs.at(0) + "/bin";
                    a = (QFile(dirsqlexecutable + m_executable).exists());
                }
            }
        }
    }
#endif

    if (a)
    {
        m_settings->setValue(Param_SQLExecutable, dirsqlexecutable);
        m_dirSQLExecutable = dirsqlexecutable;
        return;
    }

    /*! 3. On n'a rien trouvé - on teste la valeur enregistrée dans rufus.ini */

    dirsqlexecutable = m_settings->value(Param_SQLExecutable).toString();
    if (QFile(dirsqlexecutable + m_executable).exists())
    {
        m_dirSQLExecutable = dirsqlexecutable;
        return;
    }
}


void RufusAdmin::ResumeTCPSocketStatut(QString listidposteconnectes)
{
    if (!m_utiliseTCP)
        return;
    QString statut;
    listidposteconnectes.remove(TCPMSG_ListeStringIdPostesConnectes);
    QStringList listid = listidposteconnectes.split(TCPMSG_Separator);
    for (auto it = listid.begin(); it != listid.end(); ++it)
    {
        PosteConnecte *post = Datas::I()->postesconnectes->getByStringId(*it);
        if (it == listid.begin())
        {
            // le 1er item de gListSockets est le serveur
            statut += tr("ServeurTCP") + "\n\t";
            if (post) //!>* chaque item contient adresseIP, adresseMac, LoaclhostName(), idUser puis  TCPMSG_ListeSockets
            {
                statut += post->ipadress() + " - "
                        + post->macadress() + " - "
                        + post->nomposte() + " --- "
                        + (Datas::I()->users->getById(post->id())? Datas::I()->users->getById(post->id())->login() : "") + "\n";
            }
            else
                statut += tr("inconnu") + "\n";
            statut += tr("Postes connectés") + "\n";
        }
        else
        {
            if (post)
            {
                statut += "\t" + post->ipadress() + " - "
                        + post->macadress() + " - "
                        + post->nomposte() + " --- "
                        + (Datas::I()->users->getById(post->id())? Datas::I()->users->getById(post->id())->login() : "") + "\n";
            }
            else
                statut += "\t" + tr("inconnu") + "\n";
        }
    }
    m_socketStatut = statut;
    foreach (PosteConnecte *post, *Datas::I()->postesconnectes->postesconnectes())
    {
        if (post)
            if(post->isdistant())
                m_socketStatut += "\t" + (Datas::I()->sites->getById(post->idlieu())? Datas::I()->sites->getById(post->idlieu())->nom() : "") + " ---- "
                        + (Datas::I()->users->getById(post->id())? Datas::I()->users->getById(post->id())->login() : "") + "\n";
            }
    emit ModifEdit(m_socketStatut); // déclenche la modification de la fenêtre resumestatut
}

void RufusAdmin::Edit(QString txt, int delaieffacement)
{
    UpDialog        *gAsk           = new UpDialog(this);
    UpTextEdit      *TxtEdit        = new UpTextEdit(gAsk);
    int x = 0;
    int y = 0;
    QList<QScreen*> listscreens = QGuiApplication::screens();
    if (listscreens.size())
    {
        x = listscreens.first()->geometry().width();
//    int y = qApp->desktop()->availableGeometry().height();
//    int x = qApp->desktop()->availableGeometry().width();
        y = listscreens.first()->geometry().height();
    }

    gAsk->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

    TxtEdit->setText(txt);
    TxtEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    connect(this,   &RufusAdmin::ModifEdit, TxtEdit, [=](QString txt) {TxtEdit->setText(txt);});

    gAsk->setMaximumWidth(x);
    gAsk->setMaximumHeight(y);

    gAsk->dlglayout()->insertWidget(0,TxtEdit);

    gAsk->AjouteLayButtons(UpDialog::ButtonCancel | UpDialog::ButtonOK);
    connect(gAsk->OKButton, &QPushButton::clicked, gAsk, &QDialog::accept);
    gAsk->restoreGeometry(m_settings->value("PositionsFiches/PositionEdit").toByteArray());

    if (delaieffacement > 0)
    {
        QTimer *tim = new QTimer(gAsk);
        tim->setSingleShot(true);
        connect(tim, &QTimer::timeout, gAsk, &UpDialog::accept);
        tim->start(delaieffacement);
    }

    gAsk->exec();
    m_settings->setValue("PositionsFiches/PositionEdit",gAsk->saveGeometry());
    delete gAsk;
}


void RufusAdmin::VerifLastVersion()
{
    auto comparelastversion = [&] {
        QString actversion = qApp->applicationVersion().split("/").at(0);
        QDate dateactversion = QDate::fromString(actversion,"dd-MM-yyyy");
        QDate datenewversion = QDate::fromString(m_UPDLastVersion, "yyyy/MM/dd");
        if (dateactversion < datenewversion)
        {
            QString text = QObject::tr("La nouvelle version est datée du ") + QLocale::system().toString(datenewversion, "d MMM yyyy") + "<br/>"
                       + QObject::tr("Vous utilisez la version du ") + QLocale::system().toString(dateactversion, "d MMM yyyy");
            if (m_UPDBase)
            {
                text += "<br/>" + QObject::tr("Cette nouvelle version impose une mise à jour de la base de données");
                if (!m_UPDCcompatibiltyWithPrec)
                    text += "<br/>" + QObject::tr("Après cette mise à jour, tous les postes utilisant Rufus sur cette base devront aussi évoluer vers la nouvelle versionr");
                else
                    text += "<br/>" + QObject::tr("Cette mise à jour de la base de données reste compatible avec votre version actuelle de Rufus");
            }
            else
                text += "<br/>" + QObject::tr("Cette nouvelle version n'impose pas de mise à jour de la base de données et est compatible avec la précédente version de Rufus");
            text += "<br/>" + QObject::tr("Vous pouvez télécharger la nouvelle version sur la page Téléchargements du site") + " <a href=\"https://www.rufusvision.org\">www.rufusvision.org</a>";
            UpMessageBox::Watch(this, QObject::tr("Une nouvelle version de Rufus est en ligne"), text, UpDialog::ButtonOK, "https://www.rufusvision.org");
        }
        qDebug() << "OS = " << m_os;
    };

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(LIEN_XML_RUFUSLASTVERSION));
    QNetworkReply *reply = manager->get(request);

    connect(manager, &QNetworkAccessManager::finished,
            this, [=]
    {
        QByteArray data;
        if(reply->error() == QNetworkReply::NoError)
        {
            m_os = QSysInfo::productType();
            if (m_os == "osx")
                m_os  = "macos";
            else if (m_os == "" || m_os == "ubuntu")
                m_os = "linux";
            reply->deleteLater();
            data = reply->readAll();
            QDomDocument docxml;
            docxml.setContent(data);
            QDomElement xml = docxml.documentElement();
            for (int i=0; i<xml.childNodes().size(); i++)
            {
                QDomElement system = xml.childNodes().at(i).toElement();
                if (m_os != system.tagName())
                    continue;
                for (int j=0; j<system.childNodes().size(); j++)
                {
                    QDomElement child = system.childNodes().at(j).toElement();
                    if (child.tagName() == "Date")
                        m_UPDLastVersion = child.text();
                    else if (child.tagName() == "Base")
                    {
                        for (int k=0; k<child.childNodes().size(); k++)
                        {
                            QDomElement basechild = child.childNodes().at(k).toElement();
                            if (basechild.tagName() == "UPDBase")
                                m_UPDBase = (basechild.text() == "Yes");
                            else if (basechild.tagName() == "CompatibleWithPrecedent")
                                m_UPDCcompatibiltyWithPrec = (basechild.text() == "Yes");
                        }
                    }
                    else if (child.tagName() == "UPDRessources")
                        m_UPDRessources = (child.text() == "Yes");
                    else if (child.tagName() == "Comment")
                        m_UPDComment = child.text();
                }
                i = xml.childNodes().size();
            }
        }
        comparelastversion();
    });
}


void RufusAdmin::VerifModifsFlags()
/*! Utilisé pour vérifier
 *      des modifs de la salle d'attente
 *      des modifs de la liste des correspondants
 *      ou l'arrivée de nouveaux  messages
 *  effectués par des postes distants
 */
{
    int flag = flags->flagSalleDAttente();
    if (m_flagsalledattente < flag)
    {
        m_flagsalledattente = flag;
        EnvoieTCPMessage(TOUS, TCPMSG_MAJSalAttente);
    }
    flag = flags->flagCorrespondants();
    if (m_flagcorrespondants < flag)
    {
        m_flagcorrespondants = flag;
        EnvoieTCPMessage(TOUS, TCPMSG_MAJCorrespondants);
    }
    flag = flags->flagMessages();
    if (m_flagmessages < flag)
    {
        m_flagmessages = flag;

        QString req =
                "select mess." CP_ID_MSG ", " CP_IDDESTINATAIRE_JOINTURESMSG ", " CP_DATECREATION_MSG " from "
                TBL_MESSAGES " mess left outer join " TBL_MESSAGESJOINTURES " joint on mess." CP_ID_MSG " = joint." CP_IDMSG_JOINTURESMSG " \n"
                " where " CP_DATECREATION_MSG " > '" + m_dateDernierMessage.toString("yyyy-MM-dd HH:mm:ss")
                + "' and " CP_ASUPPRIMER_MSG " is null"
                + " order by " CP_DATECREATION_MSG;                         //! tous les nouveaux messages depuis la date du dernier message
        QList<QVariantList> listmsg = db->StandardSelectSQL(req, m_ok);
        int TotalNvxMessages = listmsg.size();
        QHash<int,int> mapmessages;                                         //! la map des messages - key = iddestinataire, value = nombre de messages
        if (TotalNvxMessages>0)
        {
            for (int i=0; i<TotalNvxMessages; i++)
            {
                QHash<int,int>::const_iterator itm = mapmessages.constFind(listmsg.at(i).at(1).toInt());
                if (itm != mapmessages.constEnd())
                    mapmessages[itm.key()] = itm.value()+1;
                else
                    mapmessages[listmsg.at(i).at(1).toInt()] = 1;
            }
            m_dateDernierMessage = listmsg.at(listmsg.size()-1).at(2).toDateTime();
        }
        for (QHash<int,int>::const_iterator itmsg = mapmessages.constBegin(); itmsg != mapmessages.constEnd(); ++itmsg)
        {
            QString msg = TCPMSG_Separator + QString::number(itmsg.value()) + TCPMSG_MsgBAL;
            EnvoieTCPMessage(BAL,msg,itmsg.key());
        }
    }
}


