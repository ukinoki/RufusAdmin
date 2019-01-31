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

#include "rufusadmin.h"
#include "ui_rufusadmin.h"
#include <QDebug>

RufusAdmin::RufusAdmin(QWidget *parent) : QMainWindow(parent), ui(new Ui::RufusAdmin)
{
    Datas::I();
    // la version du programme correspond à la date de publication, suivie de "/" puis d'un sous-n° - p.e. "23-6-2017/3"
    qApp->setApplicationVersion("01-02-2019/3");       // doit impérativement être composé de date version / n°version);

    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    QString border = "border-image: url(://wallpaper.jpg)";
    qApp->setStyleSheet(
        "QDialog{" + border + "}"
        "QGroupBox{font:bold;}"
        "QLineEdit {background-color:white; border: 1px solid rgb(150,150,150);border-radius: 5px;}"
        "QLineEdit:focus {border: 1px solid rgb(164, 205, 255);border-radius: 5px;}"
        "QRadioButton::indicator {width: 18px; height: 18px;}"
        "QRadioButton::indicator::checked {image: url(://blueball.png);}"
        "QScrollArea {background-color:rgb(237, 237, 237); border: 1px solid rgb(150,150,150);}"
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

    giconBackup         .addFile(QStringLiteral("://Backup.png"),               QSize(),        QIcon::Normal, QIcon::Off);
    giconCopy           .addFile(QStringLiteral("://copy.png"),                 QSize(),        QIcon::Normal, QIcon::Off);
    giconSunglasses     .addFile(QStringLiteral("://Sunglasses.png"),           QSize(),        QIcon::Normal, QIcon::Off);
    giconSortirDossier  .addFile(QStringLiteral("://metadoc.png"),              QSize(25,25),   QIcon::Normal, QIcon::Off);
    giconAnnul          .addFile(QStringLiteral("://Supprime.png"),             QSize(30,30),   QIcon::Normal, QIcon::Off);
    giconEuro           .addFile(QStringLiteral("://euro.ico"),                 QSize(40,40),   QIcon::Normal, QIcon::Off);
    giconEuroCount      .addFile(QStringLiteral("://EuroCount.png"),            QSize(40,40),   QIcon::Normal, QIcon::Off);
    giconFermeAppuye    .addFile(QStringLiteral("://shutdowndown.png"),         QSize(30,30),   QIcon::Normal, QIcon::Off);
    giconFermeRelache   .addFile(QStringLiteral("://shutdown.png"),             QSize(30,30),   QIcon::Normal, QIcon::Off);
    giconNull           .addFile(QStringLiteral(""),                            QSize(30,30),   QIcon::Normal, QIcon::Off);
    giconOK             .addFile(QStringLiteral("://start.png"),                QSize(),        QIcon::Normal, QIcon::Off);
    gIconRufusAdmin     .addFile(QStringLiteral("://wrench-screwdriver.png"),   QSize(),        QIcon::Normal, QIcon::Off);
    giconErase          .addFile(QStringLiteral("://eraser.png"),               QSize(),        QIcon::Normal, QIcon::Off);

    ui->EffacePrgSauvupPushButton   ->setIcon(giconErase);
    ui->EffacePrgSauvupPushButton   ->setIconSize(QSize(35,35));
    ui->RestaurBaseupPushButton     ->setIcon(giconCopy);
    ui->ImmediatBackupupPushButton  ->setIcon(giconBackup);

    setWindowIcon(giconSunglasses);
    ui->FermepushButton->setUpButtonStyle(UpSmallButton::CLOSEBUTTON);
    gnomFichIni         = QDir::homePath() + NOMFIC_INI;
    gsettingsIni        = new QSettings(gnomFichIni, QSettings::IniFormat);
    gNomFichRufusIni    = QDir::homePath() + NOMFIC_INIRUFUS;
    gNouvMDP            = "nouv";
    gAncMDP             = "anc";
    gConfirmMDP         = "confirm";
    Utils::mkpath(QDir::homePath() + NOMDIR_RUFUS);

    RestoreFontAppli(); // les polices doivent être appliquées après la définition des styles
    setMapDatas();    

    ConnexionBase();
    if (!VerifBase())
        exit(0);

    idlieuExercice = DetermineLieuExercice();
    gNomLieuExercice = "";
    QList<QList<QVariant>> listlieux = db->StandardSelectSQL("select nomlieu from " NOM_TABLE_LIEUXEXERCICE " where idlieu = " + QString::number(idlieuExercice), ok);
    gNomLieuExercice = listlieux.at(0).at(0).toString();
    ui->AppareilsconnectesupLabel->setText(tr("Appareils connectés au réseau") + " <font color=\"green\"><b>" + gNomLieuExercice + "</b></font> ");

    //recherche de l'idUser du compte AdminDocs
    idAdminDocs = 0;
    QString req = "select iduser from " NOM_TABLE_UTILISATEURS " where UserLogin = '" NOM_ADMINISTRATEURDOCS "'";
    QList<QList<QVariant>> listusr = db->StandardSelectSQL(req, ok);
    if (listusr.size()==0)
    {
        req = "select iduser from " NOM_TABLE_UTILISATEURS " where UserNom = '" NOM_ADMINISTRATEURDOCS "'";
        QList<QList<QVariant>> listusers = db->StandardSelectSQL(req, ok);
        if (listusers.size()>0)
        {
            db->StandardSQL("update " NOM_TABLE_UTILISATEURS " set UserLogin = '" NOM_ADMINISTRATEURDOCS "' where UserNom = '" NOM_ADMINISTRATEURDOCS "'");
        }
        else
            db->StandardSQL("insert into " NOM_TABLE_UTILISATEURS " (UserNom, UserLogin) values ('" NOM_ADMINISTRATEURDOCS "','" NOM_ADMINISTRATEURDOCS "')");
        req = "select iduser from " NOM_TABLE_UTILISATEURS " where UserLogin = '" NOM_ADMINISTRATEURDOCS "'";
        listusr = db->StandardSelectSQL(req, ok);
        QList<QList<QVariant>> listmdp = db->StandardSelectSQL("select mdpadmin from " NOM_TABLE_PARAMSYSTEME,ok);
        if (listmdp.at(0).at(0).toString() == "")
            db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set mdpadmin = '" + db->getDataBase().password() + "'");
    }
    UserAdmin = new User(DataBase::getInstance()->loadUserDatabyLogin(NOM_ADMINISTRATEURDOCS));
    idAdminDocs = UserAdmin->id();

    // on vérifie que le programme n'est pas déjà en cours d'éxécution sur un autre poste
    QString reqp = "select NomPosteConnecte from " NOM_TABLE_USERSCONNECTES
                   " where idUser = " + QString::number(idAdminDocs) +
                   " and NomPosteConnecte != '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                   " and idlieu = " + QString::number(idlieuExercice) +
                   " and time_to_sec(timediff(now(),heurederniereconnexion)) < 60";
    QList<QList<QVariant>> listusr2 = db->StandardSelectSQL(reqp, ok);
    if (listusr2.size()>0)
    {
        UpMessageBox::Watch(this, tr("Programme déjà en cours d'éxécution sur le poste ") + listusr2.at(0).at(0).toString().remove(" - " NOM_ADMINISTRATEURDOCS), tr("Sortie du programme"));
        exit(0);
    }
    else
        db->StandardSQL("delete from " NOM_TABLE_USERSCONNECTES " where idUser = " + QString::number(idAdminDocs) + " and idlieu = " + QString::number(idlieuExercice));

    // 5 mettre en place le TcpSocket
    gIPadr                      = Utils::getIpAdress();
    gMacAdress                  = Utils::getMACAdress();
    TCPServer                   = TcpServer::getInstance();
    TCPServer                   ->setId(idAdminDocs);
    connect(TCPServer,          &TcpServer::ModifListeSockets,      this,   &RufusAdmin::ResumeTCPSocketStatut);
    TCPServer                   ->start();
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set AdresseTCPServeur = '" + gIPadr + "'");
    gflagCorrespdts             = GetflagCorrespdts();
    gflagSalDat                 = GetflagSalDat();


    gTimerSalDatCorrespMsg      = new QTimer(this);     /* scrutation des modifs de la salle d'attente et des correspondants et de l'arrivée de nouveaux messages utilisé par
                                                           pour verifier les modifications faites par les postes distants
                                                        */
    gTimerVerifVerrou           = new QTimer(this);     // utilisé par le TcpServer pour vérifier l'absence d'utilisateurs déconnectés dans la base

    gTimerUserConnecte          = new QTimer(this);     // mise à jour de la connexion à la base de données
    gTimerSupprDocs             = new QTimer(this);     // utilisé par le poste importateur pour vérifier s'il y a des documents à supprimer
    gTimerVerifDivers           = new QTimer(this);     // vérification du poste importateur des documents et e la version de la base
    gTimerSupprDocs             = new QTimer(this);     // verification des documents à supprimer
    gTimerProgressBar           = new QTimer(this);     // progression de la progressbar - quand la progressbar est au maximum, la fiche est cachée

    setPosteImportDocs(); // on prend la place d'importateur des documents dans les utilisateurs connectés
    Slot_VerifPosteImport();
    Slot_VerifVersionBase();
    Slot_CalcExporteDocs();
    QList<QList<QVariant>> listdate = db->StandardSelectSQL("select max(creele) from " NOM_TABLE_MESSAGES, ok);
    if (listdate.size()==0)
        gDateDernierMessage = QDateTime::currentDateTime();
    else
        gDateDernierMessage = listdate.at(0).at(0).toDateTime();

    // Lancement du timer de vérification des verrous - +++ à lancer après le timer gTimerVerifGestDocs puisqu'il l'utilise
    gTimerVerifVerrou->start(60000);// "toutes les 60 secondes"

    gTimerSalDatCorrespMsg  ->start(1000);
    gTimerUserConnecte      ->start(10000);
    gTimerVerifDivers       ->start(30000);

    QString veille = MISE_EN_VEILLE;
    dureeVeille = veille.toInt();
    ui->MiseEnVeilleprogressBar->setMinimum(0);
    ui->MiseEnVeilleprogressBar->setMaximum(dureeVeille);
    ui->MiseEnVeilleprogressBar->setInvertedAppearance(true);
    connect(gTimerProgressBar, &QTimer::timeout, this, [=] {ui->MiseEnVeilleprogressBar->setValue(ui->MiseEnVeilleprogressBar->value()-1);});
    gTimerProgressBar->start(1);
    gTimerSupprDocs->start(60000);// "toutes les 60 secondes"
    gTimerDocsAExporter = new QTimer(this);
    gTimerDocsAExporter->start(60000);// "toutes les 60 secondes"

    ConnectTimers();

    connect(ui->EmplacementServeurupComboBox,   SIGNAL(currentIndexChanged(int)),   this,   SLOT(Slot_EnregistreEmplacementServeur(int)));
    connect(ui->ExportImagespushButton,         &QPushButton::clicked,              this,   &RufusAdmin::ExporteDocs);
    connect(ui->FermepushButton,                SIGNAL(clicked(bool)),              this,   SLOT(Slot_MasqueAppli()));
    connect(ui->GestionBanquespushButton,       SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestionBanques()));
    connect(ui->GestLieuxpushButton,            SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestLieux()));
    connect(ui->GestionMotifspushButton,        SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestionMotifs()));
    connect(ui->GestUserpushButton,             SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestUser()));
    connect(ui->InitMDPAdminpushButton,         SIGNAL(clicked(bool)),              this,   SLOT(Slot_ModifMDP()));;
    connect(ui->RestaurBaseupPushButton,        SIGNAL(clicked(bool)),              this,   SLOT(Slot_RestaureBase()));
    connect(ui->StockageupPushButton,           SIGNAL(clicked(bool)),              this,   SLOT(Slot_ModifDirImagerie()));
    connect(ui->NetworkStatuspushButton,        &QPushButton::clicked,              this,   [=] {Edit(gSocketStatut, 20000);});


    widgAppareils = new WidgetButtonFrame(ui->AppareilsConnectesupTableWidget);
    widgAppareils->AddButtons(WidgetButtonFrame::PlusButton | WidgetButtonFrame::MoinsButton);
    connect(widgAppareils, SIGNAL(choix(int)), this, SLOT(Slot_ChoixButtonFrame(int)));

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
    widgAppareils->widgButtonParent()->setFixedWidth(ui->AppareilsConnectesupTableWidget->width());
    QVBoxLayout *applay = new QVBoxLayout();
    applay      ->addWidget(ui->MiseEnVeilleprogressBar);
    QHBoxLayout *Stocklay = new QHBoxLayout();
    Stocklay    ->addWidget(ui->StockageupLabel);
    Stocklay    ->addWidget(ui->StockageupLineEdit);
    Stocklay    ->addWidget(ui->StockageupPushButton);
    Stocklay    ->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Expanding));
    QHBoxLayout *appcentrelay = new QHBoxLayout();
    appcentrelay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Expanding));
    appcentrelay->addWidget(ui->AppareilsconnectesupLabel);
    appcentrelay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Expanding));
    applay      ->addLayout(appcentrelay);
    applay      ->addWidget(widgAppareils->widgButtonParent());
    applay      ->addLayout(Stocklay);

    int marge   = 5;
    applay      ->setContentsMargins(marge,marge,marge,marge);
    applay      ->setSpacing(marge);
    ui->Appareilsconnectesframe ->setFixedWidth(widgAppareils->widgButtonParent()->width() + marge + marge + marge);
    ui->Appareilsconnectesframe ->setLayout(applay);
    ui->Sauvegardeframe         ->setFixedWidth(widgAppareils->widgButtonParent()->width() + marge + marge + marge);
    setFixedWidth(ui->Appareilsconnectesframe->width() + 20);
    Remplir_Table();
    if (gMode == Poste)
    {
        QList<QList<QVariant>> listdir = db->StandardSelectSQL("select dirimagerie from " NOM_TABLE_PARAMSYSTEME, ok);
        QString NomDirStockageImagerie = listdir.at(0).at(0).toString();
        gsettingsIni->setValue("DossierImagerie",NomDirStockageImagerie);
        setWindowTitle("RufusAdmin - " + tr("Monoposte") + " - " + gNomLieuExercice);
    }
    else if (gMode == ReseauLocal)
    {
        setWindowTitle("RufusAdmin - " + tr("Réseau local") + " - " + gNomLieuExercice);
    }
    else if (gMode == Distant)
    {
        setWindowTitle("RufusAdmin - " + tr("Accès distant crypté SSL") + " - " + gNomLieuExercice);
        ui->Exportframe         ->setVisible(false);
        ui->Diversframe         ->setVisible(false);
    }

    ui->Exportframe         ->setVisible(gMode != Distant);
    QString Base = (gMode== Distant? "BDD_DISTANT/" : "");
    ui->StockageupLineEdit->setText(gsettingsIni->value(Base + "DossierImagerie").toString());

    ReconstruitListeLieuxExercice();

    QString VerifBasereq = "select VersionBase from " NOM_TABLE_PARAMSYSTEME;
    QList<QList<QVariant>> listversion = db->StandardSelectSQL(VerifBasereq, ok);
    if (!ok || listversion.size()==0)
        ui->VersionBaselabel->setText(tr("Version de la base") + "\t<font color=\"red\"><b>" + tr("inconnue") + "</b></font>");
    else
        ui->VersionBaselabel->setText(tr("Version de la base ") + "<font color=\"green\"><b>" + listversion.at(0).at(0).toString() + "</b></font>");
    ui->VersionRufuslabel->setText(tr("Version de RufusAdmin ") + "<font color=\"green\"><b>" + qApp->applicationVersion() + "</b></font>");

    ui->Sauvegardeframe         ->setEnabled(gMode == Poste);
    ui->ParamSauvegardelabel    ->setVisible(gMode != Poste);
    ui->ParamSauvegardelabel    ->setEnabled(gMode != Poste);

    if (gMode == Poste)
    {
        QString reqBkup = "select LundiBkup, MardiBkup, MercrediBkup, JeudiBkup, VendrediBkup, SamediBkup, DimancheBkup, HeureBkup, DirBkup from " NOM_TABLE_PARAMSYSTEME;
        QList<QList<QVariant>> listBKup = db->StandardSelectSQL(reqBkup, ok);
        QString DirBkup = listBKup.at(0).at(8).toString();
        if (QDir(DirBkup).exists())
            ui->DirBackupuplineEdit->setText(DirBkup);
        if (listBKup.at(0).at(7).toTime().isValid())
            ui->HeureBackuptimeEdit->setTime(listBKup.at(0).at(7).toTime());
        ui->LundiradioButton    ->setChecked(listBKup.at(0).at(0).toInt()==1);
        ui->MardiradioButton    ->setChecked(listBKup.at(0).at(1).toInt()==1);
        ui->MercrediradioButton ->setChecked(listBKup.at(0).at(2).toInt()==1);
        ui->JeudiradioButton    ->setChecked(listBKup.at(0).at(3).toInt()==1);
        ui->VendrediradioButton ->setChecked(listBKup.at(0).at(4).toInt()==1);
        ui->SamediradioButton   ->setChecked(listBKup.at(0).at(5).toInt()==1);
        ui->DimancheradioButton ->setChecked(listBKup.at(0).at(6).toInt()==1);
        connect(ui->DirBackuppushButton,        &QPushButton::clicked,      this,   &RufusAdmin::Slot_ModifDirBackup);
        QList<QRadioButton*> listbutton2 = ui->JourSauvegardegroupBox->findChildren<QRadioButton*>();
        for (int i=0; i<listbutton2.size(); i++)
            connect(listbutton2.at(i),          &QPushButton::clicked,      this,   &RufusAdmin::Slot_ModifDateBackup);
        connect(ui->HeureBackuptimeEdit,        &QTimeEdit::timeChanged,    this,   &RufusAdmin::Slot_ModifDateBackup);
        connect(ui->EffacePrgSauvupPushButton,  &QPushButton::clicked,      this,   &RufusAdmin::Slot_EffacePrgSauvegarde);
        connect(ui->ImmediatBackupupPushButton, &QPushButton::clicked,      this,   &RufusAdmin::ImmediateBackup);
    }

    trayIconMenu = new QMenu();

    gRufusAdminTrayIcon = new QSystemTrayIcon();
    gRufusAdminTrayIcon->setContextMenu(trayIconMenu);
    gRufusAdminTrayIcon->setIcon(gIconRufusAdmin);
    gRufusAdminTrayIcon->setVisible(true);
    connect(trayIconMenu,   &QMenu::aboutToShow,    this,   &RufusAdmin::Slot_TrayIconMenu);
    ui->MessageupLabel->setText("");

    ImportDocsExtThread = new ImportDocsExternesThread(idAdminDocs, idlieuExercice, gMode != Distant);
    connect(ImportDocsExtThread,    SIGNAL(emitmsg(QStringList, int, bool)),    this,       SLOT(AfficheMessageImport(QStringList, int, bool)));
    connect(ImportDocsExtThread,    SIGNAL(emitmsg(QString)),                   TCPServer,  SLOT(envoyerATous(QString)));
    connect(&tim,                   &QTimer::timeout,                           this,       &RufusAdmin::ListeAppareils);
    tim             .setInterval(5000);
    Slot_ImportDocsExternes();

    initListeBanques();
    initListeMotifs();

    installEventFilter(this);
}

