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

#include "rufusadmin.h"
#include "ui_rufusadmin.h"

RufusAdmin::RufusAdmin(QWidget *parent) : QMainWindow(parent), ui(new Ui::RufusAdmin)
{
    Datas::I();
    // la version du programme correspond à la date de publication, suivie de "/" puis d'un sous-n° - p.e. "23-6-2017/3"
    qApp->setApplicationVersion("19-10-2018/1");       // doit impérativement être composé de date version / n°version);

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
    QString NomDirRufus = QDir::homePath() + NOMDIR_RUFUS;
    QDir DirRufus(NomDirRufus);
    if (!DirRufus.exists())
        DirRufus.mkdir(NomDirRufus);

    RestoreFontAppli(); // les polices doivent être appliquées après la définition des styles
    setMapDatas();    

    /*QString adressexml = QDir::homePath() + SCRIPTPLISTFILE;
    QFile xmldoc(adressexml);
    if (xmldoc.open(QIODevice::ReadOnly))
    {
        QDomDocument *docxml = new QDomDocument();
        docxml->setContent(&xmldoc);
        QString info;
        QDomNode n = docxml->firstChild();
        while(!n.isNull()) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(!e.isNull()) {
                info += "Nom de la balise\t" + e.tagName()+"\n"; // the node really is an element.
                QDomNode n1 = n.firstChild();
                while(!n1.isNull()) {
                    QDomElement e1 = n1.toElement(); // try to convert the node to an element.
                    if(!e1.isNull()){
                        info += "Nom de la balise\t" + e1.tagName()+"\n"; // the node really is an element.
                        QDomNode n2 = n.firstChild();
                        while(!n2.isNull()) {
                            QDomElement e2 = n2.toElement(); // try to convert the node to an element.
                            if(!e2.isNull()){
                                info += "Nom de la balise\t" + e2.tagName()+"\n"; // the node really is an element.
                            }
                            n2 = n2.nextSibling();
                        }
                    }
                    n1 = n1.nextSibling();
                }
            }
            n = n.nextSibling();
        }

        UpDialog        *gAsk           = new UpDialog();
        QVBoxLayout     *globallay      = dynamic_cast<QVBoxLayout*>(gAsk->layout());
        QTextEdit *gTxtEdit              = new QTextEdit(gAsk);
        gAsk->setModal(true);
        gTxtEdit->setText(info);

        globallay->insertWidget(0,gTxtEdit);

        gAsk->AjouteLayButtons();
        connect(gAsk->OKButton,SIGNAL(clicked(bool)),gAsk,SLOT(accept()));

        gAsk->exec();
        delete gAsk;
    }*/
    ConnexionBase();
    if (!VerifBase())
        exit(0);
    idlieuExercice = DetermineLieuExercice();
    gNomLieuExercice = "";
    QSqlQuery lieuquer("select nomlieu from " NOM_TABLE_LIEUXEXERCICE " where idlieu = " + QString::number(idlieuExercice), db);
    lieuquer.first();
    gNomLieuExercice = lieuquer.value(0).toString();
    ui->AppareilsconnectesupLabel->setText(tr("Appareils connectés au réseau") + " <font color=\"green\"><b>" + gNomLieuExercice + "</b></font> ");

    //recherche de l'idUser du compte AdminDocs
    idAdminDocs = 0;
    QString req = "select iduser from " NOM_TABLE_UTILISATEURS " where UserNom = '" NOM_ADMINISTRATEURDOCS "'";
    QSqlQuery usrquer(req, db);
    if (usrquer.size()==0)
    {
        QSqlQuery("insert into " NOM_TABLE_UTILISATEURS " (UserNom, UserLogin) values ('" NOM_ADMINISTRATEURDOCS "','" NOM_ADMINISTRATEURDOCS "')",db);
        usrquer.exec();
        QSqlQuery mdpquer("select mdpadmin from " NOM_TABLE_PARAMSYSTEME,db);
        mdpquer.first();
        if (mdpquer.value(0).toString() == "")
            QSqlQuery("update " NOM_TABLE_PARAMSYSTEME " set mdpadmin = '" +db.password() + "'", db);
    }
    usrquer.first();
    UserAdmin = new User(DataBase::getInstance()->loadUserDatabyLogin(NOM_ADMINISTRATEURDOCS));
    idAdminDocs = UserAdmin->id();

    // on vérifie que le programme n'est pas déjà en cours d'éxécution sur un autre poste
    QString reqp = "select NomPosteConnecte from " NOM_TABLE_USERSCONNECTES
                   " where idUser = " + QString::number(idAdminDocs) +
                   " and NomPosteConnecte != '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                   " and idlieu = " + QString::number(idlieuExercice) +
                   " and time_to_sec(timediff(now(),heurederniereconnexion)) < 60";
    QSqlQuery usr2quer(reqp, db);
    if (usr2quer.size()>0)
    {
        usr2quer.first();
        UpMessageBox::Watch(this, tr("Programme déjà en cours d'éxécution sur le poste ") + usr2quer.value(0).toString().remove(" - " NOM_ADMINISTRATEURDOCS), tr("Sortie du programme"));
        exit(0);
    }
    else
        QSqlQuery("delete from " NOM_TABLE_USERSCONNECTES " where idUser = " + QString::number(idAdminDocs) + " and idlieu = " + QString::number(idlieuExercice), db);

    // 5 mettre en place le TcpSocket
    gIPadr                      = Utils::getIpAdress();
    gMacAdress                  = Utils::getMACAdress();
    TcpServer                   = new GestionTcPServer(this);
    gTimerSalDatEtCorresp       = new QTimer(this);     /* scrutation des modifs de la salle d'attente et des correspondants utilisé par
                                                           le TCPServer pour verifier les modifications faites par les postes distants
                                                        */
    gTimerVerifVerrou           = new QTimer(this);     // utilisé par le TcpServer pour vérifier l'absence d'utilisateurs déconnectés dans la base
    gTimerVerifServeur          = new QTimer(this);     // vérification de la persistance du serveur
    InitTCP();

    gTimerUserConnecte          = new QTimer(this);     // mise à jour de la connexion à la base de données
    gTimerSupprDocs             = new QTimer(this);     // utilisé par le poste importateur pour vérifier s'il y a des documents à supprimer
    gTimerVerifDivers           = new QTimer(this);     // vérification du poste importateur des documents et e la version de la base
    gTimerInactive              = new QTimer(this);     // reduction de la fenêtre dans la barre des taches
    gTimerSupprDocs             = new QTimer(this);     // verification des documents à supprimer

    setPosteImportDocs(); // on prend la place d'importateur des documents dans les utilisateurs connectés
    Slot_VerifPosteImport();
    Slot_VerifVersionBase();
    Slot_CalcExporteDocs();

    // Lancement du timer de vérification des verrous - +++ à lancer après le timer gTimerVerifGestDocs puisqu'il l'utilise
    gTimerVerifVerrou->start(60000);// "toutes les 60 secondes"

    gTimerSalDatEtCorresp   ->start(1000);
    gTimerUserConnecte      ->start(10000);
    gTimerVerifDivers       ->start(30000);

    dureeVeille = 60000;
    gTimerInactive->setSingleShot(true);
    gTimerInactive->setInterval(dureeVeille);

    gTimerSupprDocs->start(60000);// "toutes les 60 secondes"
    gTimerDocsAExporter = new QTimer(this);
    gTimerDocsAExporter->start(60000);// "toutes les 60 secondes"
    // Lancement du timer de suppression des fichiers documents inutiles
    if (gMode != Distant)
    {
        connect(gTimerSupprDocs, SIGNAL(timeout()), this, SLOT(Slot_SupprimerDocs()));
        connect(gTimerDocsAExporter, SIGNAL(timeout()), this, SLOT(Slot_CalcExporteDocs()));
    }
    ConnectTimers();

    connect(ui->EmplacementServeurupComboBox,   SIGNAL(currentIndexChanged(int)),   this,   SLOT(Slot_EnregistreEmplacementServeur(int)));
    connect(ui->ExportImagespushButton,         SIGNAL(clicked(bool)),              this,   SLOT(Slot_ExporteDocs()));
    connect(ui->FermepushButton,                SIGNAL(clicked(bool)),              this,   SLOT(Slot_MasqueAppli()));
    connect(ui->GestionBanquespushButton,       SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestionBanques()));
    connect(ui->GestLieuxpushButton,            SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestLieux()));
    connect(ui->GestUserpushButton,             SIGNAL(clicked(bool)),              this,   SLOT(Slot_GestUser()));
    connect(ui->InitMDPAdminpushButton,         SIGNAL(clicked(bool)),              this,   SLOT(Slot_ModifMDP()));;
    connect(ui->ParamMotifspushButton,          SIGNAL(clicked(bool)),              this,   SLOT(Slot_ParamMotifs()));
    connect(ui->RestaurBaseupPushButton,        SIGNAL(clicked(bool)),              this,   SLOT(Slot_RestaureBase()));
    connect(ui->StockageupPushButton,           SIGNAL(clicked(bool)),              this,   SLOT(Slot_ModifDirImagerie()));


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
        QSqlQuery dirquer("select dirimagerie from " NOM_TABLE_PARAMSYSTEME, db);
        dirquer.first();
        QString NomDirStockageImagerie = dirquer.value(0).toString();
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
    QSqlQuery VersionBaseQuery(VerifBasereq,db);
    if (VersionBaseQuery.lastError().type() != QSqlError::NoError || VersionBaseQuery.size()==0)
        ui->VersionBaselabel->setText(tr("Version de la base") + "\t<font color=\"red\"><b>" + tr("inconnue") + "</b></font>");
    else
    {
        VersionBaseQuery.first();
        ui->VersionBaselabel->setText(tr("Version de la base ") + "<font color=\"green\"><b>" + VersionBaseQuery.value(0).toString() + "</b></font>");
    }
    ui->VersionRufuslabel->setText(tr("Version de RufusAdmin ") + "<font color=\"green\"><b>" + qApp->applicationVersion() + "</b></font>");

    ui->Sauvegardeframe         ->setEnabled(gMode == Poste);
    ui->ParamSauvegardelabel    ->setVisible(gMode != Poste);
    ui->ParamSauvegardelabel    ->setEnabled(gMode != Poste);

    if (gMode == Poste)
    {
        QString reqBkup = "select LundiBkup, MardiBkup, MercrediBkup, JeudiBkup, VendrediBkup, SamediBkup, DimancheBkup, HeureBkup, DirBkup from " NOM_TABLE_PARAMSYSTEME;
        QSqlQuery querBkup(reqBkup, db);
        TraiteErreurRequete(querBkup, reqBkup);
        querBkup.first();
        QString DirBkup = querBkup.value(8).toString();
        if (QDir(DirBkup).exists())
            ui->DirBackupuplineEdit->setText(DirBkup);
        if (querBkup.value(7).toTime().isValid())
            ui->HeureBackuptimeEdit ->setTime(querBkup.value(7).toTime());
        ui->LundiradioButton    ->setChecked(querBkup.value(0).toInt()==1);
        ui->MardiradioButton    ->setChecked(querBkup.value(1).toInt()==1);
        ui->MercrediradioButton ->setChecked(querBkup.value(2).toInt()==1);
        ui->JeudiradioButton    ->setChecked(querBkup.value(3).toInt()==1);
        ui->VendrediradioButton ->setChecked(querBkup.value(4).toInt()==1);
        ui->SamediradioButton   ->setChecked(querBkup.value(5).toInt()==1);
        ui->DimancheradioButton ->setChecked(querBkup.value(6).toInt()==1);
        connect(ui->DirBackuppushButton,        SIGNAL(clicked(bool)),      this,   SLOT(Slot_ModifDirBackup()));
        QList<QRadioButton*> listbutton2 = ui->JourSauvegardegroupBox->findChildren<QRadioButton*>();
        for (int i=0; i<listbutton2.size(); i++)
            connect(listbutton2.at(i),          SIGNAL(clicked(bool)),      this,   SLOT(Slot_ModifDateBackup()));
        connect(ui->HeureBackuptimeEdit,        SIGNAL(timeChanged(QTime)), this,   SLOT(Slot_ModifDateBackup()));
        connect(ui->EffacePrgSauvupPushButton,  SIGNAL(clicked(bool)),      this,   SLOT(Slot_EffacePrgSauvegarde()));
        connect(ui->ImmediatBackupupPushButton, &QPushButton::clicked,      [=] {ImmediateBackup();});
    }

    trayIconMenu = new QMenu();

    gRufusAdminTrayIcon = new QSystemTrayIcon();
    gRufusAdminTrayIcon->setContextMenu(trayIconMenu);
    gRufusAdminTrayIcon->setIcon(gIconRufusAdmin);
    gRufusAdminTrayIcon->setVisible(true);
    connect(trayIconMenu, SIGNAL(aboutToShow()), this,  SLOT(Slot_TrayIconMenu()));
    ui->MessageupLabel->setText("");

    installEventFilter(this);
}

