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

#include "dlg_paramconnexion.h"
#include "ui_dlg_paramconnexion.h"

dlg_paramconnexion::dlg_paramconnexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_paramconnexion)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QStringList ports;
    ports << "3306" << "3307";
    ui->PortcomboBox        ->addItems(ports);
    ui->AccesgroupBox       ->setFocusProxy(ui->PosteradioButton);
    ui->OKuppushButton      ->setShortcut(QKeySequence("Meta+Return"));
    ui->PosteradioButton    ->setChecked(true);
    gIPAvecZero             = "";
    gIPSansZero             = "";
    gClient                 = "";
    gServeur                = "";

    ui->AnnuluppushButton   ->setShortcut(QKeySequence("F12"));
    ui->OKuppushButton      ->setShortcut(QKeySequence("Meta+Return"));

    connect(ui->AnnuluppushButton,      SIGNAL(clicked(bool)),          this,   SLOT(reject()));
    connect(ui->OKuppushButton,         SIGNAL(clicked(bool)),          this,   SLOT(Slot_Verif()));
    connect(ui->TestuppushButton,       SIGNAL(clicked(bool)),          this,   SLOT(Slot_Test()));
    connect(ui->DistantradioButton,     SIGNAL(clicked(bool)),          this,   SLOT(Slot_RegleAffichage()));
    connect(ui->LocalradioButton,       SIGNAL(clicked(bool)),          this,   SLOT(Slot_RegleAffichage()));
    connect(ui->PosteradioButton,       SIGNAL(clicked(bool)),          this,   SLOT(Slot_RegleAffichage()));
    connect(ui->IPlineEdit,             SIGNAL(editingFinished()),      this,   SLOT(Slot_MAJIP()));

    ui->IPlabel->setText(tr("adresse IP"));
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             AdressIP = address.toString();
    }
    QStringList listIP = AdressIP.split(".");
    for (int i=0;i<listIP.size()-1;i++)
    {
        if (listIP.at(i).toInt()<100)
            Domaine += "0";
        if (listIP.at(i).toInt()<10)
            Domaine += "0";
        Domaine += listIP.at(i) + ".";
    }
    ui->IPlabel->setText(tr("Adresse IP") + " -> " + Domaine);
    ui->IPlineEdit->setText("000");
    ui->IPlineEdit->setInputMask("000");
    Slot_RegleAffichage();
}

dlg_paramconnexion::~dlg_paramconnexion()
{
    delete ui;
}

void dlg_paramconnexion::Slot_MAJIP()
{
    AdresseValide   = true;
    if (ui->PosteradioButton->isChecked())
    {
        gClient     = "localhost";
        gServeur    = "localhost";
        gMode       = Poste;
    }
    if (ui->LocalradioButton->isChecked())
    {
        if (ui->IPlineEdit->text().toInt()>255)
        {
            AdresseValide = false;
            UpMessageBox::Watch(this,tr("Adresse invalide"), tr("L'adresse spécifiée pour le serveur est invalide"));
            ui->IPlineEdit->setFocus();
            return;
        }
        gIPAvecZero         = "";
        gIPSansZero         = "";
        gClient             = "";
        gServeur            = "";
        gMode               = ReseauLocal;
        gIPSansZero += QString::number(ui->IPlineEdit->text().toInt());
        gIPAvecZero = gIPSansZero;
        if (ui->IPlineEdit->text().toInt()<100)
            gIPAvecZero = "0" + gIPSansZero;
        if (ui->IPlineEdit->text().toInt()<10)
            gIPAvecZero = "0" + gIPAvecZero;
        QStringList listIP = AdressIP.split(".");
        for (int i=0;i<listIP.size()-1;i++)
            gClient += QString::number(listIP.at(i).toInt()) + ".";
        gClient += "%";
        gServeur = Domaine + gIPAvecZero;
    }
    if (ui->DistantradioButton->isChecked())
    {
        gServeur    = ui->DistantlineEdit->text();
        gClient     = "%";
        gMode       = Distant;
    }
}

void dlg_paramconnexion::Slot_RegleAffichage()
{
    ui->IPlabel         ->setEnabled(ui->LocalradioButton->isChecked());
    ui->IPlineEdit      ->setEnabled(ui->LocalradioButton->isChecked());
    ui->DistantLabel    ->setEnabled(ui->DistantradioButton->isChecked());
    ui->DistantlineEdit ->setEnabled(ui->DistantradioButton->isChecked());
    if (sender() == ui->LocalradioButton)
        ui->IPlineEdit->setFocus();
    if (sender() == ui->DistantradioButton)
        ui->DistantlineEdit->setFocus();
}