RufusAdmin::~RufusAdmin()
{
    delete ui;
}

void RufusAdmin::AfficheMessageImport(QStringList listmsg, int pause, bool bottom)
{
    dlg_message(listmsg, pause, bottom);
}

void RufusAdmin::ListeAppareils()
{
    QString req = "select distinct list.TitreExamen, list.NomAPPareil from " NOM_TABLE_APPAREILSCONNECTESCENTRE " appcon, " NOM_TABLE_LISTEAPPAREILS " list"
          " where list.idappareil = appcon.idappareil and idLieu = " + QString::number(idlieuExercice);
    //qDebug()<< req;
    bool ok;
    QList<QList<QVariant>> listdocs = db->StandardSelectSQL(req, ok);
    if (listdocs.size()>0)
        ImportDocsExtThread->RapatrieDocumentsThread(listdocs);
}

void RufusAdmin::closeEvent(QCloseEvent *)
{
    // on retire le poste de la variable posteimportdocs SQL
    setPosteImportDocs(false);
    // on retire Admin de la table des utilisateurs connectés
    QString req = "delete from " NOM_TABLE_USERSCONNECTES
                  " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                  " and idlieu = " + QString::number(idlieuExercice);
    db->StandardSQL(req);
}

void RufusAdmin::AskAppareil()
{
    DisconnectTimerInactive();
    gAskAppareil = new UpDialog(this);
    gAskAppareil->setModal(true);
    gAskAppareil->move(QPoint(x()+width()/2,y()+height()/2));
    gAskAppareil->setFixedWidth(400);
    gAskAppareil->setWindowTitle(tr("Choisissez un appareil"));
    QVBoxLayout *globallay = dynamic_cast<QVBoxLayout*>(gAskAppareil->layout());
    QHBoxLayout *lay = new QHBoxLayout;
    UpLabel *label = new UpLabel();
    label->setText("Nom de l'appareil");
    label->setFixedSize(120,25);
    lay->addWidget(label);
    lay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
    UpComboBox *upCombo = new UpComboBox();
    upCombo->insertItems(0,glistAppareils);
    upCombo->setFixedSize(260,32);
    upCombo->setChampCorrespondant("NomAppareil");
    lay->addWidget(upCombo);
    globallay->insertLayout(0,lay);
    globallay->setSizeConstraint(QLayout::SetFixedSize);
    gAskAppareil->AjouteLayButtons(UpDialog::ButtonOK);
    connect(gAskAppareil->OKButton,    SIGNAL(clicked(bool)), this, SLOT(Slot_EnregistreAppareil()));
    gAskAppareil->exec();
    upCombo->showPopup();
    ConnectTimerInactive();
}

QString RufusAdmin::getExpressionSize(double size)
{
    QString com = "Mo";
    if (size>1024)
    {
        com = "Go";
        size /= 1024;
        if (size>1024)
        {
            com = "To";
            size /= 1024;
        }
    }
    return QString::number(size,'f',2) + com;
}

/*!
 *  \brief AskBupRestore
 *  la fiche qui permet de paramètrer une opération de sauvegarde ou de restauration
 *  \param restore :            true = restauration - false = backup
 *  \param pathorigin :         le dossier de stockage de l'imagerie sur le serveur
 *  \param pathdestination :    le dossier où se trouve la backup
 *  \param OKini :              le rufus.ini est sauvegardé
 *  \param OKRssces :           les fichiers ressources sont sauvegardé
 *  \param OKimages :           les fichiers images sont sauvegardés
 *  \param OKvideos :           les fichiers videos sont sauvegardés
 *
 */
void RufusAdmin::AskBupRestore(bool Restore, QString pathorigin, QString pathdestination, bool OKini, bool OKRssces, bool OKimages, bool OKvideos)
{
    QMap<QString,double>      DataDir;
    // taille de la base de données ----------------------------------------------------------------------------------------------------------------------------------------------
    BaseSize = 0;
    if (Restore)
    {
        QStringList filters, listnomsfilestorestore;
        filters << "*.sql";
        for (int j=0; j<QDir(pathorigin).entryList(filters).size(); j++)
            listnomsfilestorestore << pathorigin + "/" + QDir(pathorigin).entryList(filters).at(j);
        for (int i=0; i<listnomsfilestorestore.size(); i++)
            BaseSize += QFile(listnomsfilestorestore.at(i)).size()/1024/1024;
    }
    else
        BaseSize = CalcBaseSize();
    ImagesSize = 0;
    VideosSize = 0;
    // espace libre sur le disque ------------------------------------------------------------------------------------------------------------------------------------------------
    FreeSpace = QStorageInfo(pathdestination).bytesAvailable();
    FreeSpace = FreeSpace/1024/1024;
    //qDebug() << QStorageInfo(ui->DirBackupuplineEdit->text()).bytesAvailable();
    //qDebug() << QString::number(FreeSpace,'f',0);

    gAskBupRestore = new UpDialog(this);
    gAskBupRestore->setModal(true);
    gAskBupRestore->move(QPoint(x()+width()/2,y()+height()/2));
    gAskBupRestore->setWindowTitle(Restore? tr("Dossiers à restaurer") : tr("Dossiers à sauvegarder"));
    QVBoxLayout *globallay = dynamic_cast<QVBoxLayout*>(gAskBupRestore->layout());
    int labelsize = 15;

    if (Restore)
    {
        QHBoxLayout *layini = new QHBoxLayout;
        UpLabel *labelini = new UpLabel();
        labelini->setVisible(false);
        labelini->setFixedSize(labelsize, labelsize);
        layini->addWidget(labelini);
        UpCheckBox *Inichk  = new UpCheckBox();
        Inichk->setText("fichier de paramètrage Rufus.ini");
        Inichk->setEnabled(OKini);
        Inichk->setChecked(OKini);
        Inichk->setAccessibleDescription("ini");
        layini->addWidget(Inichk);
        layini->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        globallay->insertLayout(0, layini);

        QHBoxLayout *layRssces = new QHBoxLayout;
        UpLabel *labelrssces = new UpLabel();
        labelrssces->setVisible(false);
        labelrssces->setFixedSize(labelsize, labelsize);
        layRssces->addWidget(labelrssces);
        UpCheckBox *Rssceschk  = new UpCheckBox();
        Rssceschk->setText("fichier ressources d'impression");
        Rssceschk->setEnabled(OKRssces);
        Rssceschk->setChecked(OKRssces);
        Rssceschk->setAccessibleDescription("ressources");
        layRssces->addWidget(Rssceschk);
        layRssces->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        globallay->insertLayout(0, layRssces);
    }
    if (OKvideos)
    {
        // taille du dossier video ---------------------------------------------------------------------------------------------------------------------------------------
        DataDir = Utils::dir_size(pathorigin + NOMDIR_VIDEOS);
        VideosSize = DataDir["Size"]/1024/1024;
        QHBoxLayout *layVideos = new QHBoxLayout;
        UpLabel *labeVideos = new UpLabel();
        labeVideos->setVisible(false);
        labeVideos->setFixedSize(labelsize, labelsize);
        layVideos->addWidget(labeVideos);
        UpCheckBox *Videoschk  = new UpCheckBox();
        Videoschk->setText("Videos");
        Videoschk->setEnabled(OKvideos || !Restore);
        Videoschk->setChecked(OKvideos || !Restore);
        Videoschk->setAccessibleDescription("videos");
        layVideos->addWidget(Videoschk);
        layVideos->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        UpLabel *lblvolvid = new UpLabel();
        lblvolvid->setText(getExpressionSize(VideosSize));
        layVideos->addWidget(lblvolvid);
        globallay->insertLayout(0, layVideos);
        connect(Videoschk, SIGNAL(clicked(bool)), this,    SLOT(Slot_CalcTimeBupRestore()));
    }
    if (OKimages)
    {
        // taille du dossier Images ---------------------------------------------------------------------------------------------------------------------------------------
        DataDir = Utils::dir_size(pathorigin + NOMDIR_IMAGES);
        ImagesSize = DataDir["Size"]/1024/1024;
        QHBoxLayout *layImges = new QHBoxLayout;
        UpLabel *labelmges = new UpLabel();
        labelmges->setVisible(false);
        labelmges->setFixedSize(labelsize, labelsize);
        layImges->addWidget(labelmges);
        UpCheckBox *Imgeschk  = new UpCheckBox();
        Imgeschk->setText("Images");
        Imgeschk->setEnabled(OKimages || !Restore);
        Imgeschk->setChecked(OKimages || !Restore);
        Imgeschk->setAccessibleDescription("images");
        layImges->addWidget(Imgeschk);
        layImges->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
        UpLabel *lblvolimg = new UpLabel();
        lblvolimg->setText(getExpressionSize(ImagesSize));
        layImges->addWidget(lblvolimg);
        globallay->insertLayout(0, layImges);
        connect(Imgeschk, SIGNAL(clicked(bool)), this,    SLOT(Slot_CalcTimeBupRestore()));
    }

    QHBoxLayout *layBDD = new QHBoxLayout;
    UpLabel *labelBDD = new UpLabel();
    labelBDD->setVisible(false);
    labelBDD->setFixedSize(labelsize, labelsize);
    layBDD->addWidget(labelBDD);
    UpCheckBox *BDDchk  = new UpCheckBox();
    BDDchk->setText("base de données");
    BDDchk->setChecked(true);
    BDDchk->setAccessibleDescription("base");
    layBDD->addWidget(BDDchk);
    layBDD->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
    UpLabel *lblvolbase = new UpLabel();
    lblvolbase->setText(getExpressionSize(BaseSize));
    layBDD->addWidget(lblvolbase);
    globallay->insertLayout(0, layBDD);


    QHBoxLayout *layResume = new QHBoxLayout;
    labelResume = new UpLabel();
    layResume->addWidget(labelResume);
    globallay->insertLayout(globallay->count()-1, layResume);

    QHBoxLayout *layVolumeLibre = new QHBoxLayout;
    labelVolumeLibre = new UpLabel();
    layVolumeLibre->addWidget(labelVolumeLibre);
    globallay->insertLayout(globallay->count()-1, layVolumeLibre);

    connect(BDDchk, SIGNAL(clicked(bool)), this,    SLOT(Slot_CalcTimeBupRestore()));

    //globallay->setSizeConstraint(QLayout::SetFixedSize);
    gAskBupRestore->setFixedWidth(400);
    gAskBupRestore->AjouteLayButtons(UpDialog::ButtonOK);
    connect(gAskBupRestore->OKButton,    SIGNAL(clicked(bool)), gAskBupRestore, SLOT(accept()));
    Slot_CalcTimeBupRestore();
}

void RufusAdmin::Slot_CalcTimeBupRestore()
{
    double time(0), volume(0);
    QList<UpCheckBox*> listchk = gAskBupRestore->findChildren<UpCheckBox*>();
    for (int i= 0; i<listchk.size(); i++)
    {
        if (listchk.at(i)->accessibleDescription() == "base")
        {
            if (listchk.at(i)->isChecked())
                volume += BaseSize;
        }
        if (listchk.at(i)->accessibleDescription() == "images")
        {
            if (listchk.at(i)->isChecked())
                volume += ImagesSize;
        }
        if (listchk.at(i)->accessibleDescription() == "videos")
        {
            if (listchk.at(i)->isChecked())
                volume += VideosSize;
        }
    }
    time = (volume/1024 /2)*60000; //duréée approximative de sauvegarde en ms
    QString Volumelitteral = getExpressionSize(volume);
    QString timelitteral;
    if (Volumelitteral.right(2) == "Go")
        timelitteral = QString::number(time/60000,'f',0) + tr(" minutes");
    else if (Volumelitteral.right(2) == "To")
        timelitteral = QString::number(time/60000/60,'f',0) + tr(" heures");
    else
        timelitteral = tr("moins d'une minute");
    QString color = FreeSpace>volume? "green": "red";
    QString msg = tr("Volume à transférer: ") + " <font color=\""+ color + "\"><b>" + Volumelitteral + "</b></font>";
    labelResume->setText(msg + " - " + tr("Temps estimé: ") + timelitteral);
    QString Volumelibre = getExpressionSize(FreeSpace);
    msg = tr("Volume disponible sur le disque: ") + " <font color=\""+ color + "\"><b>" + Volumelibre + "</b></font>";

    labelVolumeLibre->setText(msg);
    gAskBupRestore->OKButton->setEnabled(FreeSpace>volume);
}

void RufusAdmin::ConnectTimers()
{
    connect (gTimerUserConnecte,        SIGNAL(timeout()),      this,   SLOT(Slot_MetAJourLaConnexion()));
    connect (gTimerVerifDivers,         SIGNAL(timeout()),      this,   SLOT(Slot_VerifPosteImport()));
    connect (gTimerVerifDivers,         SIGNAL(timeout()),      this,   SLOT(Slot_VerifVersionBase()));
    connect (gTimerUserConnecte,        SIGNAL(timeout()),      this,   SLOT(Slot_ImportDocsExternes()));
    if (gMode != Distant)
    {
        connect (gTimerUserConnecte,    &QTimer::timeout,       this,   &RufusAdmin::ExporteDocs);
        connect (gTimerSupprDocs,       &QTimer::timeout,       this,   &RufusAdmin::SupprimerDocsEtFactures);
        connect (gTimerDocsAExporter,   SIGNAL(timeout()),      this,   SLOT(Slot_CalcExporteDocs()));
    }
    ConnectTimerInactive();
    connect (gTimerSalDatCorrespMsg,    &QTimer::timeout,       this,   &RufusAdmin::VerifModifsSalledAttenteCorrespondantsetNouveauxMessages);
    connect (gTimerVerifVerrou,         &QTimer::timeout,       this,   &RufusAdmin::VerifVerrouDossier);
}

void RufusAdmin::DisconnectTimers()
{
    disconnect (gTimerUserConnecte,     SIGNAL(timeout()),      this,   SLOT(Slot_MetAJourLaConnexion()));
    disconnect (gTimerSupprDocs,        &QTimer::timeout,       this,   &RufusAdmin::SupprimerDocsEtFactures);
    disconnect (gTimerVerifDivers,      SIGNAL(timeout()),      this,   SLOT(Slot_VerifPosteImport()));
    disconnect (gTimerVerifDivers,      SIGNAL(timeout()),      this,   SLOT(Slot_VerifVersionBase()));
    disconnect (gTimerUserConnecte,     SIGNAL(timeout()),      this,   SLOT(Slot_ImportDocsExternes()));
    disconnect (gTimerDocsAExporter,    SIGNAL(timeout()),      this,   SLOT(Slot_CalcExporteDocs()));
    disconnect (gTimerUserConnecte,     &QTimer::timeout,       this,   &RufusAdmin::ExporteDocs);
    DisconnectTimerInactive();
    gTimerSalDatCorrespMsg  ->disconnect();
    gTimerVerifVerrou       ->disconnect();
}

/*!
 * \brief RufusAdmin::CompressFileJPG(QByteArray ba, QString nomfileOK, QString nomfileEchec)
 * comprime un fichier jpg à une taille inférieure à celle de la macro TAILLEMAXIIMAGES
 * \param QString nomfileOK le nom du fichier d'origine utilisé aussi en cas d'échec pour faire le log
 * \param QDate datetransfert date utilisée en cas d'échec pour faire le log
 * \return true si réussi, false si échec de l'enregistrement du fichier
 * en cas d'échec
    * un fichier de log est utilisé ou créé au besoin dans le répertoire NOMDIR_ECHECSTRANSFERTS
    * et une ligne résumant l'échec est ajoutée en fin de ce fichier
    * le fichier d'origine est ajouté dans ce même répertoire
 */
bool RufusAdmin::CompressFileJPG(QString nomfile, QDate datetransfert)
{
    /* on vérifie si le dossier des echecs de transferts existe sur le serveur et on le crée au besoin*/
    QString NomDirStockageImagerie  = gsettingsIni->value("DossierImagerie").toString();
    QString CheminEchecTransfrDir   = NomDirStockageImagerie + NOMDIR_ECHECSTRANSFERTS;
    if (!Utils::mkpath(CheminEchecTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminEchecTransfrDir + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        dlg_message(listmsg, 3000, false);
        return false;
    }
    /* on vérifie si le dossier provisoire existe sur le poste et on le crée au besoin*/
    QString DirStockProvPath = NomDirStockageImagerie + NOMDIR_PROV;
    if (!Utils::mkpath(DirStockProvPath))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + DirStockProvPath + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        dlg_message(listmsg, 3000, false);
        return false;
    }

    QFile CC(nomfile);
    double sz = CC.size();
    if (sz < TAILLEMAXIIMAGES)
        return true;
    QImage  img(nomfile);
    QString filename = QFileInfo(nomfile).fileName();
    QString nomfichresize = DirStockProvPath + "/" + filename;
    QFile fileresize(nomfichresize);
    if (fileresize.exists())
        fileresize.remove();
    QFile echectrsfer(CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfert.toString("yyyy-MM-dd") + ".txt");
    QPixmap pixmap;
    double w = img.width();
    double h = img.height();
    int x = img.width();
    if (int(w*h)>(4096*1024)) // si l'image dépasse 4 Mpixels, on la réduit en conservant les proportions
    {
        double proportion = w/h;
        int y = int(sqrt((4096*1024)/proportion));
        x = int (y*proportion);
    }
    pixmap = pixmap.fromImage(img.scaledToWidth(x,Qt::SmoothTransformation));
    /* on enregistre le fichier sur le disque du serveur
     * si on n'y arrive pas,
        * on crée le fichier log des echecstransferts correspondants dans le répertoire des echecs de transfert sur le serveur
        * on complète ce fichier en ajoutant une ligne correspondant à cet échec
        * on enregistre dans ce dossier une copie du fichier d'origine
     */
    if (!pixmap.save(nomfichresize, "jpeg"))
    {
        if (echectrsfer.open(QIODevice::Append))
        {
            QTextStream out(&echectrsfer);
            out << CC.fileName() << "\n" ;
            echectrsfer.close();
            CC.copy(CheminEchecTransfrDir + "/" + filename);
        }
        return false;
    }
    CC.remove();
    /* on comprime*/
    int tauxcompress = 90;
    while (sz > TAILLEMAXIIMAGES && tauxcompress > 1)
    {
        pixmap.save(nomfichresize, "jpeg",tauxcompress);
        sz = fileresize.size();
        tauxcompress -= 10;
    }
    fileresize.copy(nomfile);
    fileresize.close();
    fileresize.remove();
    return true;
}