RufusAdmin::~RufusAdmin()
{
    delete ui;
}

void RufusAdmin::closeEvent(QCloseEvent *)
{
    // on retire le poste de la variable posteimportdocs SQL
    setPosteImportDocs(false);
    // on retire Admin de la table des utilisateurs connectés
    QString req = "delete from " NOM_TABLE_USERSCONNECTES
                  " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                  " and idlieu = " + QString::number(idlieuExercice);
    QSqlQuery qer(req,db);
    TraiteErreurRequete(qer,req,"");
}

void RufusAdmin::AskAppareil()
{
    disconnect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
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
    connect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
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
        DataDir = dir_size(pathorigin + NOMDIR_VIDEOS);
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
        DataDir = dir_size(pathorigin + NOMDIR_IMAGES);
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
        connect (gTimerUserConnecte,    SIGNAL(timeout()),      this,   SLOT(Slot_ExporteDocs()));
        connect (gTimerUserConnecte,    SIGNAL(timeout()),      this,   SLOT(Slot_MetAJourLaConnexion()));
        connect (gTimerSupprDocs,       SIGNAL(timeout()),      this,   SLOT(Slot_SupprimerDocs()));
        connect (gTimerDocsAExporter,   SIGNAL(timeout()),      this,   SLOT(Slot_CalcExporteDocs()));
    }
    connect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
    connect (gTimerSalDatEtCorresp,     &QTimer::timeout,       this,   &RufusAdmin::VerifSalleDAttenteEtCorrespondants);
    connect (gTimerVerifServeur,        &QTimer::timeout,       this,   &RufusAdmin::VerifServeur);
    connect (gTimerVerifVerrou,         &QTimer::timeout,       this,   &RufusAdmin::VerifVerrouDossier);
}