void dlg_paramconnexion::Slot_Test()
{
    if (TestConnexion())
        UpMessageBox::Watch(this,tr("Paramètres OK!"));
}

void dlg_paramconnexion::Slot_Verif()
{
    if (TestConnexion())
        accept();
}

QSqlDatabase dlg_paramconnexion::getdatabase()
{
    return db;
}

bool dlg_paramconnexion::TestConnexion()
{
    if (!VerifFiche())
        return false;
    else
    {
        Slot_MAJIP();
        if(!AdresseValide)
            return false;
        QString Login = NOM_ADMINISTRATEURDOCS;
        if (ui->DistantradioButton->isChecked())
            Login += "SSL";
        db = QSqlDatabase::addDatabase("QMYSQL","Rufus");
        db.setHostName(gServeur);
        db.setUserName(Login);
        db.setPassword(ui->MDPlineEdit->text());
        db.setPort(ui->PortcomboBox->currentText().toInt());
        QString  ConnectOptions = (ui->DistantradioButton->isChecked()?
                                  "SSL_KEY=/etc/mysql/client-key.pem;"
                                  "SSL_CERT=/etc/mysql/client-cert.pem;"
                                  "SSL_CA=/etc/mysql/ca-cert.pem;"
                                  "MYSQL_OPT_RECONNECT=1"
                                     :
                                  "MYSQL_OPT_RECONNECT=1");
        db.setConnectOptions(ConnectOptions);


        if (!db.open())
        {
            UpMessageBox::Watch(this,tr("Paramètres non reconnus!") + "<br />"
                                + tr("Serveur") + "\n\t-> " + gServeur + "<br />"
                                + tr("Login") + "\n\t-> " NOM_ADMINISTRATEURDOCS "<br />"
                                + tr("MDP") + "\n\t-> " + ui->MDPlineEdit->text() + "<br />"
                                + tr("Port") + "\n\t-> " + ui->PortcomboBox->currentText() + "<br />"
                                + tr("Connexion impossible.") + "<br />"
                                + db.lastError().text());

            return false;
        }
        QSqlQuery ("set global sql_mode = 'NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES';", db);
        QSqlQuery ("SET GLOBAL event_scheduler = 1 ;", db);
        QSqlQuery ("SET GLOBAL max_allowed_packet=" MAX_ALLOWED_PACKET "*1024*1024 ;", db);

        QString req = "show grants for '" + Login + "'@'" + gClient + "'";
        QSqlQuery grantsquery(req, db);
        if (grantsquery.size()==0)
        {
            UpMessageBox::Watch(this,tr("Erreur sur le serveur"),
                                tr("Impossible de retrouver les droits de l'utilisateur ") + NOM_ADMINISTRATEURDOCS);
            return false;
        }
        grantsquery.first();
        QString reponse = grantsquery.value(0).toString();
        if (reponse.left(9) != "GRANT ALL")
        {
            UpMessageBox::Watch(this,tr("Erreur sur le serveur"),
                                tr("L'utilisateur ") + NOM_ADMINISTRATEURDOCS + tr(" existe mais ne dispose pas "
                                 "de toutes les autorisations pour modifier ou créer des données sur le serveur.\n"
                                 "Choisissez un autre utilisateur ou modifiez les droits de cet utilisateur au niveau du serveur.\n"));
            return false;
        }
    }
    return true;
}

bool dlg_paramconnexion::VerifFiche()
{
    if (ui->MDPlineEdit->text() == "")
    {
        UpMessageBox::Watch(this,tr( "Vous n'avez pas précisé le mot de passe."));
        ui->MDPlineEdit->setFocus();
        return false;
    }
    QList <QRadioButton*> listboutons = ui->AccesgroupBox->findChildren<QRadioButton*>();
    bool a = false;
    for (int i=0; i<listboutons.size(); i++)
    {
        if (listboutons.at(i)->isChecked())
        {
            a = true;
            break;
        }
    }
    if (!a)
    {
        UpMessageBox::Watch(this, tr("Vous n'avez pas précisé le mode d'accès."));
        return false;
    }
    if (ui->LocalradioButton->isChecked() && ui->IPlineEdit->text() == "")
    {
        UpMessageBox::Watch(this, tr("Vous n'avez pas précisé l'adresse du serveur."));
        ui->IPlineEdit->setFocus();
        return false;
    }
    return true;
}