void RufusAdmin::ConnectTimerInactive()
{
    connect(ui->MiseEnVeilleprogressBar, &QProgressBar::valueChanged, this, [=]
    {
        if (ui->MiseEnVeilleprogressBar->value() == ui->MiseEnVeilleprogressBar->minimum())
            Slot_MasqueAppli();
    });
}

void RufusAdmin::DisconnectTimerInactive()
{
    ui->MiseEnVeilleprogressBar->disconnect();
}

double RufusAdmin::CalcBaseSize()
{
    double basesize = 0;
    QString sizereq = "SELECT SUM(SizeMB) from "
                      "(SELECT table_schema, round(sum(data_length+index_length)/1024/1024,4) AS SizeMB FROM information_schema.tables"
                      " where table_schema = 'ComptaMedicale'"
                      " or table_schema = 'Ophtalmologie'"
                      " or table_schema = 'rufus'"
                      " GROUP BY table_schema)"
                      " as bdd";
    QList<QVariant> basedata = db->getFirstRecordFromStandardSelectSQL(sizereq,ok);
    if (ok && basedata.size()>0)
        basesize = basedata.at(0).toDouble();
    return basesize;
}

void RufusAdmin::ConnexionBase()
{
    DlgParam = new dlg_paramconnexion();
    if (DlgParam->exec()>0)
        db = DataBase::getInstance();
    else
        exit(0);
    gMode = db->getMode();
}

/*-----------------------------------------------------------------------------------------------------------------
    -- Détermination du lieu exercice pour la session en cours -------------------------------------------------------------
    -----------------------------------------------------------------------------------------------------------------*/
int RufusAdmin::DetermineLieuExercice()
{
    int idLieu = 1;
    if (gMode == Distant)
    {
        QString lieuxreq = "select idLieu, NomLieu from " NOM_TABLE_LIEUXEXERCICE
                           " where idLieu <> (select idLieuParDefaut from " NOM_TABLE_PARAMSYSTEME ")";
        QList<QList<QVariant>> listlieux = db->StandardSelectSQL(lieuxreq, ok);
        if (listlieux.size()==1)
            idLieu = listlieux.at(0).at(0).toInt();
        else if (listlieux.size()>1)
        {
            DisconnectTimerInactive();
            UpDialog *gAskLieux     = new UpDialog();
            gAskLieux               ->AjouteLayButtons();
            QVBoxLayout *globallay  = dynamic_cast<QVBoxLayout*>(gAskLieux->layout());
            QGroupBox*boxlieux      = new QGroupBox();
            globallay               ->insertWidget(0,boxlieux);
            boxlieux                ->setAccessibleName("Parent");
            boxlieux                ->setTitle(tr("D'où vous connectez-vous?"));

            QFontMetrics fm         = QFontMetrics(qApp->font());
            int hauteurligne        = int(fm.height()*1.6);
            boxlieux                ->setFixedHeight(((listlieux.size() + 1)*hauteurligne)+5);
            QVBoxLayout *vbox       = new QVBoxLayout;
            for (int i=0; i<listlieux.size(); i++)
            {
                UpRadioButton   *pradiobutt = new UpRadioButton(boxlieux);
                pradiobutt      ->setText(listlieux.at(i).at(1).toString());
                pradiobutt      ->setAccessibleName(listlieux.at(i).at(0).toString());
                QString data ("");
                if (listlieux.at(i).at(1).toString()!="")
                    data += listlieux.at(i).at(1).toString();
                if (data == "" )
                {
                    data += listlieux.at(i).at(2).toString();
                    if (listlieux.at(i).at(6).toString()!="")
                        data += (data != ""? " " : "") + listlieux.at(i).at(6).toString();
                }
                if (listlieux.at(i).at(6).toString()!="")
                    data += (data != ""? " - " : "") + listlieux.at(i).at(6).toString();
                data = "";
                if (listlieux.at(i).at(1).toString()!="")
                    data += listlieux.at(i).at(1).toString();
                if (listlieux.at(i).at(2).toString()!="")
                    data += (data != ""? "\n" : "") + listlieux.at(i).at(2).toString();
                if (listlieux.at(i).at(3).toString()!="")
                    data += (data != ""? "\n" : "") + listlieux.at(i).at(3).toString();
                if (listlieux.at(i).at(4).toString()!="")
                    data += (data != ""? "\n" : "") + listlieux.at(i).at(4).toString();
                if (listlieux.at(i).at(5).toString()!="")
                    data += (data != ""? "\n" : "") + listlieux.at(i).at(5).toString();
                if (listlieux.at(i).at(6).toString()!="")
                    data += (data != ""? " " : "") + listlieux.at(i).at(6).toString();
                if (listlieux.at(i).at(7).toString()!="")
                    data += (data != ""? "\nTel: " : "Tel: ") + listlieux.at(i).at(7).toString();
                pradiobutt      ->setImmediateToolTip(data);
                pradiobutt      ->setChecked(i==0);
                vbox            ->addWidget(pradiobutt);
            }
            vbox                ->setContentsMargins(8,0,8,0);
            boxlieux            ->setLayout(vbox);
            gAskLieux           ->setModal(true);
            globallay           ->setSizeConstraint(QLayout::SetFixedSize);
            connect(gAskLieux->OKButton,   SIGNAL(clicked(bool)),  gAskLieux, SLOT(accept()));
            gAskLieux->exec();
            QList<QRadioButton*> listbutt = boxlieux->findChildren<QRadioButton*>();
            for (int j=0; j<listbutt.size(); j++)
                if (listbutt.at(j)->isChecked())
                    idLieu = listbutt.at(j)->accessibleName().toInt();
            delete gAskLieux;
            ConnectTimerInactive();
        }
    }
    else
    {
        QString lieuxreq = "select idLieuParDefaut from " NOM_TABLE_PARAMSYSTEME;
        QList<QList<QVariant>> listlieux = db->StandardSelectSQL(lieuxreq, ok);
        if (listlieux.at(0).at(0).toInt()>=1)
            idLieu = listlieux.at(0).at(0).toInt();
        else
        {
            QList<QList<QVariant>> listliux = db->StandardSelectSQL("select min(idlieu) from " NOM_TABLE_LIEUXEXERCICE, ok);
            idLieu = listliux.at(0).at(0).toInt();
            db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set idLieuParDefaut = " + listliux.at(0).at(0).toString());
        }
    }
    return idLieu;
}

bool RufusAdmin::eventFilter(QObject *obj, QEvent *event)
{
    if (!event->spontaneous() && event->type() != QEvent::UpdateRequest)
    {
        //qDebug() << "event = " << event;
        ui->MiseEnVeilleprogressBar->setValue(dureeVeille);
    }
    return QWidget::eventFilter(obj, event);
}

QString RufusAdmin::getDossierDocuments(QString Appareil)
{
    return gsettingsIni->value("DossierEchangeImagerie/" + Appareil).toString();
}

void RufusAdmin::NouvAppareil()
{
    AskAppareil();
    delete gAskAppareil;
}

void RufusAdmin::SupprAppareil()
{
    if (ui->AppareilsConnectesupTableWidget->selectedItems().size()==0)
        return;
    QString req = " select list.TitreExamen, list.NomAppareil from " NOM_TABLE_LISTEAPPAREILS " list, " NOM_TABLE_APPAREILSCONNECTESCENTRE " appcon"
                  " where list.idAppareil = appcon.idappareil"
                  " and list.idappareil = " + ui->AppareilsConnectesupTableWidget->selectedItems().at(0)->text();
    QList<QList<QVariant>> listapps = db->StandardSelectSQL(req, ok);
    if (!ok)
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
        req = "delete from " NOM_TABLE_APPAREILSCONNECTESCENTRE " where idAppareil = "
                + ui->AppareilsConnectesupTableWidget->selectedItems().at(0)->text()
                + " and idLieu = " + QString::number(idlieuExercice);
        db->StandardSQL(req);
        gsettingsIni->remove("DossierEchangeImagerie/" + listapps.at(0).at(1).toString());
        Remplir_Table();
    }
}

void RufusAdmin::RestoreFontAppli()
{
    QFont AppFont = QFont(POLICEPARDEFAUT);
    AppFont.setPointSize(POINTPARDEFAUT);
    for (int i = 5; i < 30; i++)
    {
        AppFont.setPointSize(i);
        QFontMetrics fm(AppFont);
        int Htaille = fm.width("date de naissance");
        if (Htaille > 108 || fm.height()*1.1 > 20)
        {
            AppFont.setPointSize(i-1);
            i=30;
        }
    }
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

    QString  Remplirtablerequete = "SELECT list.idAppareil, list.TitreExamen, list.NomAppareil, Format"
              " FROM "  NOM_TABLE_APPAREILSCONNECTESCENTRE " appcon , " NOM_TABLE_LISTEAPPAREILS " list"
              " where list.idappareil = appcon.idappareil"
              " and idLieu = " + QString::number(idlieuExercice) +
              " ORDER BY TitreExamen";
    QList<QList<QVariant>> listapps = db->StandardSelectSQL(Remplirtablerequete, ok);
    if (!ok)
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
        line4->setRowTable(i);
        line4->setFocusPolicy(Qt::NoFocus);
        line4->setStyleSheet("UpLineEdit {background-color:white; border: 0px solid rgb(150,150,150);border-radius: 0px;}"
                              "UpLineEdit:focus {border: 0px solid rgb(164, 205, 255);border-radius: 0px;}");
        connect(line4,                    SIGNAL(TextModified(QString)),         this,   SLOT(Slot_EnregDossierStockageApp(QString)));
        ui->AppareilsConnectesupTableWidget->setCellWidget(i,col,line4);

        col++; //5                                                                    // bouton
        QPixmap pix = giconSortirDossier.pixmap(QSize(15,15));
        QIcon ic;
        ic.addPixmap(pix);
        dossbouton->setIcon(ic);
        dossbouton->setId(listapps.at(i).at(0).toInt());
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
        connect(dossbouton,       SIGNAL(clicked(bool)), this   ,SLOT(Slot_ChoixDossierStockageApp()));

        ui->AppareilsConnectesupTableWidget->setRowHeight(i,int(fm.height()*1.3));
    }

    glistAppareils.clear();
    QString req = "select NomAppareil from " NOM_TABLE_LISTEAPPAREILS
                  " where idAppareil not in (select idAppareil from " NOM_TABLE_APPAREILSCONNECTESCENTRE " where idlieu = " + QString::number(idlieuExercice) + ")";
    QList<QList<QVariant>> listnomsapps = db->StandardSelectSQL(req, ok);
    if (listnomsapps.size() == 0)
        widgAppareils->plusBouton->setEnabled(false);
    else
        for (int i=0; i<listnomsapps.size(); i++)
            glistAppareils << listnomsapps.at(i).at(0).toString();
    widgAppareils->moinsBouton->setEnabled(listapps.size()>0);
}

void RufusAdmin::setPosteImportDocs(bool a)
{
    if (gMode == Distant)
        return;
    // Il n'y pas de variables utilisateur globale dans MySQL, on est donc obligé de passer par une procédure stockée pour en simuler une
    // pour créer une procédure avec Qt, séparer le drop du create, ne pas utiliser les délimiteurs et utiliser les retours à la ligne \n\.......

    // si a = true, on se met en poste importateur - Admin
    // si a = false, on retire le poste en cours et on met NULL à la place.

    QString IpAdress("NULL");
    QString req = "USE `" NOM_BASE_CONSULTS "`;";
    db->StandardSQL(req);

    req = "DROP PROCEDURE IF EXISTS " NOM_POSTEIMPORTDOCS ";";
    db->StandardSQL(req);

    if (a)
        IpAdress = QHostInfo::localHostName() + " - " NOM_ADMINISTRATEURDOCS;
    req = "CREATE PROCEDURE " NOM_POSTEIMPORTDOCS "()\n\
          BEGIN\n\
          SELECT '" + IpAdress + "';\n\
          END ;";
    db->StandardSQL(req);
    if (a)
        Slot_MetAJourLaConnexion();
}

void RufusAdmin::Slot_ChoixButtonFrame(int i)
{
    switch (i) {
    case 1:
        NouvAppareil();
        break;
    case -1:
        SupprAppareil();
        break;
    default:
        break;
    }
}

void RufusAdmin::Slot_ChoixDossierStockageApp()
{
    UpPushButton *bout = static_cast<UpPushButton*>(sender());
    QString req = "select TitreExamen, NomAppareil from " NOM_TABLE_LISTEAPPAREILS " where idAppareil = " + QString::number(bout->getId());
    QString exam = "";
    QList<QVariant> examdata = db->getFirstRecordFromStandardSelectSQL(req, ok);
    if (!ok)
        return;
    if (examdata.size()>0)
        exam = examdata.at(1).toString();
    QString dir = getDossierDocuments(exam);
    if (dir == "")
        dir = QDir::homePath() + NOMDIR_RUFUS;
    QFileDialog dialog(this, "", dir);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);
    DisconnectTimerInactive();
    if (dialog.exec()>0)
    {
        QDir dockdir = dialog.directory();
        int row;
        UpLineEdit *line = Q_NULLPTR;
        row = ui->AppareilsConnectesupTableWidget->findItems(QString::number(bout->getId()), Qt::MatchExactly).at(0)->row();
        line    = dynamic_cast<UpLineEdit*>(ui->AppareilsConnectesupTableWidget->cellWidget(row,4));
        if (line!=Q_NULLPTR)
            line->setText(dockdir.path());
        gsettingsIni->setValue("DossierEchangeImagerie/" + exam, dockdir.path());
    }
    ConnectTimerInactive();
}

void RufusAdmin::Slot_EnregistreAppareil()
{
    if (!gAskAppareil) return;
    QString req = "insert into " NOM_TABLE_APPAREILSCONNECTESCENTRE " (idAppareil, idLieu) Values("
                  " (select idappareil from " NOM_TABLE_LISTEAPPAREILS " where NomAppareil = '" + gAskAppareil->findChildren<UpComboBox*>().at(0)->currentText() + "'), "
                  + QString::number(idlieuExercice) + ")";
    db->StandardSQL(req);
    gAskAppareil->done(0);
    Remplir_Table();
}

void RufusAdmin::Slot_ModifDirImagerie()
{
    QString dir = ui->StockageupLineEdit->text();
    if (dir == "")
        dir = QDir::homePath() + NOMDIR_RUFUS;
    QFileDialog dialog(this, "", dir);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);
    if (dialog.exec()>0)
    {
        QDir dockdir = dialog.directory();
        if (gMode==Poste)
            if (!dockdir.match(QDir::homePath() + NOMDIR_RUFUS "/*", dockdir.path()))
            {
                UpMessageBox::Watch(this, tr("Vous devez choisir un sous-dossier du dossier Rufus"), QDir::homePath() + NOMDIR_RUFUS);
                return;
            }
        ui->StockageupLineEdit->setText(dockdir.path());
        QString Base = (gMode==Distant? "BDD_DISTANT/" : "");
        gsettingsIni->setValue(Base + "DossierImagerie", dockdir.path());
        if (gMode==Poste)
            db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set dirImagerie = '" + dockdir.path() + "'");
    }
}

void RufusAdmin::Slot_EnregDossierStockageApp(QString dir)
{
    UpLineEdit *line    = dynamic_cast<UpLineEdit*>(sender());
    if (line==Q_NULLPTR) return;
    if (!QDir(dir).exists() && dir != "")
    {
        QString textline = line->getValeurAvant();
        UpMessageBox::Watch(this,tr("Repertoire invalide!"));
        line->setText(textline);
        return;
    }
    QString id;
    id = ui->AppareilsConnectesupTableWidget->item(line->getRowTable(),0)->text();
    QString req = "select NomAppareil from " NOM_TABLE_LISTEAPPAREILS " where idAppareil = " + id;
    QString exam = "";
    QList<QVariant> examdata = db->getFirstRecordFromStandardSelectSQL(req, ok, tr("Impossible de retrouver le nom de l'appareil"));
    if (!ok)
        return;
    if (examdata.size()>0)
        exam = examdata.at(0).toString();
    if (exam != "")
        gsettingsIni->setValue("DossierEchangeImagerie/" + exam, dir);
}

void RufusAdmin::Slot_EnregistreEmplacementServeur(int idx)
{
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set idlieupardefaut = " + ui->EmplacementServeurupComboBox->itemData(idx).toString());
}