void RufusAdmin::DisconnectTimers()
{
    disconnect (gTimerUserConnecte,     SIGNAL(timeout()),      this,   SLOT(Slot_MetAJourLaConnexion()));
    disconnect (gTimerSupprDocs,        SIGNAL(timeout()),      this,   SLOT(Slot_SupprimerDocs()));
    disconnect (gTimerVerifDivers,      SIGNAL(timeout()),      this,   SLOT(Slot_VerifPosteImport()));
    disconnect (gTimerVerifDivers,      SIGNAL(timeout()),      this,   SLOT(Slot_VerifVersionBase()));
    disconnect (gTimerUserConnecte,     SIGNAL(timeout()),      this,   SLOT(Slot_ImportDocsExternes()));
    disconnect (gTimerDocsAExporter,    SIGNAL(timeout()),      this,   SLOT(Slot_CalcExporteDocs()));
    disconnect (gTimerUserConnecte,     SIGNAL(timeout()),      this,   SLOT(Slot_ExporteDocs()));
    disconnect (gTimerInactive,         SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
    gTimerSalDatEtCorresp   ->disconnect();
    gTimerVerifServeur      ->disconnect();
    gTimerVerifVerrou       ->disconnect();
}

void RufusAdmin::ConnectTimerInactive()
{
    connect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
}

void RufusAdmin::DisconnectTimerInactive()
{
    disconnect (gTimerInactive,         SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
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
    QSqlQuery sizequer (sizereq,db);
    if (sizequer.size()>0)
    {
        sizequer.first();
        basesize = sizequer.value(0).toDouble();
    }
    return basesize;
}

QMap<QString, double> RufusAdmin::dir_size(const QString DirPath)
{
    QMap<QString, double>      DataDir;
    double sizex = 0;
    double nfiles = 0;

    QDir dir(DirPath);
    if(!dir.exists())
    {
        DataDir["Size"]= 0;
        DataDir["Nfiles"]= 0;
        return DataDir;
    }
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        sizex += (fileInfo.isDir()) ? this->dir_size(fileInfo.absoluteFilePath())["Size"]: fileInfo.size();
        nfiles += (fileInfo.isDir()) ? this->dir_size(fileInfo.absoluteFilePath())["Nfiles"] : i+1;
    }
    DataDir["Size"]= sizex;
    DataDir["Nfiles"]= nfiles;
    return DataDir;
}

void RufusAdmin::ConnexionBase()
{
    DlgParam = new dlg_paramconnexion();
    if (DlgParam->exec()>0)
        db = DataBase::getInstance()->getDataBase();
    else
        exit(0);
    gMode = DataBase::getInstance()->getMode();
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
        //qDebug() << lieuxreq;
        QSqlQuery lxquer(lieuxreq,db);
        TraiteErreurRequete(lxquer, lieuxreq);
        if (lxquer.size()==1)
        {
            lxquer.first();
            idLieu = lxquer.value(0).toInt();
        }
        else if (lxquer.size()>1)
        {
            disconnect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
            UpDialog *gAskLieux     = new UpDialog();
            gAskLieux               ->AjouteLayButtons();
            QVBoxLayout *globallay  = dynamic_cast<QVBoxLayout*>(gAskLieux->layout());
            QGroupBox*boxlieux      = new QGroupBox();
            globallay               ->insertWidget(0,boxlieux);
            boxlieux                ->setAccessibleName("Parent");
            boxlieux                ->setTitle(tr("D'où vous connectez-vous?"));

            QFontMetrics fm         = QFontMetrics(qApp->font());
            int hauteurligne        = int(fm.height()*1.6);
            boxlieux                ->setFixedHeight(((lxquer.size() + 1)*hauteurligne)+5);
            QVBoxLayout *vbox       = new QVBoxLayout;
            for (int i=0; i<lxquer.size(); i++)
            {
                lxquer          .seek(i);
                UpRadioButton   *pradiobutt = new UpRadioButton(boxlieux);
                pradiobutt      ->setText(lxquer.value(1).toString());
                pradiobutt      ->setAccessibleName(lxquer.value(0).toString());
                QString data ("");
                if (lxquer.value(1).toString()!="")
                    data += lxquer.value(1).toString();
                if (data == "" )
                {
                    data += lxquer.value(2).toString();
                    if (lxquer.value(6).toString()!="")
                        data += (data != ""? " " : "") + lxquer.value(6).toString();
                }
                if (lxquer.value(6).toString()!="")
                    data += (data != ""? " - " : "") + lxquer.value(6).toString();
                data = "";
                if (lxquer.value(1).toString()!="")
                    data += lxquer.value(1).toString();
                if (lxquer.value(2).toString()!="")
                    data += (data != ""? "\n" : "") + lxquer.value(2).toString();
                if (lxquer.value(3).toString()!="")
                    data += (data != ""? "\n" : "") + lxquer.value(3).toString();
                if (lxquer.value(4).toString()!="")
                    data += (data != ""? "\n" : "") + lxquer.value(4).toString();
                if (lxquer.value(5).toString()!="")
                    data += (data != ""? "\n" : "") + lxquer.value(5).toString();
                if (lxquer.value(6).toString()!="")
                    data += (data != ""? " " : "") + lxquer.value(6).toString();
                if (lxquer.value(7).toString()!="")
                    data += (data != ""? "\nTel: " : "Tel: ") + lxquer.value(7).toString();
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
            connect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
        }
    }
    else
    {
        QString lieuxreq = "select idLieuParDefaut from " NOM_TABLE_PARAMSYSTEME;
        QSqlQuery lxquer(lieuxreq,db);
        TraiteErreurRequete(lxquer,lieuxreq);
        lxquer.first();
        if (lxquer.value(0).toInt()>=1)
            idLieu = lxquer.value(0).toInt();
        else
        {
            QSqlQuery lieuquer("select min(idlieu) from " NOM_TABLE_LIEUXEXERCICE,db);
            lieuquer.first();
            idLieu = lieuquer.value(0).toInt();
            QSqlQuery("update " NOM_TABLE_PARAMSYSTEME " set idLieuParDefaut = " + lieuquer.value(0).toString(), db);
        }
    }
    return idLieu;
}
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Interception des évènements internes -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
QDebug operator<<(QDebug str, const QEvent * ev)
{
   static int eventEnumIndex = QEvent::staticMetaObject
         .indexOfEnumerator("Type");
   str << "QEvent";
   if (ev) {
      QString name = QEvent::staticMetaObject
            .enumerator(eventEnumIndex).valueToKey(ev->type());
      if (!name.isEmpty()) str << name; else str << ev->type();
   } else {
      str << (void*)ev;
   }
   return str.maybeSpace();
}

bool RufusAdmin::eventFilter(QObject *obj, QEvent *event)
{
    if (!event->spontaneous())
    {
        //qDebug() << "event = " << event;
        gTimerInactive->start();
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
    QSqlQuery appQuery(req,db);
    if (TraiteErreurRequete(appQuery, req, ""))
        return;
    if (appQuery.size()==0)
        return;
    appQuery.first();
    UpMessageBox msgbox(this);
    msgbox.setText(tr("Suppression d'un appareil!"));
    msgbox.setInformativeText(tr("Voulez vous vraiment supprimer l'appareil") + "\n"
                              + appQuery.value(1).toString() + " ?");
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
        QSqlQuery(req,db);
        gsettingsIni->remove("DossierEchangeImagerie/" + appQuery.value(1).toString());
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
    QSqlQuery RemplirTableViewQuery (Remplirtablerequete,db);
    if (TraiteErreurRequete(RemplirTableViewQuery, Remplirtablerequete,""))
        return;
    ui->AppareilsConnectesupTableWidget->setRowCount(RemplirTableViewQuery.size());
    RemplirTableViewQuery.first();
    for (i = 0; i < RemplirTableViewQuery.size(); i++)
    {
        pItem0      = new QTableWidgetItem() ;
        pItem1      = new QTableWidgetItem() ;
        pItem2      = new QTableWidgetItem() ;
        pItem3      = new QTableWidgetItem() ;
        line4       = new UpLineEdit(ui->AppareilsConnectesupTableWidget);
        dossbouton  = new UpPushButton(ui->AppareilsConnectesupTableWidget);

        int col = 0;
        pItem0->setText(RemplirTableViewQuery.value(0).toString());                             // idAppareil
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem0);

        col++; //1
        pItem1->setText(RemplirTableViewQuery.value(1).toString());                             // TypeExamen
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem1);

        col++; //2
        pItem2->setText(RemplirTableViewQuery.value(2).toString());                             // NomAppareil
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem2);

        col++; //3                                                                              // Format fichier
        pItem3->setText(RemplirTableViewQuery.value(3).toString());
        ui->AppareilsConnectesupTableWidget->setItem(i,col,pItem3);

        col++; //4                                                                              // chemin
        line4->setText(getDossierDocuments(RemplirTableViewQuery.value(2).toString()));
        line4->setRowTable(i);
        line4->setFocusPolicy(Qt::NoFocus);
        line4->setStyleSheet("UpLineEdit {background-color:white; border: 0px solid rgb(150,150,150);border-radius: 0px;}"
                              "UpLineEdit:focus {border: 0px solid rgb(164, 205, 255);border-radius: 0px;}");
        connect(line4,                    SIGNAL(TextModified(QString)),         this,   SLOT(Slot_EnregDossierStockageApp(QString)));
        ui->AppareilsConnectesupTableWidget->setCellWidget(i,col,line4);

        col++; //5                                                                              // bouton
        QPixmap pix = giconSortirDossier.pixmap(QSize(15,15));
        QIcon ic;
        ic.addPixmap(pix);
        dossbouton->setIcon(ic);
        dossbouton->setId(RemplirTableViewQuery.value(0).toInt());
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
        RemplirTableViewQuery.next();
    }

    glistAppareils.clear();
    QString req = "select NomAppareil from " NOM_TABLE_LISTEAPPAREILS
                  " where idAppareil not in (select idAppareil from " NOM_TABLE_APPAREILSCONNECTESCENTRE " where idlieu = " + QString::number(idlieuExercice) + ")";
    QSqlQuery listappquery(req,db);
    if (listappquery.size() == 0)
        widgAppareils->plusBouton->setEnabled(false);
    else
    {
        listappquery.first();
        for (int i=0; i<listappquery.size(); i++)
        {
            glistAppareils << listappquery.value(0).toString();
            listappquery.next();
        }
    }
   widgAppareils->moinsBouton->setEnabled(RemplirTableViewQuery.size()>0);
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
    QSqlQuery quer(req, db);
    TraiteErreurRequete(quer,req);

    req = "DROP PROCEDURE IF EXISTS " NOM_POSTEIMPORTDOCS ";";
    QSqlQuery quer1(req, db);
    TraiteErreurRequete(quer1,req);

    if (a)
        IpAdress = QHostInfo::localHostName() + " - " NOM_ADMINISTRATEURDOCS;
    req = "CREATE PROCEDURE " NOM_POSTEIMPORTDOCS "()\n\
          BEGIN\n\
          SELECT '" + IpAdress + "';\n\
          END ;";
    QSqlQuery quer2(req, db);
    TraiteErreurRequete(quer2,req);
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
    QSqlQuery quer(req,db);
    QString exam = "";
    if (quer.size()>0){
        quer.first();
        exam = quer.value(1).toString();
    }

    QString dir = getDossierDocuments(exam);
    if (dir == "")
        dir = QDir::homePath() + NOMDIR_RUFUSADMIN;
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
    QSqlQuery (req,db);
    gAskAppareil->done(0);
    Remplir_Table();
}

