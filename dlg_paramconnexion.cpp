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

#include "dlg_paramconnexion.h"
#include "ui_dlg_paramconnexion.h"

dlg_paramconnexion::dlg_paramconnexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_paramconnexion)
{
    ui->setupUi(this);
    db = DataBase::getInstance();
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QStringList ports;
    ports << "3306" << "3307";
    ui->PortcomboBox        ->addItems(ports);
    ui->OKuppushButton      ->setShortcut(QKeySequence("Meta+Return"));

    ui->AnnuluppushButton   ->setShortcut(QKeySequence("F12"));
    ui->OKuppushButton      ->setShortcut(QKeySequence("Meta+Return"));

    connect(ui->AnnuluppushButton,      SIGNAL(clicked(bool)),          this,   SLOT(reject()));
    connect(ui->OKuppushButton,         SIGNAL(clicked(bool)),          this,   SLOT(Slot_Verif()));
    connect(ui->TestuppushButton,       SIGNAL(clicked(bool)),          this,   SLOT(Slot_Test()));
}

dlg_paramconnexion::~dlg_paramconnexion()
{
    delete ui;
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
    return DataBase::getInstance()->getDataBase();
}

bool dlg_paramconnexion::TestConnexion()
{
    //TODO : SQL Mettre en place un compte generique pour l'accès à la base de données.

    if (!VerifFiche())
        return false;
    QString error = "";
    QString Base, server;

    QString Login = NOM_ADMINISTRATEURDOCS;
    QString Password = ui->MDPlineEdit->text();

    if ( Password.isEmpty() ) {UpMessageBox::Watch(this,tr("Vous n'avez pas précisé votre mot de passe!"));   ui->MDPlineEdit->setFocus();    return 0;}
    //à mettre avant le connectToDataBase() sinon une restaurationp plante parce qu'elle n'a pas les renseignements
    db->initFromFirstConnexion("BDD_POSTE", "localhost", ui->PortcomboBox->currentText().toInt(), false);
    error = db->connectToDataBase(NOM_BASE_CONSULTS, Login, Password);

    if( error.size() )
    {
        UpMessageBox::Watch(this, tr("Erreur sur le serveur MySQL"),
                            tr("Impossible de se connecter au serveur avec le login ") + Login
                            + tr(" et ce mot de passe") + "\n"
                            + tr("Revoyez le réglage des paramètres de connexion dans le fichier rufus.ini.") + "\n"
                            + error);
        return false;
    }

    QString Client;
    if (db->getBase() == "BDD_DISTANT")
        Client = "%";
    else if (db->getBase() == "BDD_LOCAL" && Utils::rgx_IPV4.exactMatch(db->getServer()))
    {
        QStringList listIP = db->getServer().split(".");
        for (int i=0;i<listIP.size()-1;i++)
        {
            Client += QString::number(listIP.at(i).toInt()) + ".";
            if (i==listIP.size()-2)
                Client += "%";
        }
    }
    else
        Client = db->getServer();

    db->StandardSQL("set global sql_mode = 'NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES';");
    db->StandardSQL("SET GLOBAL event_scheduler = 1 ;");
    db->StandardSQL("SET GLOBAL max_allowed_packet=" MAX_ALLOWED_PACKET "*1024*1024 ;");

    QString req = "show grants for '" + Login + (db->getBase() == "BDD_DISTANT"? "SSL" : "")  + "'@'" + Client + "'";
    bool ok;
    QList<QVariant> grantsdata = db->getFirstRecordFromStandardSelectSQL(req,ok);

    if (!ok || grantsdata.size()==0)
    {
        UpMessageBox::Watch(this,tr("Erreur sur le serveur"),
                            tr("Impossible de retrouver les droits de l'utilisateur ") + NOM_ADMINISTRATEURDOCS);
        return false;
    }
    QString reponse = grantsdata.at(0).toString();
    if (reponse.left(9) != "GRANT ALL")
    {
        UpMessageBox::Watch(this,tr("Erreur sur le serveur"),
                            tr("L'utilisateur ") + NOM_ADMINISTRATEURDOCS + tr(" existe mais ne dispose pas "
                                                                               "de toutes les autorisations pour modifier ou créer des données sur le serveur.\n"
                                                                               "Choisissez un autre utilisateur ou modifiez les droits de cet utilisateur au niveau du serveur.\n"));
        return false;
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
    return true;
}