void RufusAdmin::Slot_EnregistreNouvMDPAdmin()
{
    if (gAskMDP != Q_NULLPTR)
    {
        // Vérifier la cohérence
        QString anc, nouv, confirm;
        UpMessageBox msgbox(this);
        msgbox.setText(tr("Erreur"));
        msgbox.setIcon(UpMessageBox::Warning);
        UpSmallButton *OKBouton = new UpSmallButton;
        OKBouton->setText("OK");
        msgbox.addButton(OKBouton, UpSmallButton::STARTBUTTON);
        anc         = gAskMDP->findChild<UpLineEdit*>(gAncMDP)->text();
        nouv        = gAskMDP->findChild<UpLineEdit*>(gNouvMDP)->text();
        confirm     = gAskMDP->findChild<UpLineEdit*>(gConfirmMDP)->text();
        QRegExp  rxMdp           = QRegExp("^[a-zA-Z0-9]{3,15}$");

        if (anc == "")
        {
            QSound::play(NOM_ALARME);
            msgbox.setInformativeText(tr("Ancien mot de passe requis"));
            gAskMDP->findChild<UpLineEdit*>(gAncMDP)->setFocus();
            msgbox.exec();
            return;
        }
        if (anc != db->getDataBase().password())
        {
            QSound::play(NOM_ALARME);
            msgbox.setInformativeText(tr("Le mot de passe que vous voulez modifier n'est pas le bon\n"));
            gAskMDP->findChild<UpLineEdit*>(gAncMDP)->setFocus();
            msgbox.exec();
            return;
        }
        if (!rxMdp.exactMatch(nouv) || nouv == "")
        {
            QSound::play(NOM_ALARME);
            msgbox.setInformativeText(tr("Le nouveau mot de passe n'est pas conforme\n(au moins 3 caractères - chiffres ou lettres non accentuées -\n"));
            gAskMDP->findChild<UpLineEdit*>(gNouvMDP)->setFocus();
            msgbox.exec();
            return;
        }
        if (nouv != confirm)
        {
            QSound::play(NOM_ALARME);
            msgbox.setInformativeText("Les mots de passe ne correspondent pas\n");
            gAskMDP->findChild<UpLineEdit*>(gNouvMDP)->setFocus();
            msgbox.exec();
            return;
        }
        msgbox.setText(tr("Modifications enregistrées"));
        msgbox.setInformativeText(tr("Le nouveau mot de passe a été enregistré avec succès"));
        QString req = "update " NOM_TABLE_PARAMSYSTEME " set MDPAdmin = '" + nouv + "'";
        db->StandardSQL(req);
        // Enregitrer le nouveau MDP de la base
        req = "update " NOM_TABLE_UTILISATEURS " set userMDP = '" + nouv + "' where idUser = " + QString::number(idAdminDocs);
        db->StandardSQL(req);
        // Enregitrer le nouveau MDP de connexion à MySQL
        req = "set password for '" NOM_ADMINISTRATEURDOCS "'@'localhost' = '" + nouv + "'";
        db->StandardSQL(req);
        QString AdressIP;
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                 AdressIP = address.toString();
        }
        QString Domaine;
        QStringList listIP = AdressIP.split(".");
        for (int i=0;i<listIP.size()-1;i++)
            Domaine += listIP.at(i) + ".";
        req = "set password for '" NOM_ADMINISTRATEURDOCS "'@'" + Domaine + "%' = '" + nouv + "'";
        db->StandardSQL(req);
        req = "set password for '" NOM_ADMINISTRATEURDOCS "SSL'@'%' = '" + nouv + "'";
        db->StandardSQL(req);
        gAskMDP->done(0);
        msgbox.exec();
    }
}

void RufusAdmin::setMapDatas()
{
    gmapIcons["OK"]             = giconOK;
    gmapIcons["Annul"]          = giconAnnul;
    gmapIcons["Euro"]           = giconEuro;
    gmapIcons["EuroCount"]      = giconEuroCount;
    gmapIcons["FermeAppuye"]    = giconFermeAppuye;
    gmapIcons["FermeRelache"]   = giconFermeRelache;
    gmapIcons["Help"]           = giconHelp;
    gmapIcons["Null"]           = giconNull;
}

QMap<QString, QIcon> RufusAdmin::MapIcons()
{
    return gmapIcons;
}

void RufusAdmin::Slot_CalcExporteDocs()
{
    if (gMode == Distant)
        return;
    QString totreq = "SELECT idimpression FROM " NOM_TABLE_IMPRESSIONS " where jpg is not null or pdf is not null limit 1";
    //qDebug() << totreq;
    ui->ExportImagespushButton->setEnabled(db->StandardSelectSQL(totreq, ok).size()>0);
}