void RufusAdmin::Slot_ModifDirImagerie()
{
    QString dir = ui->StockageupLineEdit->text();
    if (dir == "")
        dir = QDir::homePath() + NOMDIR_RUFUSADMIN;
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
        {
            QString req ="update " NOM_TABLE_PARAMSYSTEME " set dirImagerie = '" + dockdir.path() + "'";
            QSqlQuery quer(req, db);
            TraiteErreurRequete(quer,req);
        }
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
    QSqlQuery quer(req,db);
    QString exam = "";
    if (quer.size()>0){
        quer.first();
        exam = quer.value(0).toString();
    }
    if (exam != "")
        gsettingsIni->setValue("DossierEchangeImagerie/" + exam, dir);
    else
        UpMessageBox::Watch(this,tr("Impossible de retrouver le nom de l'appareil"));
}

void RufusAdmin::Slot_EnregistreEmplacementServeur(int idx)
{
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set idlieupardefaut = " + ui->EmplacementServeurupComboBox->itemData(idx).toString(),db);
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
        if (anc != db.password())
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
        QSqlQuery chgmdpadmquery(req,db);
        TraiteErreurRequete( chgmdpadmquery,req,"");
        // Enregitrer le nouveau MDP de la base
        req = "update " NOM_TABLE_UTILISATEURS " set userMDP = '" + nouv + "' where idUser = " + QString::number(idAdminDocs);
        QSqlQuery chgmdpquery(req,db);
        TraiteErreurRequete( chgmdpquery,req,"");
        // Enregitrer le nouveau MDP de connexion à MySQL
        req = "set password for '" NOM_ADMINISTRATEURDOCS "'@'localhost' = '" + nouv + "'";
        QSqlQuery chgmdpBasequery(req, db);
        TraiteErreurRequete(chgmdpBasequery,req, "");
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
        QSqlQuery chgmdpBaseReseauquery(req, db);
        TraiteErreurRequete(chgmdpBaseReseauquery,req, "");
        req = "set password for '" NOM_ADMINISTRATEURDOCS "SSL'@'%' = '" + nouv + "'";
        QSqlQuery chgmdpBaseDistantquery(req, db);
        TraiteErreurRequete(chgmdpBaseDistantquery,req, "");
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
    ui->ExportImagespushButton->setEnabled(QSqlQuery(totreq,db).size()>0);
}