void RufusAdmin::ExporteDocs()
{
    bool ok;
    QString NomDirStockageImagerie  = gsettingsIni->value("DossierImagerie").toString();
    if (!QDir(NomDirStockageImagerie).exists() || NomDirStockageImagerie == "")
    {
        QString msg = tr("Le dossier de sauvegarde d'imagerie") + " <font color=\"red\"><b>" + NomDirStockageImagerie + "</b></font>" + tr(" n'existe pas");
        msg += "<br />" + tr("Renseignez un dossier valide dans") + " <font color=\"green\"><b>" + tr("Emplacement de stockage des documents archivés") + "</b></font>";
        Message(msg, 6000, false);
        return;
    }
    QString CheminEchecTransfrDir   = NomDirStockageImagerie + NOMDIR_ECHECSTRANSFERTS;
    if (!Utils::mkpath(CheminEchecTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminEchecTransfrDir + "</b></font>" + tr(" invalide");
        Message(msg, 3000, false);
        return;
    }
    int total = db->StandardSelectSQL("SELECT idimpression FROM " NOM_TABLE_IMPRESSIONS " where jpg is not null or pdf is not null",ok).size();
    total +=    db->StandardSelectSQL("SELECT idFacture FROM " NOM_TABLE_FACTURES " where jpg is not null or pdf is not null", ok).size();
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
            disconnect (gTimerDocsAExporter,    SIGNAL(timeout()),      this,   SLOT(Slot_CalcExporteDocs()));
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
    QString CheminOKTransfrDir      = NomDirStockageImagerie + NOMDIR_IMAGES;
    QDir DirTrsferOK;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        Message(msg, 3000, false);
        return;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES JPG
    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString req = "SELECT idimpression, idpat, SousTypeDoc, Dateimpression, jpg, lienversfichier, typedoc FROM " NOM_TABLE_IMPRESSIONS " where jpg is not null";
    //qDebug() << req;
    QList<QList<QVariant>> listexportjpg = db->StandardSelectSQL(req, ok );
    if (ok)
        for (int i=0; i<listexportjpg.size(); i++)
        {
            /* si le lien vers le fichier est valide, on efface le champ jpg et on passe à la réponse suivante*/
            if (listexportjpg.at(i).at(5).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOMDIR_IMAGES + listexportjpg.at(i).at(5).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL ("update " NOM_TABLE_IMPRESSIONS " set jpg = null where idimpression = " + listexportjpg.at(i).at(0).toString() );
                    continue;
                }
            }
            QDate datetransfer    = listexportjpg.at(i).at(3).toDate();
            CheminOKTransfrDir    = CheminOKTransfrDir + "/" + datetransfer.toString("yyyy-MM-dd");
            if (!Utils::mkpath(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                QStringList listmsg;
                listmsg << msg;
                dlg_message(listmsg, 3000, false);
                return;
            }
            QString NomFileDoc = listexportjpg.at(i).at(1).toString() + "_" + listexportjpg.at(i).at(6).toString() + "-"
                    + listexportjpg.at(i).at(2).toString().replace("/",".") + "_"
                    + listexportjpg.at(i).at(3).toDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("HHmmss")
                    + "-" + listexportjpg.at(i).at(0).toString();
            QString CheminOKTransfrDoc  = CheminOKTransfrDir + "/" + NomFileDoc + "." JPG;
            QString CheminOKTransfrProv = CheminOKTransfrDir + "/" + NomFileDoc + "prov." JPG;
            QByteArray ba = listexportjpg.at(i).at(6).toByteArray();
            QPixmap pix;
            pix.loadFromData(ba);
            /*
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
                qDebug() << "erreur";
                return;
            }
            if (!CompressFileJPG(CheminOKTransfrProv))
            {
                db->SupprRecordFromTable(listexportjpg.at(i).at(0).toInt(), "idFacture", NOM_TABLE_FACTURES);
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
            db->StandardSQL("update " NOM_TABLE_IMPRESSIONS " set jpg = null,"
                                                            " lienversfichier = '/" + datetransfer.toString("yyyy-MM-dd") + "/" + Utils::correctquoteSQL(NomFileDoc) +
                            "' where idimpression = " + listexportjpg.at(i).at(0).toString());
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
            dlg_message(listmsg, 10);
        }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES PDF
    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString reqpdf = "SELECT idimpression, idpat, SousTypeDoc, Dateimpression, pdf, lienversfichier, compression, typedoc FROM " NOM_TABLE_IMPRESSIONS " where pdf is not null";
    QList<QList<QVariant>> listexportpdf = db->StandardSelectSQL(reqpdf, ok );
    if (ok)
        for (int i=0; i<listexportpdf.size(); i++)
        {
            if (listexportpdf.at(i).at(5).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOMDIR_IMAGES + listexportpdf.at(i).at(5).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL ("update " NOM_TABLE_IMPRESSIONS " set pdf = null where idimpression = " + listexportpdf.at(i).at(0).toString());
                    continue;
                }
            }
            QDate datetransfer    = listexportpdf.at(i).at(3).toDate();
            CheminOKTransfrDir      = CheminOKTransfrDir + "/" + datetransfer.toString("yyyy-MM-dd");
            if (!Utils::mkpath(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                QStringList listmsg;
                listmsg << msg;
                dlg_message(listmsg, 3000, false);
                return;
            }
            QString NomFileDoc = listexportpdf.at(i).at(1).toString() + "_" + listexportpdf.at(i).at(7).toString() + "-"
                    + listexportpdf.at(i).at(2).toString().replace("/",".") + "_"
                    + listexportpdf.at(i).at(3).toDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("HHmmss")
                    + "-" + listexportpdf.at(i).at(0).toString()  + "." PDF;
            QString CheminOKTransfrDoc = CheminOKTransfrDir + "/" + NomFileDoc;

            QByteArray bapdf;
            bapdf.append(listexportpdf.at(i).at(4).toByteArray());

            Poppler::Document* document = Poppler::Document::loadFromData(bapdf);
            if (!document || document->isLocked() || document == Q_NULLPTR)
            {
                QStringList listmsg;
                listmsg << tr("Impossible de charger le document ") + NomFileDoc;
                dlg_message(listmsg, 3000, false);
                QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer.toString("yyyy-MM-dd") + ".txt";
                QFile   echectrsfer(echectrsfername);
                if (echectrsfer.open(QIODevice::Append))
                {
                    QTextStream out(&echectrsfer);
                    out << NomFileDoc << "\n" ;
                    echectrsfer.close();
                    QFile CD(CheminEchecTransfrDir + "/" + NomFileDoc);
                    if (CD.open(QIODevice::Append))
                    {
                        QTextStream out(&CD);
                        out << listexportpdf.at(i).at(4).toByteArray() ;
                    }
                }
                QString delreq = "delete from  " NOM_TABLE_IMPRESSIONS " where idimpression = " + listexportpdf.at(i).at(0).toString();
                //qDebug() << delreq;
                db->StandardSQL (delreq);
                delete document;
                continue;
            }
            Poppler::PDFConverter *doctosave = document->pdfConverter();
            doctosave->setOutputFileName(CheminOKTransfrDoc);
            doctosave->convert();

            QFile CC(CheminOKTransfrDoc);
            CC.open(QIODevice::ReadWrite);
            CC.setPermissions(QFileDevice::ReadOther
                              | QFileDevice::ReadGroup
                              | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                              | QFileDevice::ReadUser   | QFileDevice::WriteUser);
            CC.close();
            db->StandardSQL ("update " NOM_TABLE_IMPRESSIONS " set pdf = null, compression = null,"
                                                             " lienversfichier = '/" + datetransfer.toString("yyyy-MM-dd") + "/" + Utils::correctquoteSQL(NomFileDoc)  + "'"
                                                                                                                                                                         " where idimpression = " + listexportpdf.at(i).at(0).toString());
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
            dlg_message(listmsg, 10);
        }
    int totdoc = listexportjpg.size() + listexportpdf.size();
    if (totdoc > 0)
    {
        listmsg <<  tr("export terminé") << QString::number(totdoc) + (totdoc>1? tr(" documents exportés en ") : tr(" document exporté en "))  + duree;
        dlg_message(listmsg, 3000);
    }



/* LES FACTURES  ============================================*/

    faits = 0;
    debut = QTime::currentTime();
    listmsg.clear();
    duree = "";
    CheminOKTransfrDir  = NomDirStockageImagerie + NOMDIR_FACTURES;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        dlg_message(listmsg, 3000, false);
        return;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES JPG
    //-----------------------------------------------------------------------------------------------------------------------------------------
    req = "SELECT idFacture, DateFacture, LienFichier, Intitule, Echeancier, idDepense, jpg FROM " NOM_TABLE_FACTURES
            " where jpg is not null";
    //qDebug() << req;
    QList<QList<QVariant>> listexportjpgfact = db->StandardSelectSQL(req, ok);
    if (ok)
        for (int i=0; i<listexportjpgfact.size(); i++)
        {
            /* si le lien vers le fichier est valide, on efface le champ jpg et on passe à la réponse suivante*/
            if (listexportjpgfact.at(i).at(2).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOMDIR_FACTURES + listexportjpgfact.at(i).at(2).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL("update " NOM_TABLE_FACTURES " set jpg = null where idfacture = " + listexportjpgfact.at(i).at(0).toString());
                    continue;
                }
            }
            /* nommage d'un fichier facture
             * idFacture + "_" + "ECHEANCIER ou FACTURE" + "_" + Intitule + "_" + DateFacture + ( + "_" + iddepense si facture et pas échéancier)
             */
            QDate datetransfer  = listexportjpgfact.at(i).at(1).toDate();
            QString user;

            QString NomFileDoc = listexportjpgfact.at(i).at(0).toString() + "_"
                    + (listexportjpgfact.at(i).at(4).toInt()==1? ECHEANCIER : FACTURE) + "-"
                    + listexportjpgfact.at(i).at(3).toString().replace("/",".") + "_"
                    + datetransfer.toString("yyyyMMdd");
            // on recherche le user à l'origine de cette facture
            QList<QList<QVariant>> Listeusr;
            if (listexportjpgfact.at(i).at(4).toInt()==1)          // c'est un échéancier
                req = "select dep.idUser, UserLogin from " NOM_TABLE_DEPENSES " dep, " NOM_TABLE_UTILISATEURS " usr"
                                                                                                              " where dep.idUser  = usr.idUser"
                                                                                                              " and idFacture = " + listexportjpgfact.at(i).at(0).toString();
            else                                                // c'est une facture, l'iduser est dans la table
                req = "select dep.idUser, UserLogin from " NOM_TABLE_DEPENSES " dep, " NOM_TABLE_UTILISATEURS " usr"
                                                                                                              " where dep.idUser  = usr.idUser"
                                                                                                              " and idDep = " + listexportjpgfact.at(i).at(5).toString();
            Listeusr = db->StandardSelectSQL(req, ok);
            if (Listeusr.size()==0) // il n'y a aucune depense enregistrée pour cette facture, on la détruit
            {
                db->SupprRecordFromTable(listexportjpgfact.at(i).at(0).toInt(), "idFacture", NOM_TABLE_FACTURES);
                continue;
            }
            user = Listeusr.at(0).at(1).toString();
            if (listexportjpgfact.at(i).at(4).toInt()!=1)
                NomFileDoc += "-"+listexportjpgfact.at(i).at(5).toString();

            CheminOKTransfrDir  = CheminOKTransfrDir + "/" + user;
            if (!Utils::mkpath(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                QStringList listmsg;
                listmsg << msg;
                dlg_message(listmsg, 3000, false);
                return;
            }

            QString CheminOKTransfrDoc  = CheminOKTransfrDir + "/" + NomFileDoc + "." JPG;
            QString CheminOKTransfrProv = CheminOKTransfrDir + "/" + NomFileDoc + "prov." JPG;
            QByteArray ba = listexportjpgfact.at(i).at(6).toByteArray();
            QPixmap pix;
            pix.loadFromData(ba);
            /*
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
                qDebug() << "erreur";
                return;
            }
            if (!CompressFileJPG(CheminOKTransfrProv))
            {
                db->SupprRecordFromTable(listexportjpgfact.at(i).at(0).toInt(), "idFacture", NOM_TABLE_FACTURES);
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
            db->StandardSQL("update " NOM_TABLE_FACTURES " set jpg = null, LienFichier = '/" + user + "/" + Utils::correctquoteSQL(NomFileDoc) + "." JPG "'"
                            " where idFacture = " + listexportjpgfact.at(i).at(0).toString());
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
            dlg_message(listmsg, 10);
        }

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES PDF
    //-----------------------------------------------------------------------------------------------------------------------------------------
    reqpdf = "SELECT idFacture, DateFacture, LienFichier, Intitule, Echeancier, idDepense, pdf FROM " NOM_TABLE_FACTURES
            " where pdf is not null";
    QList<QList<QVariant>> listexportpdffact = db->StandardSelectSQL(reqpdf, ok );
    if (ok)
        for (int i=0; i<listexportpdffact.size(); i++)
        {
            if (listexportpdffact.at(i).at(2).toString() != "")
            {
                QString CheminFichier = NomDirStockageImagerie + NOMDIR_FACTURES + listexportpdffact.at(i).at(2).toString();
                if (QFile(CheminFichier).exists())
                {
                    db->StandardSQL  ("update " NOM_TABLE_FACTURES " set pdf = null where idFacture = " + listexportpdffact.at(i).at(0).toString());
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
            QList<QList<QVariant>> Listeusr;
            if (listexportpdffact.at(i).at(4).toInt()==1)          // c'est un échéancier
                req = "select dep.idUser, UserLogin from " NOM_TABLE_DEPENSES " dep, " NOM_TABLE_UTILISATEURS " usr"
                                                                                                              " where dep.idUser  = usr.idUser"
                                                                                                              " and idFacture = " + listexportpdffact.at(i).at(0).toString();
            else                                                // c'est une facture, l'iduser est dans la table
                req = "select dep.idUser, UserLogin from " NOM_TABLE_DEPENSES " dep, " NOM_TABLE_UTILISATEURS " usr"
                                                                                                              " where dep.idUser  = usr.idUser"
                                                                                                              " and idDep = " + listexportpdffact.at(i).at(5).toString();
            Listeusr = db->StandardSelectSQL(req, ok);
            if (Listeusr.size()==0) // il n'y a aucune depense enregistrée pour cette facture, on la détruit
            {
                db->SupprRecordFromTable(listexportpdffact.at(i).at(0).toInt(), "idFacture", NOM_TABLE_FACTURES);
                continue;
            }
            user = Listeusr.at(0).at(1).toString();
            if (listexportpdffact.at(i).at(4).toInt()!=1)
                NomFileDoc += "-"+listexportpdffact.at(i).at(5).toString();

            CheminOKTransfrDir  = CheminOKTransfrDir + "/" + user;
            if (!Utils::mkpath(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                QStringList listmsg;
                listmsg << msg;
                dlg_message(listmsg, 3000, false);
                return;
            }
            QString CheminOKTransfrDoc      = CheminOKTransfrDir + "/" + NomFileDoc + "." PDF;

            QByteArray bapdf;
            bapdf.append(listexportpdffact.at(i).at(6).toByteArray());

            Poppler::Document* document = Poppler::Document::loadFromData(bapdf);
            if (!document || document->isLocked() || document == Q_NULLPTR)
            {
                QStringList listmsg;
                listmsg << tr("Impossible de charger le document ") + NomFileDoc;
                dlg_message(listmsg, 3000, false);
                QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer.toString("yyyy-MM-dd") + ".txt";
                QFile   echectrsfer(echectrsfername);
                if (echectrsfer.open(QIODevice::Append))
                {
                    QTextStream out(&echectrsfer);
                    out << NomFileDoc << "\n" ;
                    echectrsfer.close();
                    QFile CD(CheminEchecTransfrDir + "/" + NomFileDoc);
                    if (CD.open(QIODevice::Append))
                    {
                        QTextStream out(&CD);
                        out << listexportpdffact.at(i).at(6).toByteArray() ;
                    }
                }
                QString delreq = "delete from  " NOM_TABLE_FACTURES " where idFacture = " + listexportpdffact.at(i).at(0).toString();
                //qDebug() << delreq;
                db->StandardSQL  (delreq);
                delete document;
                continue;
            }
            Poppler::PDFConverter *doctosave = document->pdfConverter();
            doctosave->setOutputFileName(CheminOKTransfrDoc);
            doctosave->convert();

            QFile CC(CheminOKTransfrDoc);
            CC.open(QIODevice::ReadWrite);
            CC.setPermissions(QFileDevice::ReadOther
                              | QFileDevice::ReadGroup
                              | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                              | QFileDevice::ReadUser   | QFileDevice::WriteUser);
            CC.close();
            db->StandardSQL ("update " NOM_TABLE_FACTURES " set pdf = null, LienFichier = '/" + user + "/" + Utils::correctquoteSQL(NomFileDoc)  + "." PDF "'"
                             " where idFacture = " + listexportpdffact.at(i).at(0).toString());
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
            dlg_message(listmsg, 10);
        }
    int totfac = listexportjpgfact.size() + listexportpdffact.size();
    if (totfac > 0)
        Message(tr("export terminé") + "\n" + QString::number(totfac) + (totfac>1? tr(" documents comptables exportés en ") :tr(" document comptable exporté en ")) + duree);
    ConnectTimers();
}

void RufusAdmin::Slot_GestionBanques()
{
    DisconnectTimerInactive();
    Dlg_Banq = new dlg_banque(this);
    Dlg_Banq->exec();
    ConnectTimerInactive();
}

void RufusAdmin::Slot_GestLieux()
{
    DisconnectTimerInactive();
    dlg_GestionLieux *gestLieux = new dlg_GestionLieux(this);
    gestLieux->exec();
    ConnectTimerInactive();
}

void RufusAdmin::Slot_GestionMotifs()
{
    DisconnectTimerInactive();
    Dlg_motifs = new dlg_motifs(this);
    Dlg_motifs->setWindowTitle(tr("Motifs de consultations"));
    Dlg_motifs->exec();
    delete Dlg_motifs;
    ConnectTimerInactive();
}

void RufusAdmin::Slot_GestUser()
{
    DisconnectTimerInactive();
    QString req = "select IdUser from " NOM_TABLE_UTILISATEURS " where userlogin <> '" NOM_ADMINISTRATEURDOCS "'";
    int iduser = db->getFirstRecordFromStandardSelectSQL(req, ok).at(0).toInt();
    Dlg_GestUsr = new dlg_gestionusers(iduser, ui->EmplacementServeurupComboBox->currentData().toInt(), true, this);
    Dlg_GestUsr->setWindowTitle(tr("Gestion des utilisateurs"));
    Dlg_GestUsr->setConfig(dlg_gestionusers::ADMIN);
    if(Dlg_GestUsr->exec()>0)
        UpMessageBox::Watch(this, tr("Donnes utilisateurs modifiées?"),
                                  tr("Si vous avez modifié des données d'utilisateurs actuellement connectés,\n"
                                     "chacun de ces utilisateurs doit relancer le programme\n"
                                     "pour pouvoir prendre en compte les modifications apportées!"));
    ConnectTimerInactive();
}

void RufusAdmin::Slot_ImportDocsExternes()
{
    // si aucun appareil n'a de dossier d'échange, inutile de lancer l'import des documents
    bool verifdocs = false;
    for (int row=0; row<ui->AppareilsConnectesupTableWidget->rowCount(); row++)
    {
        UpLineEdit *line    = dynamic_cast<UpLineEdit*>(ui->AppareilsConnectesupTableWidget->cellWidget(row,4));
        if (line!=Q_NULLPTR)
            if (line->text() != "")
            {
                verifdocs = true;
                break;
            }
    }
    if (verifdocs != tim.isActive())
    {
        if (verifdocs)
            tim.start();
        else
            tim.stop();
    }
}

void RufusAdmin::Slot_MasqueAppli()
{
    setEnabled(false);
    foreach (QDialog* d , findChildren<QDialog*>())
        d->hide();
    hide();
}

void RufusAdmin::Slot_MetAJourLaConnexion()
{
        QString MAJConnexionRequete;
        QList<QList<QVariant>> listusers = db->StandardSelectSQL("select iduser from " NOM_TABLE_USERSCONNECTES
                                                                 " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                                                                 " and idlieu = " + QString::number(idlieuExercice),ok);
        if (listusers.size()>0)
            MAJConnexionRequete = "UPDATE " NOM_TABLE_USERSCONNECTES " SET HeureDerniereConnexion = NOW(), "
                                                                     " idUser = " + QString::number(idAdminDocs) +
                                                                     " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                                                                     " and idlieu = " + QString::number(idlieuExercice);
        else
            MAJConnexionRequete = "insert into " NOM_TABLE_USERSCONNECTES "(HeureDerniereConnexion, idUser, UserSuperviseur, UserComptable, UserParent, NomPosteConnecte, MACAdressePosteConnecte, idlieu)"
                                   " VALUES(NOW()," +
                                    QString::number(idAdminDocs) + ", -1, -1, -1, '" +
                                    QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "', '" +
                                    Utils::getMACAdress() + " - " NOM_ADMINISTRATEURDOCS  "', " +
                                    QString::number(idlieuExercice) + ")";
        //qDebug() << MAJConnexionRequete;
        db->StandardSQL(MAJConnexionRequete);

    // Deconnecter les users débranchés accidentellement
    QList<QList<QVariant>> listoldusers = db->StandardSelectSQL("select idUser, NomPosteConnecte, MACAdressePosteConnecte from  " NOM_TABLE_USERSCONNECTES
                                                             " where time_to_sec(timediff(now(),heurederniereconnexion)) > 60", ok);

    if (listoldusers.size() > 0)
    {
        for (int i=0; i<listoldusers.size();i++)
        {
            //on déverrouille les dossiers verrouillés par cet utilisateur et on les remet en salle d'attente
            QString blabla              = ENCOURSEXAMEN;
            int length                  = blabla.size();
            int a                       = listoldusers.at(i).at(0).toInt();
            QString Poste               = listoldusers.at(i).at(1).toString();
            QString LibereVerrouRequete;
            LibereVerrouRequete = "UPDATE " NOM_TABLE_SALLEDATTENTE " SET Statut = '" ARRIVE "', idUserEnCoursExam = null, PosteExamen = null"
                                  " WhERE idUserEnCoursExam = " + QString::number(a) +
                                  " AND PosteExamen = '" + Poste +
                                  "' AND Left(Statut," + QString::number(length) + ") = '" ENCOURSEXAMEN "'";
            db->StandardSQL(LibereVerrouRequete);
            //qDebug() << LibereVerrouRequete;
            //on déverrouille les actes verrouillés en comptabilité par cet utilisateur
            LibereVerrouRequete = "delete from " NOM_TABLE_VERROUCOMPTAACTES " where PosePar = " + QString::number(a);
            db->StandardSQL(LibereVerrouRequete);
            // on détruit le socket de cet utilisateur
            // KillSocket(QStringList() << listoldusers.at(i).at(0).toString() << listoldusers.at(i).at(2).toString().split(" - ").at(0));
            // on retire cet utilisateur de la table des utilisateurs connectés
            db->StandardSQL("delete from " NOM_TABLE_USERSCONNECTES " where NomPosteConnecte = '" + listoldusers.at(i).at(1).toString() + "'");
            Message(tr("Le poste ") + Poste + tr(" a été retiré de la liste des postes connectés actuellement au serveur"),1000);
        }
    }
}

void RufusAdmin::Slot_ModifMDP()
{
    DisconnectTimerInactive();
    gAskMDP    = new UpDialog(this);
    gAskMDP    ->setModal(true);
    gAskMDP    ->move(QPoint(x()+width()/2,y()+height()/2));
    QVBoxLayout *globallay = dynamic_cast<QVBoxLayout*>(gAskMDP->layout());
    QRegExp  rxMdp           = QRegExp("^[a-zA-Z0-9]{3,15}$");


    UpLineEdit *ConfirmMDP = new UpLineEdit(gAskMDP);
    ConfirmMDP->setEchoMode(QLineEdit::Password);
    ConfirmMDP->setObjectName(gConfirmMDP);
    ConfirmMDP->setValidator(new QRegExpValidator(rxMdp,this));
    ConfirmMDP->setAlignment(Qt::AlignCenter);
    globallay->insertWidget(0,ConfirmMDP);
    UpLabel *labelConfirmMDP = new UpLabel();
    labelConfirmMDP->setText(tr("Confirmez le nouveau mot de passe"));
    globallay->insertWidget(0,labelConfirmMDP);
    UpLineEdit *NouvMDP = new UpLineEdit(gAskMDP);
    NouvMDP->setEchoMode(QLineEdit::Password);
    NouvMDP->setObjectName(gNouvMDP);
    NouvMDP->setValidator(new QRegExpValidator(rxMdp,this));
    NouvMDP->setAlignment(Qt::AlignCenter);
    globallay->insertWidget(0,NouvMDP);
    UpLabel *labelNewMDP = new UpLabel();
    labelNewMDP->setText(tr("Entrez le nouveau mot de passe"));
    globallay->insertWidget(0,labelNewMDP);
    UpLineEdit *AncMDP = new UpLineEdit(gAskMDP);
    AncMDP->setEchoMode(QLineEdit::Password);
    AncMDP->setAlignment(Qt::AlignCenter);
    AncMDP->setValidator(new QRegExpValidator(rxMdp,this));
    AncMDP->setObjectName(gAncMDP);
    globallay->insertWidget(0,AncMDP);
    UpLabel *labelOldMDP = new UpLabel();
    labelOldMDP->setText(tr("Ancien mot de passe"));
    globallay->insertWidget(0,labelOldMDP);
    AncMDP->setFocus();

    gAskMDP->AjouteLayButtons(UpDialog::ButtonOK);
    QList <QWidget*> ListTab;
    ListTab << AncMDP << NouvMDP << ConfirmMDP << gAskMDP->OKButton;
    for (int i = 0; i<ListTab.size()-1 ; i++ )
        gAskMDP->setTabOrder(ListTab.at(i), ListTab.at(i+1));
    gAskMDP    ->setWindowTitle(tr("Mot de passe utilisateur"));
    connect(gAskMDP->OKButton,    SIGNAL(clicked(bool)), this, SLOT(Slot_EnregistreNouvMDPAdmin()));
    globallay->setSizeConstraint(QLayout::SetFixedSize);

    gAskMDP->exec();
    ConnectTimerInactive();
}

void RufusAdmin::Slot_RestaureBase()
{
    QString req = "select NomPosteConnecte from " NOM_TABLE_USERSCONNECTES " where NomPosteConnecte <> '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS + "'";
    QList<QList<QVariant>> listpostes = db->StandardSelectSQL(req, ok);
    if (listpostes.size() > 0)
    {
        UpMessageBox::Information(this, tr("Autres postes connectés!"),
                                     tr("Vous ne pouvez pas effectuer d'opération de sauvegarde/restauration sur la base de données"
                                     " si vous n'êtes pas le seul poste connecté.\n"
                                     "Le poste ") + listpostes.at(0).at(0).toString() + tr(" est aussi connecté"));
        return;
    }

    DisconnectTimers();
    UpMessageBox::Information(this, tr("Choix du dossier de sauvegarde"),
                              tr("Dans la fiche suivante, choisissez le dossier "
                              "contenant la sauvegarde de la base.\n\n"
                              "Une fois le dossier sélectionné, "
                              "la sauvegarde commencera automatiquement.\n"
                              "Ce processus est long et peut durer plusieurs minutes.\n"
                              "(environ 1' pour 2 Go)\n"));
    QString dir = QDir::homePath() + NOMDIR_RUFUS;
    QFileDialog dialog(Q_NULLPTR,tr("Restaurer à partir du dossier") , dir);
    dialog.setViewMode(QFileDialog::List);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    bool b = (dialog.exec()>0);
    if (!b)
    {
        ConnectTimers();
        return;
    }
    QDir dirtorestore = dialog.directory();
    if (dirtorestore.dirName()=="")
    {
        ConnectTimers();
        return;
    }
    if (dirtorestore.absolutePath().contains(" "))
    {
        UpMessageBox::Watch(Q_NULLPTR, tr("Echec de la restauration"), tr("Le chemin vers le dossier ") + dirtorestore.absolutePath() + tr(" contient des espaces!"));
        ConnectTimers();
        return;
    }

    /* ---------------------------------------------------------------------------------------------------------------------------------------------------------
    * Restauration ---------------------------------------------------------------------------------------------------------------------------------------------
        * de la base de données --------------------------------------------------------------------------------------------------------------------------
        * des fichiers de ressources ---------------------------------------------------------------------------------------------------------------------------
        * de Rufus.ini -----------------------------------------------------------------------------------------------------------------------------------------
        * du dossier d'imagerie --------------------------------------------------------------------------------------------------------------------------------
        * des videos -------------------------------------------------------------------------------------------------------------------------------------------
    * -------------------------------------------------------------------------------------------------------------------------------------------------------*/
    bool OKRessces  = false;
    bool OKini      = false;
    bool OKImages   = false;
    bool OKVideos   = false;

    QString msg;

    if (QDir(dirtorestore.absolutePath() + NOMDIR_RESSOURCES).exists())
        if (QDir(dirtorestore.absolutePath() + NOMDIR_RESSOURCES).entryList(QDir::Files | QDir::NoDotAndDotDot).size()>0)
            OKRessces = true;
    if (QFile(dirtorestore.absolutePath() + "/Rufus.ini").exists())
        OKini = true;
    if (QDir(dirtorestore.absolutePath() + NOMDIR_IMAGES).exists())
        if (QDir(dirtorestore.absolutePath() + NOMDIR_IMAGES).entryList(QDir::Dirs).size()>0)
            OKImages = true;
    if (QDir(dirtorestore.absolutePath() + NOMDIR_VIDEOS).exists())
        if (QDir(dirtorestore.absolutePath() + NOMDIR_VIDEOS).entryList(QDir::Files | QDir::NoDotAndDotDot).size()>0)
            OKVideos = true;

    QString NomDirStockageImagerie("");
    QList<QVariant> dirstock = db->getFirstRecordFromStandardSelectSQL("select dirimagerie from " NOM_TABLE_PARAMSYSTEME, ok);
    if (ok && dirstock.size()>0)
        NomDirStockageImagerie = dirstock.at(0).toString();
    if (!ok || !QDir(NomDirStockageImagerie).exists())
    {
        UpMessageBox::Watch(Q_NULLPTR,tr("Pas de dossier de stockage valide"),
                            tr("Le dossier spécifié pour le stockage de l'imagerie n'est pas valide") + "\n"
                            + tr("Indiquez un dossier valide dans la boîte de dialogue qui suit"));
        QFileDialog dialogimg(Q_NULLPTR,tr("Stocker les images dans le dossier") , QDir::homePath() + NOMDIR_RUFUS);
        dialogimg.setViewMode(QFileDialog::List);
        dialogimg.setFileMode(QFileDialog::DirectoryOnly);
        bool b = (dialogimg.exec()>0);
        if (!b)
        {
            ConnectTimers();
            return;
        }
        QDir dirstock = dialogimg.directory();
        NomDirStockageImagerie = dirstock.dirName();
        if (NomDirStockageImagerie=="")
        {
            ConnectTimers();
            return;
        }
        if (NomDirStockageImagerie.contains(" "))
        {
            UpMessageBox::Watch(Q_NULLPTR, tr("Echec de la restauration"), tr("Le chemin vers le dossier ") + NomDirStockageImagerie + tr(" contient des espaces!"));
            ConnectTimers();
            return;
        }
        NomDirStockageImagerie = dirstock.absolutePath();
        gsettingsIni->setValue("BDD_POSTE/DossierImagerie", NomDirStockageImagerie);
        QString reqimg = "update " NOM_TABLE_PARAMSYSTEME " set DirImagerie = '" + NomDirStockageImagerie + "'";
        db->StandardSQL(reqimg);
    }

    AskBupRestore(true, dirtorestore.absolutePath(), NomDirStockageImagerie, OKini, OKRessces, OKImages, OKVideos);
    if (gAskBupRestore->exec()>0)
    {
        QList<UpCheckBox*> listchk = gAskBupRestore->findChildren<UpCheckBox*>();
        for (int i= 0; i<listchk.size(); i++)
        {
            if (listchk.at(i)->accessibleDescription() == "base")
            {
                if (listchk.at(i)->isChecked())
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
                    if (!VerifMDP(db->getDataBase().password(),tr("Saisissez le mot de passe Administrateur")))
                    {
                        msg += tr("Base non restaurée");
                        break;
                    }
                    //        if (!SauvegardeBase())
                    //            return;
                    bool echecfile = true;
                    QString NomDumpFile;
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
                        //Suppression de toutes les tables
                        QString Msg = tr("Suppression de l'ancienne base Rufus en cours");
                        Message(Msg, 3000, false);
                        db->StandardSQL("drop database if exists " NOM_BASE_COMPTA);
                        db->StandardSQL("drop database if exists " NOM_BASE_OPHTA);
                        db->StandardSQL("drop database if exists " NOM_BASE_CONSULTS);
                        db->StandardSQL("drop database if exists " NOM_BASE_IMAGES);
                        int a = 99;
                        //Restauration à partir du dossier sélectionné
                        for (int j=0; j<listnomsfilestorestore.size(); j++)
                        {
                            Msg = (tr("Restauration de la base Rufus à partir de ") + listnomsfilestorestore.at(j) + "\n"
                                           + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
                            Message(Msg, 3000);
                            NomDumpFile = listnomsfilestorestore.at(j);
                            QProcess dumpProcess(parent());
                            QStringList args;
                            args << "-u " + db->getDataBase().userName();
                            args << "-p"  + db->getDataBase().password();
                            args << "-h " + db->getDataBase().hostName();
                            args << "-P " + QString::number(db->getDataBase().port());
                            args << "<";
                            args << listnomsfilestorestore.at(j) + "\"";
                            QDir Dir(QCoreApplication::applicationDirPath());
                            Dir.cdUp();
                            QString DirApp = Dir.absolutePath();
                            QString program   = "bash -c \"" + DirApp + NOMDIR_LIBS2;
                            program += "/mysql";
                            QString argstring = program;
                            for (int i=0; i<args.size();i++)    argstring += " " + args.at(i);
                            //Edit(argstring);
                            dumpProcess.start(argstring);
                            dumpProcess.waitForFinished();
                            if (dumpProcess.exitStatus() == QProcess::NormalExit)
                                a = dumpProcess.exitCode();
                            QString nombase = listfichiers.at(j);
                            nombase.remove(".sql");
                            if (a>0)
                            {
                                msg += tr("Erreur de restauration de la base") + " " + nombase;
                                break;
                            }
                            else msg += tr("Restauration de la base") + " " + nombase + " OK\n";
                        }
                    }
                }
            }
        }
        for (int i= 0; i<listchk.size(); i++)
        {
            if (listchk.at(i)->accessibleDescription() == "ini")
            {
                if (listchk.at(i)->isChecked())
                {
                    QString fileini = dirtorestore.absolutePath() + "/Rufus.ini";
                    QFile FichierIni(gNomFichRufusIni);
                    if (FichierIni.exists())
                        FichierIni.remove();
                    QFile rufusini(fileini);
                    rufusini.copy(gNomFichRufusIni);
                    msg += tr("Fichier de paramétrage Rufus.ini restauré\n");
                    Message(tr("Fichier de paramétrage Rufus.ini restauré"), 3000, false);
                }
            }
        }
        for (int i= 0; i<listchk.size(); i++)
        {
            if (listchk.at(i)->accessibleDescription() == "ressources")
            {
                if (listchk.at(i)->isChecked())
                {
                    QDir DirRssces(QDir(dirtorestore.absolutePath() + NOMDIR_RESSOURCES));
                    Utils::mkpath(QDir::homePath() + NOMDIR_RUFUS NOMDIR_RESSOURCES);
                    QStringList listnomfic = DirRssces.entryList();
                    for (int i=0; i<listnomfic.size(); i++)
                    {
                        QFile ficACopier(DirRssces.absolutePath() + "/" + listnomfic.at(i));
                        QString nomficACopier = QFileInfo(listnomfic.at(i)).fileName();
                        ficACopier.copy(QDir::homePath() + NOMDIR_RUFUS NOMDIR_RESSOURCES + "/" + nomficACopier);
                    }
                    msg += tr("Fichiers de ressources d'impression restaurés\n");
                    Message(tr("Fichiers de ressources d'impression restaurés"), 3000, false);
                }
            }
        }
        for (int i= 0; i<listchk.size(); i++)
        {
            if (listchk.at(i)->accessibleDescription() == "images")
            {
                if (listchk.at(i)->isChecked())
                {
                    QDir dirrestaureimagerie = QDir(dirtorestore.absolutePath() + NOMDIR_IMAGES);
                    QString dirdestinationimg       =  NomDirStockageImagerie + NOMDIR_IMAGES;
                    QDir DirDestImg(dirdestinationimg);
                    if (DirDestImg.exists())
                        DirDestImg.removeRecursively();
                    if (!DirDestImg.mkdir(dirdestinationimg))
                    {
                        QString Msg = tr("le dossier de destination de l'imagerie n'existe pas");
                        Message(Msg, 3000, false);
                    }
                    else
                    {
                        QString Msg = (tr("Restauration des fichiers d'imagerie\n")
                                       + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
                        Message(Msg, 3000);
                        Msg = "cp -R " + dirrestaureimagerie.absolutePath() + " " + NomDirStockageImagerie;
                        //qDebug() << Msg;
                        QProcess::execute(Msg);
                        msg += tr("Fichiers d'imagerie restaurés\n");
                        Message(tr("Fichiers d'imagerie restaurés"), 3000, false);
                    }
                }
            }
        }
        for (int i= 0; i<listchk.size(); i++)
        {
            if (listchk.at(i)->accessibleDescription() == "videos")
            {
                if (listchk.at(i)->isChecked())
                {
                    QDir dirrestaurevideo = QDir(dirtorestore.absolutePath() + NOMDIR_VIDEOS);
                    QString dirdestinationvid       =  NomDirStockageImagerie + NOMDIR_VIDEOS;
                    QDir DirDestVid(dirdestinationvid);
                    if (DirDestVid.exists())
                        DirDestVid.removeRecursively();
                    if (!DirDestVid.mkdir(dirdestinationvid))
                    {
                        QString Msg = tr("le dossier de destination des videos n'existe pas");
                        Message(Msg, 3000, false);
                    }
                    else
                    {
                        QString Msg = (tr("Restauration des fichiers videos\n")
                                       + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
                        Message(Msg, 3000, false);
                        Msg = "cp -R " + dirrestaurevideo.absolutePath() + " " + NomDirStockageImagerie;
                        QProcess::execute(Msg);
                        msg += tr("Fichiers videos restaurés\n");
                        Message(tr("Fichiers videos restaurés"), 3000);
                    }
                }
            }
        }
        delete gAskBupRestore;
        //qDebug() << msg;
        UpMessageBox::Watch(this,tr("restauration terminée"),msg);
    }
    ConnectTimers();
}

// utilisé par RufusAdmin pour supprimer les documents et les factures
// ayant été inscrits dans les tables DocsASupprimer et FacturesASupprimer
// par les autres postes

void RufusAdmin::SupprimerDocsEtFactures()
{
    bool ok = true;
    QString NomDirStockageImagerie = gsettingsIni->value("DossierImagerie").toString();

    /* Supprimer les documents en attente de suppression*/
    QString req = "Select filepath from " NOM_TABLE_DOCSASUPPRIMER;
    QList<QList<QVariant>> ListeDocs = db->StandardSelectSQL(req, ok);
    for (int i=0; i<ListeDocs.size(); i++)
    {
        QString CheminFichier = NomDirStockageImagerie + ListeDocs.at(i).at(0).toString();
        if (!QFile(CheminFichier).remove())
            UpMessageBox::Watch(this, tr("Fichier introuvable!"), CheminFichier);
        db->StandardSQL("delete from " NOM_TABLE_DOCSASUPPRIMER " where filepath = '" + Utils::correctquoteSQL(ListeDocs.at(i).at(0).toString()) + "'");
    }

    /* Supprimer les factures en attente de suppression - même démarche mais on fait une copie de la facture dans le dossier FACTURESSANSLIEN avant de la supprimer*/
    QString CheminOKTransfrDir = NomDirStockageImagerie + NOMDIR_FACTURESSANSLIEN;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        dlg_message(listmsg, 3000, false);
        return;
    }
    req = "select LienFichier from " NOM_TABLE_FACTURESASUPPRIMER;
    QList<QList<QVariant>> ListeFactures = db->StandardSelectSQL(req, ok);
    for (int i=0; i<ListeFactures.size(); i++)
    {
        QString lienfichier = ListeFactures.at(i).at(0).toString();
        /*  on copie le fichier dans le dossier facturessanslien*/
        QString user = lienfichier.split("/").at(1);
        CheminOKTransfrDir = CheminOKTransfrDir + "/" + user;
        if (!Utils::mkpath(CheminOKTransfrDir))
        {
            QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
            QStringList listmsg;
            listmsg << msg;
            dlg_message(listmsg, 3000, false);
            continue;
        }
        QFile(NomDirStockageImagerie + NOMDIR_FACTURES + lienfichier).copy(NomDirStockageImagerie + NOMDIR_FACTURESSANSLIEN + lienfichier);
        /*  on l'efface du dossier de factures*/
        QFile(NomDirStockageImagerie + NOMDIR_FACTURES + lienfichier).remove();
        /* on détruit l'enregistrement dans la table FacturesASupprimer*/
        db->StandardSQL("delete from " NOM_TABLE_FACTURESASUPPRIMER " where LienFichier = '" + Utils::correctquoteSQL(lienfichier) + "'");
    }
}

void RufusAdmin::Slot_TrayIconMenu()
{
    trayIconMenu->clear();
    if (! isVisible())
    {
        QString txt = tr("Ouvir RufusAdmin");
        QAction *pAction_VoirAppli = trayIconMenu->addAction(txt);
        connect (pAction_VoirAppli, &QAction::triggered, [=] {ChoixMenuSystemTray(txt);});
    }
    QString txt = tr("Quitter RufusAdmin");
    QAction *pAction_QuitAppli = trayIconMenu->addAction(txt);
    connect (pAction_QuitAppli, &QAction::triggered, [=] {ChoixMenuSystemTray(txt);});
}

void RufusAdmin::ChoixMenuSystemTray(QString txt)
{
    // il faut montrer la fiche d'abord sinon la fermeture du QInputDialog de VerifMDP()
    // provoque la fermeture du programme quoiqu'il arrive (???)
    bool visible = isVisible();
    if (!visible)
        showNormal();
    if (!VerifMDP(db->getDataBase().password(),tr("Saisissez le mot de passe Administrateur")))
    {
        if (!visible)
            Slot_MasqueAppli();
        return;
    }

    if (txt == tr("Quitter RufusAdmin"))
    {
        // on retire le poste de la variable posteimportdocs SQL
        setPosteImportDocs(false);
        // on retire Admin de la table des utilisateurs connectés
        QString req = "delete from " NOM_TABLE_USERSCONNECTES
                      " where MACAdressePosteConnecte = '" + Utils::getMACAdress() + " - " NOM_ADMINISTRATEURDOCS  "'"
                      " and idlieu = " + QString::number(idlieuExercice);
        db->StandardSQL(req);
        FermeTCP();
        setPosteImportDocs(false);
        exit(0);
    }
    setEnabled(true);
}

void RufusAdmin::Slot_VerifPosteImport()
{
    if (gMode == Distant)
        return;
    //On recherche si le poste défini comme importateur des docs externes n'est pas celui sur lequel s'éxécute cette session de RufusAdmin et on prend sa place dans ce cas
    QString A, PostImport;    // l'importateur des docs externes
    QString req = "SELECT name FROM mysql.proc p WHERE db = '" NOM_BASE_CONSULTS "' AND name = '" NOM_POSTEIMPORTDOCS "'";
    QList<QVariant> imptdata = db->getFirstRecordFromStandardSelectSQL(req, ok);
    if (ok && imptdata.size()>0)
    {
        QList<QVariant> procdata = db->getFirstRecordFromStandardSelectSQL("CALL " NOM_BASE_CONSULTS "." NOM_POSTEIMPORTDOCS, ok);
        if(!ok || procdata.size()==0)
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
            if (A.contains(" - " NOM_ADMINISTRATEURDOCS))
                B = tr("Administrateur");
            else
                B = (A.contains(" - prioritaire")? tr("prioritaire") : tr("non prioritaire"));
            A.remove(" - prioritaire");
            A.remove(" - " NOM_ADMINISTRATEURDOCS);
            ui->PosteImportDocslabel->setText(A);

            if (B == tr("non prioritaire"))
                B = "<font color=\"red\"><b>" + B + "</b></font>";
            else
                B = "<font color=\"green\"><b>" + B + "</b></font>";
            ui->PosteImportDocsPrioritairelabel->setText(B);
        }
    }
    QString IpAdr = QHostInfo::localHostName() + " - " NOM_ADMINISTRATEURDOCS;
    if (PostImport != "NULL" && PostImport != IpAdr)
        setPosteImportDocs();
}

void RufusAdmin::Slot_VerifVersionBase()
{
    int version = VERSION_BASE;
    QList<QVariant> versiondata = db->getFirstRecordFromStandardSelectSQL("select versionbase from " NOM_TABLE_PARAMSYSTEME, ok);
    if (!ok)
        return;
    if (versiondata.size()>0)
        version = versiondata.at(0).toInt();
    if (version != VERSION_BASE)
    {
        UpMessageBox::Watch(this, tr("Versons incompatibles"),
                tr("Vous utilisez une version de RufusAdmin prévue\npour être utilisée avec la version de la base Rufus n°") + QString::number(VERSION_BASE)+ "\n" +
                tr("La version de la base Rufus en cours d'utilisation est la n° ") + QString::number(version) + "\n" +
                tr("Faites une mise à jour de Rufus et Rufus Admin pour qu'ils utilisent les dernières versions de la base") + "\n\n-> " +
                tr("Sortie du programme"));
        exit(0);
    }
}

void RufusAdmin::ReconstruitListeLieuxExercice()
{
    /*-------------------- GESTION DES LIEUX D'EXERCICE-------------------------------------------------------*/
    ui->EmplacementServeurupComboBox->clear();
    QList<QList<QVariant>> listlieux = db->StandardSelectSQL("select idLieu, NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, LieuCodePostal, LieuVille, LieuTelephone from " NOM_TABLE_LIEUXEXERCICE, ok);
    for (int i=0; i< listlieux.size(); i++)
         ui->EmplacementServeurupComboBox->addItem(listlieux.at(i).at(1).toString(), listlieux.at(i).at(0));
    int DefautLieu = 0;
    QList<QVariant> dftLieu = db->getFirstRecordFromStandardSelectSQL("select idlieupardefaut from " NOM_TABLE_PARAMSYSTEME, ok);
    if(!ok)
        return;
    if (dftLieu.size()>0)
        DefautLieu = dftLieu.at(0).toInt();
    if (dftLieu.size()>0 && DefautLieu>0)
        ui->EmplacementServeurupComboBox->setCurrentIndex(ui->EmplacementServeurupComboBox->findData(DefautLieu));
    else
        ui->EmplacementServeurupComboBox->setCurrentIndex(0);
    /*-------------------- GESTION DES LIEUX D'EXERCICE-------------------------------------------------------*/
}

bool RufusAdmin::VerifBase()
{
    int Versionencours  = 37; //correspond aux premières versions de MAJ de la base
    int Version         = VERSION_BASE;
    QList<QVariant> Versionenr = db->getFirstRecordFromStandardSelectSQL("select VersionBase from " NOM_TABLE_PARAMSYSTEME,  ok);
    bool b              = false;
    if (!ok || Versionenr.size()==0)
        b = true;
    else
    {
        Versionencours = Versionenr.at(0).toInt();
        if (Versionencours < Version)
            b = true;
    }
    bool BupDone = false;
    if (b)
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
                UpSmallButton *AnnulBouton = new UpSmallButton();
                OKBouton->setText(tr("OK, je vais sauvegarder la base d'abord"));
                AnnulBouton->setText(tr("Pousuivre, la sauvegarde a été faite"));
                msgbox.addButton(OKBouton, UpSmallButton::CANCELBUTTON);
                msgbox.addButton(AnnulBouton, UpSmallButton::STARTBUTTON);
                msgbox.exec();
                if (msgbox.clickedButton() != AnnulBouton)
                    if (!ImmediateBackup())
                        return false;
                BupDone = true;
            }
            Message(tr("Mise à jour de la base vers la version ") + "<font color=\"red\"><b>" + QString::number(Version) + "</b></font>", 1000, false);
            QString Nomfic = "://majbase" + QString::number(Version) + ".sql";
            QFile DumpFile(Nomfic);
            QString NomDumpFile = QDir::homePath() + "/Documents/Rufus/Ressources/majbase" + QString::number(Version) + ".sql";
            DumpFile.copy(NomDumpFile);
            QFile base(NomDumpFile);
            QStringList listinstruct = DecomposeScriptSQL(NomDumpFile);
            bool a = true;
            foreach(const QString &s, listinstruct)
            {
                //Edit(s);
                if (!db->StandardSQL(s))
                        a = false;
            }
            int result=0;
            base.remove();
            if (a)
            {
                result = 1;
                UpMessageBox::Watch(this,tr("Mise à jour effectuée de la base vers la version ") + QString::number(Version));
            }
            else
            {
                QSound::play(NOM_ALARME);
                UpMessageBox::Watch(this,tr("Echec de la mise à jour vers la version ") + QString::number(Version) + "\n" + tr("Le programme de mise à jour n'a pas pu effectuer la tâche!"));
            }
            if (result!=1)
                return false;
        }
    }
    return true;
}

/*---------------------------------------------------------------------------------------------------------------------
    -- VÉRIFICATION DE MDP --------------------------------------------------------------------------------------------
    -----------------------------------------------------------------------------------------------------------------*/
bool RufusAdmin::VerifMDP(QString MDP, QString Msg)
{
    DisconnectTimerInactive();
    QInputDialog quest;
    quest.setCancelButtonText("Annuler");
    quest.setLabelText(Msg);
    quest.setInputMode(QInputDialog::TextInput);
    quest.setTextEchoMode(QLineEdit::Password);
    QList<QLineEdit*> list = quest.findChildren<QLineEdit*>();
    for (int i=0;i<list.size();i++)
        list.at(0)->setAlignment(Qt::AlignCenter);
    QList<QLabel*> listlab = quest.findChildren<QLabel*>();
    for (int i=0;i<listlab.size();i++)
        listlab.at(0)->setAlignment(Qt::AlignCenter);
    quest.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    int a = quest.exec();
    ConnectTimerInactive();
    if (a > 0)
    {
        if (quest.textValue() == MDP)
            return true;
        else
            UpMessageBox::Watch(this,tr("Mot de passe invalide!"));
    }
    return false;
}

void RufusAdmin::Slot_ModifDirBackup()
{
    if (gMode != Poste)
        return;
    QString dirsauvorigin   = ui->DirBackupuplineEdit->text();
    DisconnectTimerInactive();
    QString dirSauv         = QFileDialog::getExistingDirectory(this,tr("Choisissez le dossier dans lequel vous voulez sauvegarder la base\n"
                                                                "Le nom de dossier ne doit pas contenir d'espace"), QDir::homePath());
    if (dirSauv == "")
    {
        ConnectTimerInactive();
        return;
    }
    if (dirSauv.contains(" "))
    {
        UpMessageBox::Watch(this, tr("Nom de dossier non conforme"),tr("Vous ne pouvez pas choisir un dossier dont le nom contient des espaces"));
        ConnectTimerInactive();
        return;
    }
    ui->DirBackupuplineEdit ->setText(dirSauv);
    if (dirsauvorigin != dirSauv)
    {
        db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set DirBkup = '" + dirSauv + "'");
        ModifParamBackup();
    }
    ConnectTimerInactive();
}