void RufusAdmin::Slot_ExporteDocs()
{
    QString NomDirStockageImagerie  = gsettingsIni->value("DossierImagerie").toString();
    if (!QDir(NomDirStockageImagerie).exists() || NomDirStockageImagerie == "")
    {
        QString msg = tr("Le dossier de sauvegarde d'imagerie") + " <font color=\"red\"><b>" + NomDirStockageImagerie + "</b></font>" + tr(" n'existe pas");
        msg += "<br />" + tr("Renseignez un dossier valide dans") + " <font color=\"green\"><b>" + tr("Emplacement de stockage des documents archivés") + "</b></font>";
        Message(msg, 6000, false);
        return;
    }
    QString CheminEchecTransfrDir   = NomDirStockageImagerie + NOMDIR_ECHECSTRANSFERTS;
    QDir DirTrsferEchec;
    if (!QDir(CheminEchecTransfrDir).exists())
        if (!DirTrsferEchec.mkdir(CheminEchecTransfrDir))
        {
            QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminEchecTransfrDir + "</b></font>" + tr(" invalide");
            Message(msg, 3000, false);
            return;
        }

    QString totreq = "SELECT idimpression FROM " NOM_TABLE_IMPRESSIONS " where jpg is not null or pdf is not null";
    //qDebug() << totreq;
    QSqlQuery totquer(totreq,db);
    int total = totquer.size();
    if (total>100)
    {
        int min = total/180;
        int hour = min/60;
        min = min - (hour*60);
        QString tps = QTime(hour,min).toString("H'H'mm'mn'");
        if (UpMessageBox::Question(this, tr("Nombreux documents à convertir"),
                                   tr("Il y a ") + QString::number(total) + tr(" documents à convertir.") +"\n" +
                                   tr("Cette procédure devrait durer environ ") + tps + "\n" +
                                   tr("Voulez vous le faire maintenant?"))
                                   !=UpSmallButton::STARTBUTTON)
        {
            disconnect (gTimerDocsAExporter,    SIGNAL(timeout()),      this,   SLOT(Slot_CalcExporteDocs()));
            return;
        }
    }
    if (total==0)
        return;
    int faits = 0;
    QTime debut = QTime::currentTime();
    QString duree;


    DisconnectTimers();

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES JPG
    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString req = "SELECT idimpression, idpat, SousTypeDoc, Dateimpression, jpg, lienversfichier, typedoc FROM " NOM_TABLE_IMPRESSIONS " where jpg is not null";
    //qDebug() << req;
    QSqlQuery exportjpgquer (req,db);
    for (int i=0; i<exportjpgquer.size(); i++)
    {
        exportjpgquer.seek(i);
        if (exportjpgquer.value(5).toString() != "")
        {
            QString CheminFichier = NomDirStockageImagerie + NOMDIR_IMAGES + exportjpgquer.value(5).toString();
            if (QFile(CheminFichier).exists())
            {
                QSqlQuery ("update " NOM_TABLE_IMPRESSIONS " set jpg = null where idimpression = " + exportjpgquer.value(0).toString(),db);
                continue;
            }
        }
        QString datetransfer            = exportjpgquer.value(3).toDate().toString("yyyy-MM-dd");
        QString CheminOKTransfrDir      = NomDirStockageImagerie + NOMDIR_IMAGES;
        QDir DirTrsferOK;
        if (!QDir(CheminOKTransfrDir).exists())
            if (!DirTrsferOK.mkdir(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                Message(msg, 3000, false);
                ConnectTimers();
                return;
            }
        CheminOKTransfrDir      = CheminOKTransfrDir + "/" + datetransfer;
        if (!QDir(CheminOKTransfrDir).exists())
            if (!DirTrsferOK.mkdir(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                Message(msg, 3000, false);
                ConnectTimers();
                return;
            }
        QString NomFileDoc = exportjpgquer.value(1).toString() + "_" + exportjpgquer.value(6).toString() + "-"
                + exportjpgquer.value(2).toString() + "_"
                + exportjpgquer.value(3).toDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("HHmmss")
                + "-" + exportjpgquer.value(0).toString()  + ".jpg";
        QString CheminOKTransfrDoc      = CheminOKTransfrDir + "/" + NomFileDoc;
        QFile CC(CheminOKTransfrDoc);
        QImage  img;
        img.loadFromData(exportjpgquer.value(4).toByteArray());
        QPixmap pixmap;
        pixmap = pixmap.fromImage(img.scaledToWidth(2560,Qt::SmoothTransformation));
        if (!pixmap.save(CheminOKTransfrDoc, "jpeg"))
        {
            QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer + ".txt";
            QFile   echectrsfer(echectrsfername);
            if (echectrsfer.open(QIODevice::Append))
            {
                QTextStream out(&echectrsfer);
                out << NomFileDoc << "\n" ;
                echectrsfer.close();
                QFile CD(CheminEchecTransfrDir + "/" +NomFileDoc);
                if (CD.open(QIODevice::Append))
                {
                    QTextStream out(&CD);
                    out << exportjpgquer.value(4).toByteArray() ;
                }
            }
            QSqlQuery ("delete from  " NOM_TABLE_IMPRESSIONS " where idimpression = " + exportjpgquer.value(0).toString(),db);
            continue;
        }
        int     tauxcompress = 100;
        int sz = exportjpgquer.value(4).toByteArray().size();
        bool OK = true;
        while (sz > TAILLEMAXIIMAGES && tauxcompress > 10)
        {
            pixmap = pixmap.fromImage(img.scaledToWidth(2560,Qt::SmoothTransformation));
            if (CC.exists())
                CC.remove();
            if (!pixmap.save(CheminOKTransfrDoc, "jpeg",tauxcompress))
            {
                QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer + ".txt";
                QFile   echectrsfer(echectrsfername);
                if (echectrsfer.open(QIODevice::Append))
                {
                    QTextStream out(&echectrsfer);
                    out << NomFileDoc << "\n" ;
                    echectrsfer.close();
                    QFile CD(CheminEchecTransfrDir + "/" +NomFileDoc);
                    if (CD.open(QIODevice::Append))
                    {
                        QTextStream out(&CD);
                        out << exportjpgquer.value(4).toByteArray() ;
                    }
                }
                OK = false;
                break;
            }
            CC.open(QIODevice::ReadWrite);
            sz = CC.size();
            if (sz > TAILLEMAXIIMAGES)
                tauxcompress -= 10;
            CC.close();
        }
        if (!OK)
        {
            QSqlQuery ("delete from  " NOM_TABLE_IMPRESSIONS " where idimpression = " + exportjpgquer.value(0).toString(),db);
            continue;
        }
        CC.open(QIODevice::ReadWrite);
        CC.setPermissions(QFileDevice::ReadOther
                          | QFileDevice::ReadGroup
                          | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                          | QFileDevice::ReadUser   | QFileDevice::WriteUser);
        CC.close();
        QSqlQuery ("update " NOM_TABLE_IMPRESSIONS " set jpg = null, lienversfichier = '/" + datetransfer + "/" + NomFileDoc + "' where idimpression = " + exportjpgquer.value(0).toString(),db);
        faits ++;
        int nsec = debut.secsTo(QTime::currentTime());
        int min = nsec/60;
        int hour = min/60;
        min = min - (hour*60);
        nsec = nsec - (hour*3600) - (min*60);
        duree = QTime(hour,min,nsec).toString("HH:mm:ss");
        Message("JPG - " + NomFileDoc + " - " + QString::number(faits) + "/" + QString::number(total) + " - "  + duree, 10);
    }



    //-----------------------------------------------------------------------------------------------------------------------------------------
    //              LES PDF
    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString reqpdf = "SELECT idimpression, idpat, SousTypeDoc, Dateimpression, pdf, lienversfichier, compression, typedoc FROM " NOM_TABLE_IMPRESSIONS " where pdf is not null";
    QSqlQuery exportpdfquer (reqpdf,db);
    for (int i=0; i<exportpdfquer.size(); i++)
    {
        exportpdfquer.seek(i);
        if (exportpdfquer.value(5).toString() != "")
        {
            QString CheminFichier = NomDirStockageImagerie + NOMDIR_IMAGES + exportpdfquer.value(5).toString();
            if (QFile(CheminFichier).exists())
            {
                QSqlQuery ("update " NOM_TABLE_IMPRESSIONS " set pdf = null where idimpression = " + exportpdfquer.value(0).toString(),db);
                continue;
            }
        }
        QString datetransfer            = exportpdfquer.value(3).toDate().toString("yyyy-MM-dd");
        QString CheminOKTransfrDir      = NomDirStockageImagerie + NOMDIR_IMAGES;
        QDir DirTrsferOK;
        if (!QDir(CheminOKTransfrDir).exists())
            if (!DirTrsferOK.mkdir(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                Message(msg, 3000, false);
                ConnectTimers();
                return;
            }
        CheminOKTransfrDir      = CheminOKTransfrDir + "/" + datetransfer;
        if (!QDir(CheminOKTransfrDir).exists())
            if (!DirTrsferOK.mkdir(CheminOKTransfrDir))
            {
                QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
                Message(msg, 3000, false);
                ConnectTimers();
                return;
            }
        QString NomFileDoc = exportpdfquer.value(1).toString() + "_" + exportpdfquer.value(7).toString() + "-"
                + exportpdfquer.value(2).toString() + "_"
                + exportpdfquer.value(3).toDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("HHmmss")
                + "-" + exportpdfquer.value(0).toString()  + ".pdf";
        QString CheminOKTransfrDoc      = CheminOKTransfrDir + "/" + NomFileDoc;

        QByteArray bapdf;
        if (exportpdfquer.value(6).toString()=="1")
            bapdf.append(qUncompress(exportpdfquer.value(4).toByteArray()));
        else
            bapdf.append(exportpdfquer.value(4).toByteArray());

        Poppler::Document* document = Poppler::Document::loadFromData(bapdf);
        if (!document || document->isLocked() || document == Q_NULLPTR)
        {
            Message(tr("Impossible de charger le document ") + NomFileDoc, 3000, false);
            QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer + ".txt";
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
                    out << exportpdfquer.value(4).toByteArray() ;
                }
            }
            QString delreq = "delete from  " NOM_TABLE_IMPRESSIONS " where idimpression = " + exportpdfquer.value(0).toString();
            //qDebug() << delreq;
            QSqlQuery (delreq,db);
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
        QSqlQuery ("update " NOM_TABLE_IMPRESSIONS " set pdf = null, compression = null, lienversfichier = '/" + datetransfer + "/" + NomFileDoc  + "' where idimpression = " + exportpdfquer.value(0).toString(),db);
        faits ++;
        int nsec = debut.secsTo(QTime::currentTime());
        int min = nsec/60;
        int hour = min/60;
        min = min - (hour*60);
        nsec = nsec - (hour*3600) - (min*60);
        duree = QTime(hour,min,nsec).toString("HH:mm:ss");
        Message("PDF - " + NomFileDoc + " - " + QString::number(faits) + "/" + QString::number(total) + " - "  + duree, 10);
    }
    Message(tr("export terminé") + "\n" + QString::number(total) + " documents exportés en "  + duree, 3000);
    ConnectTimers();
}

void RufusAdmin::Slot_GestionBanques()
{
    DisconnectTimerInactive();
    Dlg_Banq = new dlg_banque(db, MapIcons(), this);
    Dlg_Banq->exec();
    ConnectTimerInactive();
}

void RufusAdmin::Slot_GestLieux()
{
    DisconnectTimerInactive();
    dlg_GestionLieux *gestLieux = new dlg_GestionLieux(db, this);
    gestLieux->exec();
    ConnectTimerInactive();
}

void RufusAdmin::Slot_GestUser()
{
    DisconnectTimerInactive();
    QString req = "select IdUser from " NOM_TABLE_UTILISATEURS " where userlogin <> '" NOM_ADMINISTRATEURDOCS "'";
    QSqlQuery listusrquery (req, db);
    if (listusrquery.size()>0)
        listusrquery.first();
    int iduser = listusrquery.value(0).toInt();
    Dlg_GestUsr = new dlg_gestionusers(iduser, ui->EmplacementServeurupComboBox->currentData().toInt(), db, MapIcons(), this);
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
    if (!verifdocs)
        return;
    bool acces = (gMode != Distant? true : false);
    ImportDocsExtThread = new ImportDocsExternesThread(db, idAdminDocs, idlieuExercice, acces);
}

void RufusAdmin::Slot_MasqueAppli()
{
    foreach (QDialog* d , findChildren<QMessageBox*>())
        d->hide();
    hide();
}

void RufusAdmin::Slot_MetAJourLaConnexion()
{
    QString lockrequete = "LOCK TABLES " NOM_TABLE_USERSCONNECTES "  WRITE, " NOM_TABLE_SALLEDATTENTE " WRITE, " NOM_TABLE_VERROUCOMPTAACTES " WRITE;";
    QSqlQuery lockquery (lockrequete, db);
    if (TraiteErreurRequete(lockquery,lockrequete,"Impossible de verrouiller " NOM_TABLE_USERSCONNECTES))
        return;

    QString MAJConnexionRequete;
    QSqlQuery usrquer("select iduser from " NOM_TABLE_USERSCONNECTES
                      " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                      " and idlieu = " + QString::number(idlieuExercice), db);
    if (usrquer.size()>0)
        MAJConnexionRequete = "UPDATE " NOM_TABLE_USERSCONNECTES " SET HeureDerniereConnexion = NOW(), "
                              " idUser = " + QString::number(idAdminDocs) +
                              " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                              " and idlieu = " + QString::number(idlieuExercice);
    else
       MAJConnexionRequete = "insert into " NOM_TABLE_USERSCONNECTES "(HeureDerniereConnexion, idUser, UserSuperviseur, UserComptable, UserParent, NomPosteConnecte, LastidModifSalDat, idlieu)"
                               " VALUES(NOW()," +
                               QString::number(idAdminDocs) + ", -1, -1, -1, '" +
                               QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "', 0, " +
                               QString::number(idlieuExercice) + ")";
    //qDebug() << MAJConnexionRequete;
    QSqlQuery MAJConnexionQuery (MAJConnexionRequete, db);
    TraiteErreurRequete(MAJConnexionQuery, MAJConnexionRequete,"");

    // Deconnecter les users dabranchés accidentellement
    QString VerifOldUserreq = "select idUser, NomPosteConnecte from  " NOM_TABLE_USERSCONNECTES " where time_to_sec(timediff(now(),heurederniereconnexion)) > 60";
    QSqlQuery verifoldquery (VerifOldUserreq,db);
    //qDebug() << VerifOldUserreq;
    TraiteErreurRequete(verifoldquery,VerifOldUserreq,"");

    if (verifoldquery.size() > 0)
    {
        verifoldquery.first();
        for (int i=0; i<verifoldquery.size();i++)
        {
            //on déverrouille les dossiers verrouillés par cet utilisateur et on les remet en salle d'attente
            QString blabla              = ENCOURSEXAMEN;
            int length                  = blabla.size();
            int a                       = verifoldquery.value(0).toInt();
            QString Poste               = verifoldquery.value(1).toString();
            QString LibereVerrouRequete;
            LibereVerrouRequete = "UPDATE " NOM_TABLE_SALLEDATTENTE " SET Statut = '" ARRIVE "', idUserEnCoursExam = null, PosteExamen = null"
                                  " WhERE idUserEnCoursExam = " + QString::number(a) +
                                  " AND PosteExamen = '" + Poste +
                                  "' AND Left(Statut," + QString::number(length) + ") = '" ENCOURSEXAMEN "'";
            QSqlQuery LibereVerrouRequeteQuery (LibereVerrouRequete,db);
            TraiteErreurRequete(LibereVerrouRequeteQuery,LibereVerrouRequete,"");
            //qDebug() << LibereVerrouRequete;
            //on déverrouille les actes verrouillés en comptabilité par cet utilisateur
            LibereVerrouRequete = "delete from " NOM_TABLE_VERROUCOMPTAACTES " where PosePar = " + QString::number(a);
            QSqlQuery LibereVerrouComptaQuery (LibereVerrouRequete,db);
            TraiteErreurRequete(LibereVerrouComptaQuery,LibereVerrouRequete,"");
            // on retire cet utilisateur de la table des utilisateurs connectés
            QSqlQuery ("delete from " NOM_TABLE_USERSCONNECTES " where NomPosteConnecte = '" + verifoldquery.value(1).toString() + "'", db);
            Message(tr("Le poste ") + Poste + tr(" a été retiré de la liste des postes connectés actuellement au serveur"),1000);
            verifoldquery.next();
        }
    }
    QSqlQuery("unlock tables",db);
}

void RufusAdmin::Slot_ModifMDP()
{
    disconnect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
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
    connect (gTimerInactive,            SIGNAL(timeout()),      this,   SLOT(Slot_MasqueAppli()));
}

void RufusAdmin::Slot_ParamMotifs()
{
    DisconnectTimerInactive();
    Dlg_motifs = new dlg_motifs(db, this);
    Dlg_motifs->setWindowTitle(tr("Motifs de consultations"));
    Dlg_motifs->exec();
    delete Dlg_motifs;
    ConnectTimerInactive();
}

void RufusAdmin::Slot_RestaureBase()
{
    QString req = "select NomPosteConnecte from " NOM_TABLE_USERSCONNECTES " where NomPosteConnecte <> '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS + "'";
    QSqlQuery postesquer(req,db);
    if (postesquer.size() > 0)
    {
        postesquer.first();
        UpMessageBox::Information(this, tr("Autres postes connectés!"),
                                     tr("Vous ne pouvez pas effectuer d'opération de sauvegarde/restauration sur la base de données"
                                     " si vous n'êtes pas le seul poste connecté.\n"
                                     "Le poste ") + postesquer.value(0).toString() + tr(" est aussi connecté"));
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
    QString dir = QDir::homePath() + NOMDIR_RUFUSADMIN;
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
    QSqlQuery dirquer("select dirimagerie from " NOM_TABLE_PARAMSYSTEME, db);
    dirquer.first();
    NomDirStockageImagerie = dirquer.value(0).toString();
    if (!QDir(NomDirStockageImagerie).exists())
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
        QSqlQuery (reqimg, db);
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
                    if (!VerifMDP(db.password(),tr("Saisissez le mot de passe Administrateur")))
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
                        QSqlQuery ("drop database if exists " NOM_BASE_COMPTA,      db);
                        QSqlQuery ("drop database if exists " NOM_BASE_OPHTA,       db);
                        QSqlQuery ("drop database if exists " NOM_BASE_CONSULTS,    db);
                        QSqlQuery ("drop database if exists " NOM_BASE_IMAGES,      db);
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
                            args << "-u " + db.userName();
                            args << "-p"  + db.password();
                            args << "-h " + db.hostName();
                            args << "-P " + QString::number(db.port());
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
                    QDir sauvRssces;
                    if (!sauvRssces.exists(QDir::homePath() + NOMDIR_RUFUS NOMDIR_RESSOURCES))
                        sauvRssces.mkdir(QDir::homePath() + NOMDIR_RUFUS NOMDIR_RESSOURCES);
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


void RufusAdmin::Slot_SupprimerDocs()
{
    QSqlQuery ("lock tables '" NOM_TABLE_DOCSASUPPRIMER "' write", db);
    QString req = "Select filepath from " NOM_TABLE_DOCSASUPPRIMER;
    QSqlQuery delreq (req, db);
    for (int i=0; i<delreq.size(); i++)
    {
        delreq.seek(i);
        QString CheminFichier = gsettingsIni->value("DossierImagerie").toString() + delreq.value(0).toString();
        if (!QFile(CheminFichier).remove())
            Message(tr("Fichier introuvable!") + " " + CheminFichier, 3000);
        QSqlQuery("delete from " NOM_TABLE_DOCSASUPPRIMER " where filepath = '" + delreq.value(0).toString() + "'", db);
    }
    QSqlQuery("unlock tables", db);
}

void RufusAdmin::Slot_TrayIconMenu()
{
    setFocus();
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
    if (txt == tr("Ouvir RufusAdmin"))
    {
        if (VerifMDP(db.password(),tr("Saisissez le mot de passe Administrateur")))
            showNormal();
    }
    else if (txt == tr("Quitter RufusAdmin"))
    {
        // on retire le poste de la variable posteimportdocs SQL
        setPosteImportDocs(false);
        // on retire Admin de la table des utilisateurs connectés
        QString req = "delete from " NOM_TABLE_USERSCONNECTES
                      " where NomPosteConnecte = '" + QHostInfo::localHostName().left(60) + " - " NOM_ADMINISTRATEURDOCS "'"
                      " and idlieu = " + QString::number(idlieuExercice);
        QSqlQuery qer(req,db);
        TraiteErreurRequete(qer,req,"");
        exit(0);
    }
}

void RufusAdmin::Slot_VerifPosteImport()
{
    if (gMode == Distant)
        return;
    //On recherche si le poste défini comme importateur des docs externes n'est pas celui sur lequel s'éxécute cette session de RufusAdmin et on prend sa place dans ce cas
    QString A, PostImport;    // l'importateur des docs externes
    QString req = "CALL " NOM_BASE_CONSULTS "." NOM_POSTEIMPORTDOCS;
    QSqlQuery quer(req, db);
    if (quer.size()==-1)
    {
        A = "";
        ui->PosteImportDocslabel->setText(tr("Pas de poste paramétré"));
        ui->PosteImportDocsPrioritairelabel->setText("");
    }
    else
    {
        quer.first();
        //qDebug() << "nbre reponses = " + QString::number(quer.size()) << NOM_POSTEIMPORTDOCS " = " + quer.value(0).toString();
        PostImport = quer.value(0).toString();
        A = quer.value(0).toString();
        A = "<font color=\"green\"><b>" + A.remove(".local") + "</b></font>";
        QString B;
        if (A.contains(" - " NOM_ADMINISTRATEURDOCS))
            B = tr("Administrateur");
        else
            B = (A.contains(" - prioritaire")? tr("prioritaire") : tr("non prioritaire"));
        B = "<b>" + B + "</b>";
        A.remove(" - prioritaire");
        A.remove(" - " NOM_ADMINISTRATEURDOCS);

        ui->PosteImportDocslabel->setText(A);
        if (B == tr("non prioritaire"))
            B = "<font color=\"red\">" + B + "</font>";
        else
            B = "<font color=\"green\">" + B + "</font>";
        ui->PosteImportDocsPrioritairelabel->setText(B);
    }
    QString IpAdr = QHostInfo::localHostName() + " - " NOM_ADMINISTRATEURDOCS;

    if (PostImport != "NULL" && PostImport != IpAdr && gMode != Distant)
        setPosteImportDocs();
}

void RufusAdmin::Slot_VerifVersionBase()
{
    QSqlQuery quer("select versionbase from " NOM_TABLE_PARAMSYSTEME, db);
    quer.first();
    int version = quer.value(0).toInt();
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
    /*-------------------- GESTION DES LIEUX D'EXRCICE-------------------------------------------------------*/
    ui->EmplacementServeurupComboBox->clear();
    QSqlQuery adrquer("select idLieu, NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, LieuCodePostal, LieuVille, LieuTelephone from " NOM_TABLE_LIEUXEXERCICE, db);
    for (int i=0; i< adrquer.size(); i++)
    {
        adrquer.seek(i);
        ui->EmplacementServeurupComboBox->addItem(adrquer.value(1).toString(),adrquer.value(0));
    }
    QSqlQuery DefautLieuquer("select idlieupardefaut from " NOM_TABLE_PARAMSYSTEME, db);
    DefautLieuquer.first();
    if (DefautLieuquer.value(0).toInt()>0)
        ui->EmplacementServeurupComboBox->setCurrentIndex(ui->EmplacementServeurupComboBox->findData(DefautLieuquer.value(0)));
    else
        ui->EmplacementServeurupComboBox->setCurrentIndex(0);
    /*-------------------- GESTION DES LIEUX D'EXRCICE-------------------------------------------------------*/
}

/*-----------------------------------------------------------------------------------------------------------------
    -- Traite et affiche le signal d'erreur d'une requete -------------------------------------------------------------
    -----------------------------------------------------------------------------------------------------------------*/
bool RufusAdmin::TraiteErreurRequete(QSqlQuery query, QString requete, QString ErrorMessage)
{
    if (query.lastError().type() != QSqlError::NoError)
    {
        UpMessageBox::Watch(this, ErrorMessage, tr("\nErreur\n") + query.lastError().text() +  tr("\nrequete = ") + requete);
        return true;
    }
    else return false;
}

bool RufusAdmin::VerifBase()
{
    int Versionencours  = 37; //correspond aux premières versions de MAJ de la base
    int Version         = VERSION_BASE;
    QString req         = "select VersionBase from " NOM_TABLE_PARAMSYSTEME;
    QSqlQuery MAJBaseQuery(req,db);
    bool b              = false;
    if (MAJBaseQuery.lastError().type() != QSqlError::NoError || MAJBaseQuery.size()==0)
        b = true;
    else
    {
        MAJBaseQuery.first();
        Versionencours = MAJBaseQuery.value(0).toInt();
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
            QSqlQuery query(db);
            bool a = true;
            foreach(const QString &s, listinstruct)
            {
                //Edit(s);
                query.exec(s);
                if (TraiteErreurRequete(query, s, ""))
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
            if (Version == 40)
            {
                QSqlQuery mdpquer("select mdpadmin from " NOM_TABLE_PARAMSYSTEME,db);
                mdpquer.first();
                QString mdp = (mdpquer.value(0).toString() != ""? mdpquer.value(0).toString() : db.password());
                QSqlQuery ("create user if not exists '" NOM_ADMINISTRATEURDOCS "SSL'@'%' identified by '" + mdp + "' REQUIRE SSL",db);
                QSqlQuery ("grant all on *.* to '" NOM_ADMINISTRATEURDOCS "SSL'@'%' identified by '" + mdp + "' with grant option",db);
            }
        }
    }
    return true;
}

/*---------------------------------------------------------------------------------------------------------------------
    -- VÉRIFICATION DE MDP --------------------------------------------------------------------------------------------
    -----------------------------------------------------------------------------------------------------------------*/
bool RufusAdmin::VerifMDP(QString MDP, QString Msg)
{
    setFocus();
    QInputDialog *quest = new QInputDialog();
    quest->setCancelButtonText("Annuler");
    quest->setLabelText(Msg);
    quest->setInputMode(QInputDialog::TextInput);
    quest->setTextEchoMode(QLineEdit::Password);
    QList<QLineEdit*> list = quest->findChildren<QLineEdit*>();
    for (int i=0;i<list.size();i++)
        list.at(0)->setAlignment(Qt::AlignCenter);
    QList<QLabel*> listlab = quest->findChildren<QLabel*>();
    for (int i=0;i<listlab.size();i++)
        listlab.at(0)->setAlignment(Qt::AlignCenter);
    quest->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    if (quest->exec() > 0)
    {
        if (quest->textValue() == MDP)
            return true;
        else
            UpMessageBox::Watch(this,tr("Mot de passe invalide!"));
    }
    delete quest;
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
        QString req = "update " NOM_TABLE_PARAMSYSTEME " set DirBkup = '" + dirSauv + "'";
        QSqlQuery (req, db);
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
    QTimer timer;
    timer.setSingleShot(true);
    connect (&timer, &QTimer::timeout, [=] {ui->MessageupLabel->setText("");});
    timer.start(pause);
    QStringList listmsg;
    listmsg << mess;
    dlg_message(listmsg, pause, bottom);
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
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set DirBkup = '" + dirSauv + "'", db);
    // ENREGISTREMENT DES PARAMETRES DE SAUVEGARDE DANS /Documents/Rufus/RufusScriptBackup.sh
    QSqlQuery dirquer("select dirimagerie from " NOM_TABLE_PARAMSYSTEME, db);
    dirquer.first();
    QString NomDirStockageImagerie = dirquer.value(0).toString();

    //1. Dossier de backup
    DefinitScriptBackup(NomDirStockageImagerie);

    //2. Heure et date du backup
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set HeureBkup = '"    + ui->HeureBackuptimeEdit->time().toString("HH:mm") + "'", db);

    QString LundiBkup       = (ui->LundiradioButton->isChecked()?   "1" : "NULL");
    QString MardiBkup       = (ui->MardiradioButton->isChecked()?   "1" : "NULL");
    QString MercrediBkup    = (ui->MercrediradioButton->isChecked()?"1" : "NULL");
    QString JeudiBkup       = (ui->JeudiradioButton->isChecked()?   "1" : "NULL");
    QString VendrediBkup    = (ui->VendrediradioButton->isChecked()?"1" : "NULL");
    QString SamediBkup      = (ui->SamediradioButton->isChecked()?  "1" : "NULL");
    QString DimancheBkup    = (ui->DimancheradioButton->isChecked()?"1" : "NULL");

    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set LundiBkup = "     + LundiBkup, db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set MardiBkup = "     + MardiBkup, db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set MercrediBkup = "  + MercrediBkup, db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set JeudiBkup = "     + JeudiBkup, db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set VendrediBkup = "  + VendrediBkup, db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set SamediBkup = "    + SamediBkup, db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set DimancheBkup = "  + DimancheBkup, db);

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
    QSqlQuery ("Use Rufus", db);
    QSqlQuery ("DROP EVENT IF EXISTS VideImagesEchange", db);
    QString req =   "CREATE EVENT VideImagesEchange "
            "ON SCHEDULE EVERY 1 DAY STARTS '2018-03-23 " + ui->HeureBackuptimeEdit->time().addSecs(-60).toString("HH:mm:ss") + "' "
            "DO DELETE FROM Images.ImagesEchange";
    QSqlQuery (req,db);
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
    scriptbackup += "DIR_VIDEOS=\"" + path + NOMDIR_VIDEOS + "\"";
    //# Rufus.ini
    scriptbackup += "\n";
    scriptbackup += "RUFUSINI=\"" + QDir::homePath() + NOMFIC_INIRUFUS + "\"";
    //# Identifiants MySQL
    scriptbackup += "\n";
    scriptbackup += "MYSQL_USER=\"dumprufus\"";
    scriptbackup += "\n";
    scriptbackup += "MYSQL_PASSWORD=\"" + db.password() + "\"";
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
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set HeureBkup = ''", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set DirBkup = ''", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set LundiBkup = NULL", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set MardiBkup = NULL", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set MercrediBkup = NULL", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set JeudiBkup = NULL", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set VendrediBkup = NULL", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set SamediBkup = NULL", db);
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set DimancheBkup = NULL", db);
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
    QSqlQuery postesquer(req,db);
    if (postesquer.size() > 0)
    {
        postesquer.first();
        UpMessageBox::Information(Q_NULLPTR, tr("Autres postes connectés!"),
                                     tr("Vous ne pouvez pas effectuer d'opération de sauvegarde/restauration sur la base de données"
                                     " si vous n'êtes pas le seul poste connecté.\n"
                                     "Le poste ") + postesquer.value(0).toString() + tr(" est aussi connecté"));
        show();
        return false;
    }

    QSqlQuery dirquer("select dirimagerie, DirBkup from " NOM_TABLE_PARAMSYSTEME, db);
    dirquer.first();
    QString NomDirStockageImagerie = dirquer.value(0).toString();
    QString NomDirDestination = dirquer.value(1).toString();
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
        {
            QDir dirdest;
            dirdest.mkdir(dest);
        }
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



void RufusAdmin::TraiteTCPMessage(QString msg)
{
    //qDebug() << msg + " - sur RufusAdmin::traitetcpmessage()";
    if (msg.contains(TCPMSG_ListeSockets))
    {
        msg.remove("{}" TCPMSG_ListeSockets);
        gListSockets.clear();
        gListSockets = msg.split("{}");
        qDebug() << "liste des clients connectés RufusAdmin.cpp - " + QTime::currentTime().toString("hh-mm-ss");
        for (int i=0; i<gListSockets.size(); i++)
        {
            QString data = gListSockets.at(i);
            data.replace(TCPMSG_Separator, " - ");
            qDebug() << data;
        }
    }
    else if (msg.contains(TCPMSG_NouvelleConnexion))
    {
        msg.remove(TCPMSG_NouvelleConnexion);
        QString login = Datas::I()->users->getLoginById(msg.split(TCPMSG_Separator).at(0).toInt());
        QString adress = msg.split(TCPMSG_Separator).at(3);
        dlg_message(QStringList() << login + " " +  tr("vient de se connecter sur") + " " + adress, 3000);
    }
    else if (msg.contains(TCPMSG_Deconnexion))
    {
        msg.remove(TCPMSG_Deconnexion);
        QString login = Datas::I()->users->getLoginById(msg.split(TCPMSG_Separator).at(0).toInt());
        QString adress = msg.split(TCPMSG_Separator).at(1);
        dlg_message(QStringList() << login + " " +  tr("vient de se déconnecter sur") + " " + adress, 3000);
    }
}

void RufusAdmin::VerifServeur()
{
    /* Appelé par:
        * le serveur sytématiquement toutes les 30 secondes pour vérifier qu'il n'a pas été remplacé
        * le client qui n'est plus en contact avec le serveur
        * une erreur sur une transmission

     * On verrouille la table parce qu'en cas de défaillance brutale du serveur, ce sera un paquet de postes qui vont vouloir la modifier
     * et se déclarer comme serveur en même temps ce qui mettrait un brave foutoir. Il faut donc qu'ils fassent cette vérification les uns après les autres.
     * Il y a peut-être un système plus simple et plus léger mais je ne sais pas comment faire
    */
    qDebug() << "VerifServeur()";
    QSqlQuery lockquery("SET AUTOCOMMIT = 0;", db);
    QSqlQuery ("LOCK TABLES " NOM_TABLE_PARAMSYSTEME "WRITE; LOCK TABLES " NOM_TABLE_PARAMSYSTEME "READ; select AdresseTCPServeur from " NOM_TABLE_PARAMSYSTEME ";", db);
    QSqlQuery ("LOCK TABLES " NOM_TABLE_PARAMSYSTEME "WRITE; LOCK TABLES " NOM_TABLE_PARAMSYSTEME "READ; select AdresseTCPServeur from " NOM_TABLE_PARAMSYSTEME ";", db);
    QString req = "select AdresseTCPServeur from " NOM_TABLE_PARAMSYSTEME ";";
    QSqlQuery TCPServerquer(req, db);
    TCPServerquer.first();

    // le serveur enregistré a changé
    if (gIPadr != TCPServerquer.value(0).toString())
            InitTcpServer();
    QSqlQuery ("COMMIT;", db );
    QSqlQuery ("UNLOCK TABLES;", db );
    QSqlQuery ("SET AUTOCOMMIT = 1;", db );
}

void RufusAdmin::InitTcpServer()
{
    TcpServer   ->start();
    QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set AdresseTCPServeur = '" + gIPadr + "'", db);
}

void RufusAdmin::InitTCP()
{
    QString delaitest       = TCPDelai_TestServer;
    QString delaitestskt    = TCPDelai_TestSocket;
    gTimerVerifServeur      ->setInterval(delaitest.toInt());
    gTimerVerifServeur      ->start();
    InitTcpServer();
    gflagMG                 = GetflagMG();
    gflagSalDat             = GetflagSalDat();
}

/*--------------------------------------------------------------------------------------------------------------------------------------
    -- Gestion du flag de mise à jour de l'affichage du médecin traitant -----------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------------------------------------*/
int RufusAdmin::GetflagMG()
{
    int flagMG = 0;
    QSqlQuery quer("select MAJflagMG from " NOM_TABLE_FLAGS, db);
    if (quer.size() > 0)
    {
        quer.first();
        flagMG = quer.value(0).toInt();
    }
    return flagMG;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
    -- Gestion du flag de mise à jour de l'affichage du médecin traitant -----------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------------------------------------*/
int RufusAdmin::GetflagSalDat()
{
    int flagSalDat = 0;
    QSqlQuery quer("select MAJflagSalDat from " NOM_TABLE_FLAGS, db);
    if (quer.size() > 0)
    {
        quer.first();
        flagSalDat = quer.value(0).toInt();
    }
    return flagSalDat;
}

void RufusAdmin::FermeTCP()
{
        TcpServer->close();
        QSqlQuery ("update " NOM_TABLE_PARAMSYSTEME " set AdresseTCPServeur = null", db);
}

void RufusAdmin::VerifVerrouDossier()
{
    /* Cette fonction sert à déconnecter et lever les verrous d'un utilisateur qui se serait déconnecté accidentellement
     *
     on fait la liste des utilisateurs qui n'ont pas remis à jour leur connexion depuis plus de 60 secondes,
     on retire les verrous qu'ils auraient pu poser et on les déconnecte*/
    bool mettreajourlasalledattente = false;
    QString VerifOldUserreq = "select idUser, NomPosteConnecte from  " NOM_TABLE_USERSCONNECTES " where time_to_sec(timediff(now(),heurederniereconnexion)) > 60";
    QSqlQuery verifoldquery (VerifOldUserreq, DataBase::getInstance()->getDataBase() );
    //qDebug() << VerifOldUserreq;
    DataBase::getInstance()->traiteErreurRequete(verifoldquery,VerifOldUserreq,"");

    if (verifoldquery.size() > 0)
    {
        verifoldquery.first();
        for (int i=0; i<verifoldquery.size();i++)
        {
            //on déverrouille les dossiers verrouillés par cet utilisateur et on les remet en salle d'attente
            QString blabla              = ENCOURSEXAMEN;
            int length                  = blabla.size();
            int a                       = verifoldquery.value(0).toInt();
            QString Poste               = verifoldquery.value(1).toString();
            QString LibereVerrouRequete;
            LibereVerrouRequete = "UPDATE " NOM_TABLE_SALLEDATTENTE " SET Statut = '" ARRIVE "', idUserEnCoursExam = null, PosteExamen = null"
                                  " WhERE idUserEnCoursExam = " + QString::number(a) +
                                  " AND PosteExamen = '" + Poste +
                                  "' AND Left(Statut," + QString::number(length) + ") = '" ENCOURSEXAMEN "'";
            QSqlQuery LibereVerrouRequeteQuery (LibereVerrouRequete, DataBase::getInstance()->getDataBase() );
            DataBase::getInstance()->traiteErreurRequete(LibereVerrouRequeteQuery,LibereVerrouRequete,"");
            //qDebug() << LibereVerrouRequete;
            //on déverrouille les actes verrouillés en comptabilité par cet utilisateur
            LibereVerrouRequete = "delete from " NOM_TABLE_VERROUCOMPTAACTES " where PosePar = " + QString::number(a);
            QSqlQuery LibereVerrouComptaQuery (LibereVerrouRequete, DataBase::getInstance()->getDataBase() );
            DataBase::getInstance()->traiteErreurRequete(LibereVerrouComptaQuery,LibereVerrouRequete,"");
            // on retire cet utilisateur de la table des utilisateurs connectés
            QString req = "delete from " NOM_TABLE_USERSCONNECTES " where NomPosteConnecte = '" + Poste + "'";
            QSqlQuery(req, DataBase::getInstance()->getDataBase() );
            mettreajourlasalledattente = true;
            Message(tr("Le poste ") + Poste + tr(" a été retiré de la liste des postes connectés actuellement au serveur"),1000);
            verifoldquery.next();
        }
    }

    // on donne le statut "arrivé" aux patients en salle d'attente dont le iduserencourssexam n'est plus present sur ce poste examen dans la liste des users connectes
    QString req = "select iduserencoursexam, posteexamen, idpat from " NOM_TABLE_SALLEDATTENTE " where statut like '" ENCOURSEXAMEN "%'";
    //qDebug() << req;
    QSqlQuery querr(req, DataBase::getInstance()->getDataBase() );
    for (int i=0; i<querr.size(); i++)
    {
        querr.seek(i);
        req = "select iduser, nomposteconnecte from " NOM_TABLE_USERSCONNECTES " where iduser = " + querr.value(0).toString()  + " and nomposteconnecte = '" + querr.value(1).toString() + "'";
        //qDebug() << req;
        QSqlQuery squer(req, DataBase::getInstance()->getDataBase() );
        if (squer.size()==0)
        {
            req = "update " NOM_TABLE_SALLEDATTENTE " set Statut = '" ARRIVE "', posteexamen = null, iduserencoursexam = null where idpat = " + querr.value(2).toString();
            //qDebug() << req;
            QSqlQuery(req,  DataBase::getInstance()->getDataBase() );
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
    /* envoi du message de MAJ de la salle d'attente au serveur */
    TcpServer->envoyerATous(TCPMSG_MAJSalAttente);;

    /* mise à jour du flag pour les utilisateurs distants qui le surveillent et mettent ainsi à jour leur salle d'attente */
    if (!DataBase::getInstance()->locktables(QStringList(NOM_TABLE_FLAGS)))
       return;
    QSqlQuery quer("select MAJflagSalDat from " NOM_TABLE_FLAGS, DataBase::getInstance()->getDataBase());
    QString MAJreq = "insert into " NOM_TABLE_FLAGS " (MAJflagSalDat) VALUES (1)";
    int a = 0;
    if (quer.seek(0)) {
        a = quer.value(0).toInt() + 1;
        MAJreq = "update " NOM_TABLE_FLAGS " set MAJflagSalDat = " + QString::number(a);
    }
    QSqlQuery (MAJreq, DataBase::getInstance()->getDataBase());
    DataBase::getInstance()->commit();
}

void RufusAdmin::VerifSalleDAttenteEtCorrespondants()
{
    if (gflagSalDat < GetflagSalDat())
    {
        gflagSalDat = GetflagSalDat();
        TcpServer->envoyerATous(TCPMSG_MAJSalAttente);
    }
    if (gflagMG < GetflagMG())
    {
        gflagMG = GetflagMG();
        TcpServer->envoyerATous(TCPMSG_MAJCorrespondants);
    }
}