void RufusAdmin::Slot_ModifDateBackup()
{
    ModifParamBackup();
}

void RufusAdmin::Message(QString mess, int pause, bool bottom)
{
    QTime dieTime= QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    ui->MessageupLabel->setText(mess);
    EffaceMessage();
    QStringList listmsg;
    listmsg << mess;
    dlg_message(listmsg, pause, bottom);
}

void RufusAdmin::EffaceMessage(int pause)
{
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(pause);
    connect (&timer, &QTimer::timeout, [=] {ui->MessageupLabel->setText("");});
}

void RufusAdmin::ModifParamBackup()
{
    if (gMode != Poste)
        return;
    bool NoDirBupDefined        = (ui->DirBackupuplineEdit->text() == "");
    bool IncorrectDirBupDefined = !QDir(ui->DirBackupuplineEdit->text()).exists() && !NoDirBupDefined;
    bool NoDayBupDefined        = true;
    QList<QRadioButton*> listbut= ui->JourSauvegardegroupBox->findChildren<QRadioButton*>();
    for (int i=0; i<listbut.size(); i++)
        if (listbut.at(i)->isChecked())
        {
            NoDayBupDefined = false;
            break;
        }
    if (NoDirBupDefined || NoDayBupDefined || IncorrectDirBupDefined)
    {
#ifdef Q_OS_MACX
    QString unload  = "bash -c \"/bin/launchctl unload \"" + QDir::homePath();
    unload += SCRIPTPLISTFILE "\"\"";
    QProcess dumpProcess(parent());
    dumpProcess.start(unload);
    dumpProcess.waitForFinished();
    if (QFile::exists(QDir::homePath() + SCRIPTBACKUPFILE))
        QFile::remove(QDir::homePath() + SCRIPTBACKUPFILE);
    if (QFile::exists(QDir::homePath() + SCRIPTPLISTFILE))
        QFile::remove(QDir::homePath() + SCRIPTPLISTFILE);
#endif
        return;
    }

    QString dirSauv   = ui->DirBackupuplineEdit->text();
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set DirBkup = '" + dirSauv + "'");
    // ENREGISTREMENT DES PARAMETRES DE SAUVEGARDE DANS /Documents/Rufus/RufusScriptBackup.sh
    QString NomDirStockageImagerie("");
    QList<QVariant> dirdata = db->getFirstRecordFromStandardSelectSQL("select dirimagerie from " NOM_TABLE_PARAMSYSTEME, ok);
    if (ok && dirdata.size()>0)
        NomDirStockageImagerie = dirdata.at(0).toString();

    //1. Dossier de backup
    DefinitScriptBackup(NomDirStockageImagerie);

    //2. Heure et date du backup
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set HeureBkup = '"    + ui->HeureBackuptimeEdit->time().toString("HH:mm") + "'");

    QString LundiBkup       = (ui->LundiradioButton->isChecked()?   "1" : "NULL");
    QString MardiBkup       = (ui->MardiradioButton->isChecked()?   "1" : "NULL");
    QString MercrediBkup    = (ui->MercrediradioButton->isChecked()?"1" : "NULL");
    QString JeudiBkup       = (ui->JeudiradioButton->isChecked()?   "1" : "NULL");
    QString VendrediBkup    = (ui->VendrediradioButton->isChecked()?"1" : "NULL");
    QString SamediBkup      = (ui->SamediradioButton->isChecked()?  "1" : "NULL");
    QString DimancheBkup    = (ui->DimancheradioButton->isChecked()?"1" : "NULL");

    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set LundiBkup = "     + LundiBkup);
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set MardiBkup = "     + MardiBkup);
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set MercrediBkup = "  + MercrediBkup);
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set JeudiBkup = "     + JeudiBkup);
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set VendrediBkup = "  + VendrediBkup);
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set SamediBkup = "    + SamediBkup);
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set DimancheBkup = "  + DimancheBkup);

#ifdef Q_OS_MACX
    // elaboration de rufus.bup.plist
    QString heure   = ui->HeureBackuptimeEdit->time().toString("H");
    QString minute  = ui->HeureBackuptimeEdit->time().toString("m");
    QString jourprg;
    int njours = 0;
    QList<QRadioButton*> listbutton2 = ui->JourSauvegardegroupBox->findChildren<QRadioButton*>();
    for (int i=0; i<listbutton2.size(); i++)
       if (listbutton2.at(i)->isChecked()) njours ++;
    QString a = (njours>1? "\t": "");
    if (njours>1)
        jourprg += "\t\t<array>\n";

    QString debutjour =
        a + "\t\t<dict>\n" +
        a + "\t\t\t<key>Weekday</key>\n" +
        a + "\t\t\t<integer>";
    QString finjour =
        "</integer>\n" +
        a + "\t\t\t<key>Hour</key>\n" +
        a + "\t\t\t<integer>"+ heure + "</integer>\n" +
        a + "\t\t\t<key>Minute</key>\n" +
        a + "\t\t\t<integer>" + minute + "</integer>\n" +
        a + "\t\t</dict>\n";
    if (ui->LundiradioButton->isChecked())
        jourprg += debutjour + "1" + finjour;
    if (ui->MardiradioButton->isChecked())
        jourprg += debutjour + "2" + finjour;
    if (ui->MercrediradioButton->isChecked())
        jourprg += debutjour + "3" + finjour;
    if (ui->JeudiradioButton->isChecked())
        jourprg += debutjour + "4" + finjour;
    if (ui->VendrediradioButton->isChecked())
        jourprg += debutjour + "5" + finjour;
    if (ui->SamediradioButton->isChecked())
        jourprg += debutjour + "6" + finjour;
    if (ui->DimancheradioButton->isChecked())
        jourprg += debutjour + "7" + finjour;
    if (njours>1)
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
                                "\t\t\t<string>" + QDir::homePath() + SCRIPTBACKUPFILE + "</string>\n"
                            "\t\t</array>\n"
                            "\t\t<key>StartCalendarInterval</key>\n"
                            + jourprg +
                        "\t</dict>\n"
                    "</plist>\n";
    if (QFile::exists(QDir::homePath() + SCRIPTPLISTFILE))
        QFile::remove(QDir::homePath() + SCRIPTPLISTFILE);
    QFile fplist(QDir::homePath() + SCRIPTPLISTFILE);
    if (fplist.open(QIODevice::ReadWrite))
    {
        QTextStream out(&fplist);
        out << plist;
        fplist.close();
    }

    // relance du launchd
    QString unload  = "bash -c \"/bin/launchctl unload \"" + QDir::homePath();
    unload += SCRIPTPLISTFILE "\"\"";
    QString load    = "bash -c \"/bin/launchctl load \""   + QDir::homePath();
    load += SCRIPTPLISTFILE "\"\"";
    QProcess dumpProcess(parent());
    dumpProcess.start(unload);
    dumpProcess.waitForFinished();
    dumpProcess.start(load);
    dumpProcess.waitForFinished();
#endif

    //programmation de l'effacement du contenu de la table ImagesEchange
    db->StandardSQL("Use " NOM_BASE_IMAGES);
    db->StandardSQL("DROP EVENT IF EXISTS VideImagesEchange");
    QString req =   "CREATE EVENT VideImagesEchange "
            "ON SCHEDULE EVERY 1 DAY STARTS '2018-03-23 " + ui->HeureBackuptimeEdit->time().addSecs(-60).toString("HH:mm:ss") + "' "
            "DO DELETE FROM " NOM_TABLE_ECHANGEIMAGES;
    db->StandardSQL(req);
    //programmation de l'effacement des pdf et jpg contenus dans Factures
    db->StandardSQL("Use " NOM_BASE_COMPTA);
    db->StandardSQL("DROP EVENT IF EXISTS VideFactures");
    req =   "CREATE EVENT VideFactures "
            "ON SCHEDULE EVERY 1 DAY STARTS '2018-03-23 " + ui->HeureBackuptimeEdit->time().addSecs(-60).toString("HH:mm:ss") + "' "
            "DO UPDATE " NOM_TABLE_FACTURES " SET jpg = null, pdf = null";
    db->StandardSQL(req);
}

QStringList RufusAdmin::DecomposeScriptSQL(QString nomficscript)
{
    QStringList listinstruct;
    QFile file(nomficscript);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        listinstruct << "";
        return QStringList();
    }
    QString queryStr(file.readAll());
    file.close();
    // On retire tous les commentaires, les tabulations, les espaces ou les retours à la ligne multiples
    //        queryStr = queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
    queryStr = queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/)",   QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
    queryStr = queryStr.replace(QRegularExpression("(^;\\n)",                   QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
    queryStr = queryStr.replace(QRegularExpression("(--.*\\n)",                 QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "\n");
    queryStr = queryStr.replace(QRegularExpression("( +)",                      QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), " ");
    queryStr = queryStr.replace(QRegularExpression("((\\t)+)",                  QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), " ");
    queryStr = queryStr.replace(QRegularExpression("(^ *)",                     QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
    queryStr = queryStr.replace(QRegularExpression("((\\n)+)",                  QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "\n");
    //Retire les espaces en début et fin de string
    queryStr = queryStr.trimmed();

    QString matched, delimiter, Atraiter;
    QRegularExpression re("^(\\s|\\n)*DELIMITER\\s*(.|\\n)*END\\s*.\\n"); //isole les créations de procédure SQL dans le script

    while (queryStr.size()>0 && queryStr.contains(";"))
    {
        //Edit(queryStr);
        QRegularExpressionMatch match = re.match(queryStr);
        if (match.hasMatch())  // --> c'est une procédure à créer
        {
            matched     = match.capturedTexts().at(0);
            Atraiter    = matched.trimmed();
            //Edit(Atraiter);
            delimiter   = Atraiter.data()[Atraiter.size()-1];
            //Edit(delimiter);
            Atraiter.replace(QRegularExpression("DELIMITER\\s*"),"");
            Atraiter.replace(delimiter,"");
            Atraiter = Atraiter.replace(QRegularExpression("(^ *)",     QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
            Atraiter = Atraiter.replace(QRegularExpression("(^(\\n)+)", QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
            Atraiter = Atraiter.replace(QRegularExpression("((\\n)+)",  QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "\n");
            //Edit(Atraiter);
            queryStr.replace(0,matched.size(),"");
        }
        else                    // -- c'est une requête SQL
        {
            matched = queryStr.split(";\n", QString::SkipEmptyParts).at(0);
            Atraiter = matched.trimmed()+ ";";
            queryStr.replace(0,matched.size()+2,"");
            queryStr = queryStr.replace(QRegularExpression("((\\n)+)",  QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "\n");
        }
        queryStr = queryStr.replace(QRegularExpression("(^(\\n)*)",     QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), "");
        listinstruct << Atraiter;
    }
    return listinstruct;
}

void RufusAdmin::DefinitScriptBackup(QString path, bool AvecImages, bool AvecVideos)
{
    // élaboration du script de backup
    QString scriptbackup = "#!/bin/bash";
    //# Configuration de base: datestamp e.g. YYYYMMDD
    scriptbackup += "\n";
    scriptbackup += "DATE=$(date +\"%Y%m%d-%H%M\")";
    //# Dossier où sauvegarder les backups (créez le d'abord!)
    scriptbackup += "\n";
    scriptbackup += "BACKUP_DIR=\"" + ui->DirBackupuplineEdit->text() + "\"";
    //# Dossier de  ressources
    scriptbackup += "\n";
    scriptbackup += "DIR_RESSOURCES=\"" + QDir::homePath() + NOMDIR_RUFUS NOMDIR_RESSOURCES + "\"";
    scriptbackup += "\n";
    scriptbackup += "DIR_IMAGES=\"" + path + NOMDIR_IMAGES + "\"";
    scriptbackup += "\n";
    scriptbackup += "DIR_FACTURES=\"" + path + NOMDIR_FACTURES + "\"";
    scriptbackup += "\n";
    scriptbackup += "DIR_VIDEOS=\"" + path + NOMDIR_VIDEOS + "\"";
    //# Rufus.ini
    scriptbackup += "\n";
    scriptbackup += "RUFUSINI=\"" + QDir::homePath() + NOMFIC_INIRUFUS + "\"";
    //# Identifiants MySQL
    scriptbackup += "\n";
    scriptbackup += "MYSQL_USER=\"dumprufus\"";
    scriptbackup += "\n";
    scriptbackup += "MYSQL_PASSWORD=\"" + db->getDataBase().password() + "\"";
    //# Commandes MySQL
    QDir Dir(QCoreApplication::applicationDirPath());
    Dir.cdUp();
    scriptbackup += "\n";
#ifdef Q_OS_MACX
    scriptbackup += "MYSQL=/usr/local/mysql/bin";           // Depuis HighSierra on ne peut plus utiliser + Dir.absolutePath() + NOMDIR_LIBS2 - le script ne veut pas utiliser le client mysql du package (???)
    scriptbackup += "/mysql";
    scriptbackup += "\n";
    scriptbackup += "MYSQLDUMP=/usr/local/mysql/bin";       // Depuis HighSierra on ne peut plus utiliser + Dir.absolutePath() + NOMDIR_LIBS2 - le script ne veut pas utiliser le client mysql du package (???)
    scriptbackup += "/mysqldump";
    scriptbackup += "\n";
#endif
    //# Bases de données MySQL à ignorer
    scriptbackup += "SKIPDATABASES=\"Database|information_schema|performance_schema|mysql|sys\"";
    //# Nombre de jours à garder les dossiers (seront effacés après X jours)
    scriptbackup += "\n";
    scriptbackup += "RETENTION=14";
    //# Create a new directory into backup directory location for this date
    scriptbackup += "\n";
    scriptbackup += "mkdir -p $BACKUP_DIR/$DATE";
    //# Retrieve a list of all databases
    scriptbackup += "\n";
    scriptbackup += "databases=`$MYSQL -u$MYSQL_USER -p$MYSQL_PASSWORD -e \"SHOW DATABASES;\" | grep -Ev \"($SKIPDATABASES)\"`";
    scriptbackup += "\n";
    scriptbackup += "for db in $databases; do";
    scriptbackup += "\n";
    scriptbackup += "echo $db";
    scriptbackup += "\n";
    scriptbackup += "$MYSQLDUMP --force --opt --user=$MYSQL_USER -p$MYSQL_PASSWORD --skip-lock-tables --events --databases $db > \"$BACKUP_DIR/$DATE/$db.sql\"";
    scriptbackup += "\n";
    scriptbackup += "done";
    // Sauvegarde la table des utilisateurs
    scriptbackup += "\n";
    scriptbackup += "$MYSQLDUMP --force --opt --user=$MYSQL_USER -p$MYSQL_PASSWORD mysql user > \"$BACKUP_DIR/$DATE/user.sql\"";
    // Detruit les anciens fichiers
    scriptbackup += "\n";
    scriptbackup += "find $BACKUP_DIR/* -mtime +$RETENTION -delete";
    // copie les fichiers ressources
    scriptbackup += "\n";
    scriptbackup +=  "cp -R $DIR_RESSOURCES $BACKUP_DIR/$DATE/Ressources";
    // copie les fichiers image
    if (AvecImages)
    {
        scriptbackup += "\n";
        scriptbackup +=  "cp -R $DIR_IMAGES $BACKUP_DIR/$DATE/Images";
    }
    // copie les fichiers facture
    if (AvecImages)
    {
        scriptbackup += "\n";
        scriptbackup +=  "cp -R $DIR_FACTURES $BACKUP_DIR/$DATE/Factures";
    }
    // copie les fichiers video
    if (AvecVideos)
    {
        scriptbackup += "\n";
        scriptbackup +=  "cp -R $DIR_VIDEOS $BACKUP_DIR/$DATE/Videos";
    }
    // copie Rufus.ini
    scriptbackup += "\n";
    scriptbackup +=  "cp $RUFUSINI $BACKUP_DIR/$DATE/Rufus.ini";
    if (QFile::exists(QDir::homePath() + SCRIPTBACKUPFILE))
        QFile::remove(QDir::homePath() + SCRIPTBACKUPFILE);
    QFile fbackup(QDir::homePath() + SCRIPTBACKUPFILE);
    if (fbackup.open(QIODevice::ReadWrite))
    {
        QTextStream out(&fbackup);
        out << scriptbackup ;
        fbackup.close();
    }
}

void RufusAdmin::Slot_EffacePrgSauvegarde()
{
    QList<QRadioButton*> listbutton2 = ui->JourSauvegardegroupBox->findChildren<QRadioButton*>();
    for (int i=0; i<listbutton2.size(); i++)
       listbutton2.at(i)->setChecked(false);
    ui->DirBackupuplineEdit->setText("");
    ui->HeureBackuptimeEdit->setTime(QTime(0,0));
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set HeureBkup = ''");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set DirBkup = ''");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set LundiBkup = NULL");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set MardiBkup = NULL");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set MercrediBkup = NULL");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set JeudiBkup = NULL");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set VendrediBkup = NULL");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set SamediBkup = NULL");
    db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set DimancheBkup = NULL");
#ifdef Q_OS_MACX
    QString unload  = "bash -c \"/bin/launchctl unload \"" + QDir::homePath();
    unload += SCRIPTPLISTFILE "\"\"";
    QProcess dumpProcess(parent());
    dumpProcess.start(unload);
    dumpProcess.waitForFinished();
    if (QFile::exists(QDir::homePath() + SCRIPTBACKUPFILE))
        QFile::remove(QDir::homePath() + SCRIPTBACKUPFILE);
    if (QFile::exists(QDir::homePath() + SCRIPTPLISTFILE))
        QFile::remove(QDir::homePath() + SCRIPTPLISTFILE);
#endif
}

bool RufusAdmin::ImmediateBackup()
{
    QString req = "select NomPosteConnecte from " NOM_TABLE_USERSCONNECTES " where NomPosteConnecte <> '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS + "'";
    QList<QList<QVariant>> listpostes = db->StandardSelectSQL(req, ok);
    if (listpostes.size() > 0)
    {
        UpMessageBox::Information(this, tr("Autres postes connectés!"),
                                     tr("Vous ne pouvez pas effectuer d'opération de sauvegarde/restauration sur la base de données"
                                     " si vous n'êtes pas le seul poste connecté.\n"
                                     "Le poste ") + listpostes.at(0).at(0).toString() + tr(" est aussi connecté"));
        show();
        return false;
    }

    QString NomDirStockageImagerie("");
    QString NomDirDestination ("");
    QList<QVariant> dirdata = db->getFirstRecordFromStandardSelectSQL("select dirimagerie, DirBkup from " NOM_TABLE_PARAMSYSTEME, ok);
    if (ok && dirdata.size()>0)
    {
        NomDirStockageImagerie = dirdata.at(0).toString();
        NomDirDestination = dirdata.at(1).toString();
    }
    if(!QDir(NomDirDestination).exists() || NomDirDestination == "")
    {
        if (UpMessageBox::Question(this,
                                   tr("Pas de destination"),
                                   NomDirDestination == ""?
                                   tr("Vous n'avez pas spécifié de dossier de destination\npour la sauvegarde\nVoulez-vous le faire maintenant?") :
                                   tr("Le dossier de destination de sauvegarde") + "\n" +  NomDirDestination + "\n" + tr("nest pas valide\nVoulez-vous choisir un autre dossier?"),
                                   UpDialog::ButtonCancel | UpDialog::ButtonOK,
                                   QStringList() << tr("Annuler") << tr("Choisir un dossier"))
            == UpSmallButton::STARTBUTTON)
        {
            QString dirSauv         = QFileDialog::getExistingDirectory(this,tr("Choisissez le dossier dans lequel vous voulez sauvegarder la base\n"
                                                                        "Le nom de dossier ne doit pas contenir d'espace"), QDir::homePath());
            if (dirSauv == "")
                return false;
            if (dirSauv.contains(" "))
            {
                UpMessageBox::Watch(this, tr("Nom de dossier non conforme"),tr("Vous ne pouvez pas choisir un dossier dont le nom contient des espaces"));
                return false;
            }
            NomDirDestination = dirSauv;
        }
        else return false;
    }
    DisconnectTimerInactive();
    AskBupRestore(false, NomDirStockageImagerie, NomDirDestination);
    if (gAskBupRestore->exec()==0)
    {
        ConnectTimerInactive();
        return false;
    }

    Message(tr("Sauvegarde en cours"),3000,false);

    QList<UpCheckBox*> listchk = gAskBupRestore->findChildren<UpCheckBox*>();
    bool OKbase     = false;
    bool OKImages   = false;
    bool OKVideos   = false;
    for (int i= 0; i<listchk.size(); i++)
    {
        if (listchk.at(i)->accessibleDescription() == "base")
            OKbase = listchk.at(i)->isChecked();
        else if (listchk.at(i)->accessibleDescription() == "images")
            OKImages = listchk.at(i)->isChecked();
        else if (listchk.at(i)->accessibleDescription() == "videos")
            OKVideos = listchk.at(i)->isChecked();
    }
    if (!OKbase && !OKImages && !OKVideos)
    {
        ConnectTimerInactive();
        return false;
    }
    if (OKbase)
    {
        QFile precBup(QDir::homePath() + SCRIPTBACKUPFILE);
        bool b = precBup.exists();
        DefinitScriptBackup(NomDirStockageImagerie, OKImages, OKVideos);
        QString msg = "sh " + QDir::homePath() + SCRIPTBACKUPFILE;
        QProcess dumpProcess(parent());
        dumpProcess.start(msg);
        dumpProcess.waitForFinished(1000000000);
        int  a = 99;
        if (dumpProcess.exitStatus() == QProcess::NormalExit)
            a = dumpProcess.exitCode();
        if (a == 0)
            msg = tr("Sauvegarde effectuée avec succès");
        else
            msg = tr("Incident pendant la sauvegarde - code incident") + " " + QString::number(a);
        Message(msg,3000,false);
        if (b)
            DefinitScriptBackup(NomDirStockageImagerie);
        else
            QFile (QDir::homePath() + SCRIPTBACKUPFILE).remove();

    }
    else
    {
        QString dest = NomDirDestination + "/" + QDateTime::currentDateTime().toString("yyyyMMdd-HHmm");
        if (OKImages || OKVideos)
            Utils::mkpath(dest);
        if (OKImages)
        {
            QString Msg = (tr("Sauvegarde des fichiers d'imagerie\n")
                           + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
            Message(Msg, 3000);
            QProcess::execute("cp -R " + NomDirStockageImagerie + NOMDIR_IMAGES + " " + dest);
            Message(tr("Fichiers d'imagerie restaurés!"), 3000, false);
        }
        if (OKVideos)
        {
            QString Msg = (tr("Sauvegarde des fichiers videos\n")
                           + tr("Ce processus peut durer plusieurs minutes en fonction de la taille de la base de données"));
            Message(Msg, 3000);
            QProcess::execute("cp -R " + NomDirStockageImagerie + NOMDIR_VIDEOS + " " + dest);
            Message(tr("Fichiers d'imagerie restaurés!"), 3000, false);
        }
    }
    ConnectTimerInactive();
    UpMessageBox::Watch(this, tr("Sauvegarde terminée"));
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
    -- Gestion du flag de mise à jour de la liste des correspondants -----------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------------------------------------*/
int RufusAdmin::GetflagCorrespdts()
{
    QList<QVariant> flagrcd = db->getFirstRecordFromStandardSelectSQL("select MAJflagMG from " NOM_TABLE_FLAGS, ok);
    if (ok && flagrcd.size() > 0)
        return flagrcd.at(0).toInt();
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
    -- Gestion du flag de mise à jour de l'arrivée de nouveaux messages -----------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------------------------------------*/
int RufusAdmin::GetflagMessages()
{
    QList<QVariant> flagmsgrcd = db->getFirstRecordFromStandardSelectSQL("select MAJflagMessages from " NOM_TABLE_FLAGS, ok);
    if (ok && flagmsgrcd.size() > 0)
        return flagmsgrcd.at(0).toInt();
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
    -- Gestion du flag de mise à jour de la salle d'attente -----------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------------------------------------*/
int RufusAdmin::GetflagSalDat()
{
    QList<QVariant> flagsaldatrcd = db->getFirstRecordFromStandardSelectSQL("select MAJflagSalDat from " NOM_TABLE_FLAGS, ok);
    if (ok && flagsaldatrcd.size() > 0)
        return flagsaldatrcd.at(0).toInt();
    return 0;
}

void RufusAdmin::FermeTCP()
{
        TCPServer->close();
        db->StandardSQL("update " NOM_TABLE_PARAMSYSTEME " set AdresseTCPServeur = null");
        delete TCPServer;
}

void RufusAdmin::ResumeTCPSocketStatut()
{
    QString statut;
    QString list = TCPServer->ListeSockets().remove("{}" TCPMSG_ListeSockets);
    QStringList ListSockets = list.split("{}");
    // le 1er item de gListSockets est le serveur
    QString Serveur = ListSockets.at(0);
    statut += tr("ServeurTCP") + "\n\t"
            + Serveur.split(TCPMSG_Separator).at(2) + " - "
            + gIPadr + " - "
            + Serveur.split(TCPMSG_Separator).at(1) + " --- "
            + Datas::I()->users->getLoginById(Serveur.split(TCPMSG_Separator).at(3).toInt());

    ListSockets.removeFirst();
    statut += "\n" + tr("postes connectés") + "\n";
    QStringList::const_iterator itsocket;
    for( itsocket = ListSockets.constBegin(); itsocket != ListSockets.constEnd(); ++itsocket )
    {
        statut += "\t" + itsocket->split(TCPMSG_Separator).at(2) + " - "
                + itsocket->split(TCPMSG_Separator).at(0) + " - "
                + itsocket->split(TCPMSG_Separator).at(1) + " --- "
                + Datas::I()->users->getLoginById(itsocket->split(TCPMSG_Separator).at(3).toInt()) + "\n";
    }
    gSocketStatut = statut;
    emit ModifEdit(gSocketStatut); // déclenche la modification de la fenêtre resumestatut
}

void RufusAdmin::Edit(QString txt, int delaieffacement)
{
    UpDialog        *gAsk           = new UpDialog(this);
    QVBoxLayout     *globallay      = dynamic_cast<QVBoxLayout*>(gAsk->layout());
    UpTextEdit      *TxtEdit        = new UpTextEdit(gAsk);
    int x = qApp->desktop()->availableGeometry().width();
    int y = qApp->desktop()->availableGeometry().height();

    gAsk->setModal(true);
    gAsk->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

    TxtEdit->setText(txt);
    TxtEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    connect(this,   &RufusAdmin::ModifEdit, TxtEdit, [=](QString txt) {TxtEdit->setText(txt);});

    gAsk->setMaximumWidth(x);
    gAsk->setMaximumHeight(y);

    globallay->insertWidget(0,TxtEdit);

    gAsk->AjouteLayButtons();
    connect(gAsk->OKButton,SIGNAL(clicked(bool)),gAsk,SLOT(accept()));
    gAsk->restoreGeometry(gsettingsIni->value("PositionsFiches/PositionEdit").toByteArray());

    if (delaieffacement > 0)
    {
        QTimer *tim = new QTimer(gAsk);
        tim->setSingleShot(true);
        connect(tim, &QTimer::timeout, gAsk, &UpDialog::accept);
        tim->start(delaieffacement);
    }

    gAsk->exec();
    gsettingsIni->setValue("PositionsFiches/PositionEdit",gAsk->saveGeometry());
    delete gAsk;
}

void RufusAdmin::VerifVerrouDossier()
{
    /* Cette fonction sert à déconnecter et lever les verrous d'un utilisateur qui se serait déconnecté accidentellement
     *
     on fait la liste des utilisateurs qui n'ont pas remis à jour leur connexion depuis plus de 60 secondes,
     on retire les verrous qu'ils auraient pu poser et on les déconnecte*/
    bool mettreajourlasalledattente = false;

    // on donne le statut "arrivé" aux patients en salle d'attente dont le iduserencourssexam n'est plus present sur ce poste examen dans la liste des users connectes
    QString req = "select iduserencoursexam, posteexamen, idpat from " NOM_TABLE_SALLEDATTENTE " where statut like '" ENCOURSEXAMEN "%'";
    //qDebug() << req;
    QList<QList<QVariant>> listuser = db->StandardSelectSQL(req, ok);
    for (int i=0; i<listuser.size(); i++)
    {
        req = "select iduser, nomposteconnecte from " NOM_TABLE_USERSCONNECTES " where iduser = " + listuser.at(i).at(0).toString()  + " and nomposteconnecte = '" + listuser.at(i).at(1).toString() + "'";
        QList<QVariant> userconnect = db->getFirstRecordFromStandardSelectSQL(req, ok);
        if (userconnect.size()==0)
        {
            req = "update " NOM_TABLE_SALLEDATTENTE " set Statut = '" ARRIVE "', posteexamen = null, iduserencoursexam = null where idpat = " + listuser.at(i).at(2).toString();
            db->StandardSQL(req);
        }
        mettreajourlasalledattente = true;
    }
    if (mettreajourlasalledattente)
        MAJTcpMsgEtFlagSalDat();
}

/*------------------------------------------------------------------------------------------------------------------------------------
-- Modifier la table UtilisateursConnectes pour signifier aux autres utilisateurs que la salle d'attente vient d'être modifiée --------------------
------------------------------------------------------------------------------------------------------------------------------------*/
void RufusAdmin::MAJTcpMsgEtFlagSalDat()
{
    /* envoi du message de MAJ de la salle d'attente aux clients */
    //TCPServer->envoyerATous(TCPMSG_MAJSalAttente);                      // le slot verifverroudossier a déconnecté un tutilisateur et modifié la salle d'attente si des patients étaient verrouillés

    /* mise à jour du flag pour les utilisateurs distants qui le surveillent et mettent ainsi à jour leur salle d'attente */
    QList<QVariant> flagsaldatrcd = db->getFirstRecordFromStandardSelectSQL("select MAJflagSalDat from " NOM_TABLE_FLAGS, ok);
    QString MAJreq = "insert into " NOM_TABLE_FLAGS " (MAJflagSalDat) VALUES (1)";
    int a = 0;
    if (ok && flagsaldatrcd.size()>(0)) {
        a = flagsaldatrcd.at(0).toInt() + 1;
        MAJreq = "update " NOM_TABLE_FLAGS " set MAJflagSalDat = " + QString::number(a);
    }
    db->StandardSQL(MAJreq);
    gflagSalDat = a;
}


/*!
 * \brief RufusAdmin::initListeBanques
 * Charge l'ensemble des banques
 * et les ajoute à la classe Banques
 */
void RufusAdmin::initListeBanques()
{
    QList<Banque*> listbanques = DataBase::getInstance()->loadBanques();
    QList<Banque*>::const_iterator itbq;
    for( itbq = listbanques.constBegin(); itbq != listbanques.constEnd(); ++itbq )
    {
        Banque *bq = const_cast<Banque*>(*itbq);
        Datas::I()->banques->addBanque( bq );
    }
}
/*!
 * \brief RufusAdmin::initListeMotifs
 * Charge l'ensemble des motifs d'actes
 * et les ajoute à la classe Motifs
 */
void RufusAdmin::initListeMotifs()
{
    QList<Motif*> listmotifs = DataBase::getInstance()->loadMotifs();
    QList<Motif*>::const_iterator itmtf;
    for( itmtf = listmotifs.constBegin(); itmtf != listmotifs.constEnd(); ++itmtf )
    {
        Motif *mtf = const_cast<Motif*>(*itmtf);
        Datas::I()->motifs->addMotif(mtf);
    }
}

void RufusAdmin::KillSocket(QStringList datas)  //TODO marche mal quand le client se reconnecte, ça ne marche plus
                                                // - accessoirement, ça marche parfaitement quand on ne s'en sert pas
{
    int idUserAEliminer = datas.at(0).toInt();
    QString MACAdressUserAEliminer = datas.at(1);
    TCPServer->Deconnexion(idUserAEliminer, MACAdressUserAEliminer);
}


void RufusAdmin::VerifModifsSalledAttenteCorrespondantsetNouveauxMessages()
/* Utilisé pour vérifier
 *      des modifs de la salle d'attente
 *      des modifs de la liste des correspondants
 *      ou l'arrivée de nouveaux  messages
 *  effectués par des postes distants
 */
{
    int flag = GetflagSalDat();
    if (gflagSalDat < flag)
    {
        gflagSalDat = flag;
        TCPServer->envoyerATous(TCPMSG_MAJSalAttente);
    }
    flag = GetflagCorrespdts();
    if (gflagCorrespdts < flag)
    {
        gflagCorrespdts = flag;
        TCPServer->envoyerATous(TCPMSG_MAJCorrespondants);
    }
    flag = GetflagMessages();
    if (gflagMessages < flag)
    {
        gflagMessages = flag;

        QString req =
                "select mess.idMessage, iddestinataire, creele from "
                NOM_TABLE_MESSAGES " mess left outer join " NOM_TABLE_MESSAGESJOINTURES " joint on mess.idmessage = joint.idmessage \n"
                " where Creele > '" + gDateDernierMessage.toString("yyyy-MM-dd HH:mm:ss")
                + "' and asupprimer is null"
                + " order by creele";
        QList<QList<QVariant>> listmsg = db->StandardSelectSQL(req, ok);
        int TotalNvxMessages = listmsg.size();
        QHash<int,int> mapmessages;
        if (TotalNvxMessages>0)
        {
            for (int i=0; i<TotalNvxMessages; i++)
            {
                QHash<int,int>::const_iterator itm = mapmessages.find(listmsg.at(i).at(1).toInt());
                if (itm != mapmessages.constEnd())
                    mapmessages[itm.key()] = itm.value()+1;
                else
                    mapmessages[listmsg.at(i).at(1).toInt()] = 1;
            }
            gDateDernierMessage = listmsg.at(listmsg.size()-1).at(2).toDateTime();
        }
        for (QHash<int,int>::const_iterator itmsg = mapmessages.constBegin(); itmsg != mapmessages.constEnd(); ++itmsg)
            TCPServer->envoyerA(itmsg.key(), TCPMSG_Separator + QString::number(itmsg.value()) + TCPMSG_MsgBAL);
    }
}


