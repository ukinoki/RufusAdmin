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

#include "dlg_gestioncomptes.h"
#include "ui_dlg_gestioncomptes.h"
#include "icons.h"


dlg_gestioncomptes::dlg_gestioncomptes(User *user, QWidget *parent) : UpDialog(parent), ui(new Ui::dlg_gestioncomptes)
{
    ui->setupUi(this);
    db                      = DataBase::I();
    m_userencours           = user;

    m_idcomptepardefaut      = m_userencours->idcomptepardefaut();

    m_listescomptesusr      = m_userencours->listecomptesbancaires();
    m_compteencours         = Datas::I()->comptes->getById(m_userencours->idcomptepardefaut());

    m_visible                = true;
    t_timer                  = new QTimer(this);
    t_timer                  ->start(500);
    connect(t_timer, &QTimer::timeout, this, &dlg_gestioncomptes::Clign);
    ui->CompteFacticePushButton->setVisible(false);

    setAttribute(Qt::WA_DeleteOnClose);

    wdg_buttonframe             = new WidgetButtonFrame(ui->ComptesuptableWidget);
    wdg_buttonframe             ->AddButtons(WidgetButtonFrame::Plus | WidgetButtonFrame::Modifier | WidgetButtonFrame::Moins);
    wdg_NouvBanqupPushButton    = new UpSmallButton();
    wdg_NouvBanqupPushButton    ->setText(tr("Gestion des organismes bancaires"));
    wdg_NouvBanqupPushButton    ->setIcon(Icons::icEuroCount());
    wdg_NouvBanqupPushButton    ->setIconSize(QSize(25,25));
    AjouteWidgetLayButtons(wdg_NouvBanqupPushButton);
    AjouteLayButtons(UpDialog::ButtonClose);
    CloseButton             ->setText(tr("Fermer"));

    connect(CloseButton,                    &QPushButton::clicked,      this, &dlg_gestioncomptes::Fermer);
    connect(ui->OKModifupSmallButton,       &QPushButton::clicked,      this, &dlg_gestioncomptes::ValidCompte);
    connect(ui->AnnulModifupSmallButton,    &QPushButton::clicked,      this, &dlg_gestioncomptes::AnnulModif);
    connect(wdg_NouvBanqupPushButton,       &QPushButton::clicked,      this, &dlg_gestioncomptes::Banques);
    connect(wdg_buttonframe,                &WidgetButtonFrame::choix,  this, &dlg_gestioncomptes::ChoixButtonFrame);
    connect(ui->CompteFacticePushButton,    &QPushButton::clicked,      this, &dlg_gestioncomptes::CompteFactice);
    connect(ui->DesactiveComptecheckBox,    &QPushButton::clicked,      this, &dlg_gestioncomptes::DesactiveCompte);

    QDoubleValidator *val = new QDoubleValidator(this);
    val->setDecimals(2);

    m_userlogin          = m_userencours->login();
    setWindowTitle(tr("Comptes bancaires de ") + m_userlogin);

    MetAJourListeBanques();

    RemplirTableView(m_idcomptepardefaut);
    ui->Compteframe             ->setEnabled(false);
    ui->OKModifupSmallButton    ->setVisible(false);
    ui->AnnulModifupSmallButton ->setVisible(false);
    ui->idCompteupLineEdit      ->setVisible(false);
    ui->idComptelabel           ->setVisible(false);

    QHBoxLayout *vlay = new QHBoxLayout();
    int marge   = 5;
    int space   = 5;
    vlay        ->setContentsMargins(marge,marge,marge,marge);
    vlay        ->setSpacing(space);
    vlay        ->addWidget(ui->Compteframe);
    vlay        ->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Expanding));
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay        ->setContentsMargins(marge,marge,marge,marge);
    hlay        ->setSpacing(space);
    hlay        ->addWidget(wdg_buttonframe->widgButtonParent());
    hlay        ->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));
    hlay        ->addLayout(vlay);
    dlglayout() ->insertLayout(0,hlay);
    dlglayout() ->setSizeConstraint(QLayout::SetFixedSize);

    m_mode = Norm;
    ui->DesactiveComptecheckBox->setVisible(true);
}

dlg_gestioncomptes::~dlg_gestioncomptes()
{
    delete ui;
}

void dlg_gestioncomptes::closeEvent(QCloseEvent *event)
{
     if (ui->ComptesuptableWidget->rowCount() == 0)
    {
        UpMessageBox msgbox;
        UpSmallButton OKBouton(tr("Je confirme"));
        UpSmallButton NoBouton(tr("Annuler"));
        msgbox.setText(tr("Vous n'avez pas de compte bancaire enregistré!"));
        msgbox.setInformativeText(tr("Vous ne pourrez pas utiliser de comptabilité"));
        msgbox.setIcon(UpMessageBox::Warning);
        msgbox.addButton(&NoBouton, UpSmallButton::CANCELBUTTON);
        msgbox.addButton(&OKBouton, UpSmallButton::STARTBUTTON);
        msgbox.exec();
        if (msgbox.clickedButton() != &OKBouton)
            event->ignore();
    }
}

void dlg_gestioncomptes::AfficheCompte(QTableWidgetItem *pitem, QTableWidgetItem *)
{
    int idCompte    = ui->ComptesuptableWidget->item(pitem->row(),0)->text().toInt();
    m_compteencours = Datas::I()->comptes ->getById(idCompte);
    if (!m_compteencours)
        return;
    Banque *bq = Datas::I()->banques->getById(m_compteencours->id());
        ui->BanqueupcomboBox            ->setCurrentText(bq? bq->nomabrege() : "");
        ui->IBANuplineEdit              ->setText(m_compteencours->iban());
        ui->IntituleCompteuplineEdit    ->setText(m_compteencours->intitulecompte());
        ui->NomCompteAbregeuplineEdit   ->setText(m_compteencours->nomabrege());
        ui->idCompteupLineEdit          ->setText(QString::number(m_compteencours->id()));
        ui->DesactiveComptecheckBox     ->setChecked(m_compteencours->isDesactive());
        ui->CompteSocietecheckBox       ->setChecked(m_compteencours->isPartage());
    wdg_buttonframe->wdg_modifBouton    ->setEnabled(false);

    /*On ne peut pas supprimer un compte s'il est utilisé ou s'il y a déjà eu des ecritures bancaires*/
    bool autorsupprimer = true;
    for (auto itusr = Datas::I()->users->actifs()->begin(); itusr != Datas::I()->users->actifs()->end();)
    {
        if (itusr.value()->idcomptepardefaut() == idCompte || itusr.value()->idcompteencaissementhonoraires()  == idCompte)
        {
            autorsupprimer = false;
            break;
        }
        ++ itusr;
    }
    if (autorsupprimer)
    {
        bool ok = true;
        QString req = "select " CP_IDCOMPTE_ARCHIVESCPT " from " TBL_ARCHIVESBANQUE
                " where " CP_IDCOMPTE_ARCHIVESCPT " = " + QString::number(idCompte) +
                " union"
                " select " CP_IDCOMPTE_LIGNCOMPTES " from " TBL_LIGNESCOMPTES
                " where " CP_IDCOMPTE_LIGNCOMPTES " = " + QString::number(idCompte) +
                " limit 1";
        autorsupprimer = (db->StandardSelectSQL(req, ok).size() == 0);
    }
    wdg_buttonframe->wdg_moinsBouton->setEnabled(autorsupprimer);
    QString ttip = "";
    if (!autorsupprimer)
        ttip = tr("Impossible de supprimer ce compte") + "\n" + tr("Des écritures  ont été enregistrées") + "\n" + tr("ou il est utilisé par un utilisateur");
}

void dlg_gestioncomptes::AnnulModif()
{
    ui->Compteframe->setEnabled(false);
    m_mode = Norm;
    ui->DesactiveComptecheckBox->setVisible(true);
    if (ui->ComptesuptableWidget->rowCount() > 0)
        AfficheCompte(ui->ComptesuptableWidget->item(ui->ComptesuptableWidget->currentRow(),0),ui->ComptesuptableWidget->item(ui->ComptesuptableWidget->currentRow(),0));
    else
        ui->Compteframe->setVisible(false);
    ui->OKModifupSmallButton->setVisible(false);
    ui->AnnulModifupSmallButton->setVisible(false);
    ui->ComptesuptableWidget->setEnabled(true);
    wdg_buttonframe->setEnabled(true);
    ui->CompteFacticePushButton->setVisible(false);
    ui->ComptesuptableWidget->setFocus();
}

void dlg_gestioncomptes::Banques()
{
    dlg_gestionbanques *Dlg_Banq = new dlg_gestionbanques();
    if (Dlg_Banq->exec()>0)
        MetAJourListeBanques();
}

void dlg_gestioncomptes::DesactiveCompte()
{
    ui->BanqueupcomboBox            ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->IBANuplineEdit              ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->IntituleCompteuplineEdit    ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->NomCompteAbregeuplineEdit   ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->CompteSocietecheckBox       ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    if (!ui->DesactiveComptecheckBox->isChecked())
    {
        /*On ne peut pas desactiver un compte s'il est le seul compte active pour cet utilisateur
        */
        bool ok = true;
        QList<QVariantList> listcomptes = db->SelectRecordsFromTable(QStringList() << CP_ID_COMPTES,
                                                                        TBL_COMPTES, ok,
                                                                        "where " CP_IDUSER_COMPTES " = " + QString::number(m_userencours->id()) + " and " CP_DESACTIVE_COMPTES " is null");
        ui->DesactiveComptecheckBox ->setEnabled(listcomptes.size()>1);
    }
}

void dlg_gestioncomptes::ChoixButtonFrame()
{
    switch (wdg_buttonframe->Choix()) {
    case WidgetButtonFrame::Plus:
        NouvCompte();
        break;
    case WidgetButtonFrame::Modifier:
        ModifCompte();
        break;
    case WidgetButtonFrame::Moins:
        SupprCompte();
        break;
    }
}

void dlg_gestioncomptes::Clign()
{
    m_visible = !m_visible;
    if (m_visible)
        ui->CompteFacticePushButton->setIcon(Icons::icNull());
    else
        ui->CompteFacticePushButton->setIcon(Icons::icHelp());
}

void dlg_gestioncomptes::CompteFactice()
{
    UpMessageBox msgbox;
    UpSmallButton OKBouton(tr("Annuler"));
    UpSmallButton RemplirBouton(tr("Utiliser des coordonnées bancaires factices"));
    msgbox.setText(tr("Enregistrement des coordonnées bancaires"));
    msgbox.setInformativeText(tr("Si vous venez de créez ce compte utilisateur et que vous vous enregistrez en tant que libéral,"
                              "il faut indiquer vos coordonnées bancaires pour le bon fonctionnement de la comptabilité\n"
                              "notamment l'édition automatique des remises de chèques.\n"
                              "Si vous souhaitez seulement le tester, le logiciel "
                              "peut remplir les champs avec des coordonnées factices.\n"
                              "Voulez vous remplir automatiquement les champs avec des coordonnées factices?.\n"));
    msgbox.setIcon(UpMessageBox::Info);
    msgbox.addButton(&RemplirBouton, UpSmallButton::STARTBUTTON);
    msgbox.addButton(&OKBouton, UpSmallButton::CANCELBUTTON);
    msgbox.exec();
    if (msgbox.clickedButton()==&RemplirBouton)
    {
        int idbanq = 0;
        bool ok = true;
        QList<QVariantList> listPaPRS = db->SelectRecordsFromTable(QStringList() << CP_ID_BANQUES,
                                                                      TBL_BANQUES, ok,
                                                                      "where" CP_NOMABREGE_BANQUES " = 'PaPRS'");
        if (listPaPRS.size()==0)
        {
            db->StandardSQL("insert into " TBL_BANQUES " (" CP_NOMABREGE_BANQUES ", " CP_NOMBANQUE_BANQUES ") values ('PaPRS','Panama Papers')");
            listPaPRS = db->SelectRecordsFromTable(QStringList() << CP_ID_BANQUES,
                                                   TBL_BANQUES, ok,
                                                   "where " CP_NOMABREGE_BANQUES " = 'PaPRS'");
        }
        idbanq = listPaPRS.at(0).at(0).toInt();
        MetAJourListeBanques();
        ui->BanqueupcomboBox->setCurrentIndex(ui->BanqueupcomboBox->findData(idbanq));
        QString intit;
        if (m_userencours->titre().size())
            intit += m_userencours->titre() + " ";
        intit += m_userencours->prenom() + " " + m_userencours->nom();
        if (Utils::trim(intit) == "")
            intit = "DR EDWARD SNOWDEN";
        ui->IntituleCompteuplineEdit    ->setText(intit);
        int al = 0;
        QString iban = "FR";
        srand(static_cast<uint>(time(Q_NULLPTR)));
        al = rand() % 100;
        while (al<10)
            al = rand() % 100;
        iban += QString::number(al) + " ";
        for(int i=0; i<5; i++)
        {
            al = rand() % 10000;
            while (al<1000)
                al = rand() % 10000;
            iban += QString::number(al) + " ";
        }
        al = rand() % 1000;
        while (al<100)
            al = rand() % 1000;
        iban += QString::number(al);
        ui->NomCompteAbregeuplineEdit   ->setText("PaPRS"+QString::number(al));
        ui->IBANuplineEdit              ->setText(iban);
        ui->CompteFacticePushButton     ->setVisible(false);
    }
}

void dlg_gestioncomptes::ModifCompte()
{
    m_mode = Modif;
    ui->DesactiveComptecheckBox     ->setVisible(true);
    ui->Compteframe                 ->setEnabled(true);
    ui->OKModifupSmallButton        ->setVisible(true);
    ui->AnnulModifupSmallButton     ->setVisible(true);
    ui->ComptesuptableWidget        ->setEnabled(false);
    ui->BanqueupcomboBox            ->setFocus();
    wdg_buttonframe                     ->setEnabled(false);

    /*On ne peut pas desactiver un compte s'il est le seul compte active pour cet utilisateur
    */
    bool ok = true;
    QList<QVariantList> listcomptes = db->SelectRecordsFromTable(QStringList() << CP_ID_COMPTES,
                                                                    TBL_COMPTES, ok,
                                                                    "where " CP_IDUSER_COMPTES " = " + QString::number(m_userencours->id()) + " and desactive is null");
    if (!ui->DesactiveComptecheckBox->isChecked())
        ui->DesactiveComptecheckBox ->setEnabled(listcomptes.size()>1);
    else
        ui->DesactiveComptecheckBox ->setEnabled(true);
    ui->BanqueupcomboBox            ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->IBANuplineEdit              ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->IntituleCompteuplineEdit    ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->NomCompteAbregeuplineEdit   ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
    ui->CompteSocietecheckBox       ->setEnabled(!ui->DesactiveComptecheckBox->isChecked());
}

void dlg_gestioncomptes::NouvCompte()
{
    m_mode = Nouv;
    ui->Compteframe                 ->setVisible(true);
    ui->Compteframe                 ->setEnabled(true);
    ui->OKModifupSmallButton        ->setVisible(true);
    ui->AnnulModifupSmallButton     ->setVisible(true);
    wdg_buttonframe                     ->setEnabled(false);

    ui->BanqueupcomboBox            ->setEnabled(true);
    ui->IBANuplineEdit              ->setEnabled(true);
    ui->IntituleCompteuplineEdit    ->setEnabled(true);
    ui->NomCompteAbregeuplineEdit   ->setEnabled(true);
    ui->CompteSocietecheckBox       ->setEnabled(true);
    ui->CompteSocietecheckBox       ->setChecked(m_userencours->isSocComptable());

    ui->BanqueupcomboBox            ->clearEditText();
    ui->IBANuplineEdit              ->clear();
    ui->IntituleCompteuplineEdit    ->clear();
    ui->NomCompteAbregeuplineEdit   ->clear();
    ui->idCompteupLineEdit          ->clear();
    ui->ComptesuptableWidget        ->setEnabled(false);
    ui->BanqueupcomboBox            ->setFocus();
    ui->CompteFacticePushButton     ->setVisible(true);
    ui->DesactiveComptecheckBox     ->setChecked(false);
    ui->DesactiveComptecheckBox     ->setVisible(false);
}

void dlg_gestioncomptes::SupprCompte()
{
    /* si on est à ce point, c'est qu'aucune écriture n'a été saisie sur ce compte
     * si le compte n'est pas partagé, on le supprime et on supprime son lien dans comptesJiointures
    */
    if(!m_compteencours)
        return;
    UpMessageBox msgbox;
    UpSmallButton OKBouton(tr("Supprimer le compte"));
    UpSmallButton NoBouton(tr("Annuler"));
    msgbox.setIcon(UpMessageBox::Warning);
    msgbox.addButton(&NoBouton, UpSmallButton::CANCELBUTTON);
    msgbox.addButton(&OKBouton, UpSmallButton::SUPPRBUTTON);
    Banque *bq = Datas::I()->banques->getById(m_compteencours->id());
    if (!bq)
        return;
    msgbox.setInformativeText(tr("Supprimer le compte ") + (bq? bq->nomabrege() : "") + " - " + m_compteencours->intitulecompte() + "?");
    msgbox.exec();
    if (msgbox.clickedButton() != &OKBouton)
        return;
    Datas::I()->comptes->SupprimeCompte(Datas::I()->comptes->getById(ui->idCompteupLineEdit->text().toInt()));
    m_userencours->setlistecomptesbancaires(Datas::I()->comptes->initListeComptesByIdUser(m_userencours->id()));
    m_listescomptesusr = m_userencours->listecomptesbancaires();
    RemplirTableView();
}

void dlg_gestioncomptes::Fermer()
{
    if (ui->OKModifupSmallButton->isVisible() && ui->OKModifupSmallButton->isEnabled())
    {
       if (UpMessageBox::Question(this, tr("Modifications non enregistrées"), tr("Voulez-vous enregistrer les modifications avant de fermer la fiche?")) == UpSmallButton::STARTBUTTON)
       {
           if (!VerifCompte())
               return;
           ValidCompte();
       }
    }
    close();
}

void dlg_gestioncomptes::ValidCompte()
{
    int idcompte=0;
    if (!VerifCompte())
        return;
    ui->Compteframe->setEnabled(false);
    ui->OKModifupSmallButton->setVisible(false);
    bool ok = true;
    QList<QVariantList> listbanq = db->SelectRecordsFromTable(QStringList() << CP_ID_BANQUES,
                                                                  TBL_BANQUES, ok,
                                                                  "where " CP_NOMBANQUE_BANQUES " = '" + Utils::correctquoteSQL(ui->BanqueupcomboBox->currentText()) + "'");
    int idbanque = listbanq.at(0).at(0).toInt();
    if (m_mode == Modif)
    {
        idcompte = ui->idCompteupLineEdit->text().toInt();
        QHash<QString, QVariant> listsets;
        listsets.insert(CP_IBAN_COMPTES            , ui->IBANuplineEdit->text());
        listsets.insert(CP_INTITULE_COMPTES        , ui->IntituleCompteuplineEdit->text());
        listsets.insert(CP_NOMABREGE_COMPTES       , ui->NomCompteAbregeuplineEdit->text());
        listsets.insert(CP_IDBANQUE_COMPTES        , QString::number(idbanque));
        listsets.insert(CP_PARTAGE_COMPTES         , (ui->CompteSocietecheckBox->isChecked()? "1" : "null"));
        listsets.insert(CP_DESACTIVE_COMPTES       , (ui->DesactiveComptecheckBox->isChecked()? "1" : "null"));
        db->UpdateTable(TBL_COMPTES,
                        listsets,
                        "where " CP_ID_COMPTES " = "          + ui->idCompteupLineEdit->text());
        m_compteencours = Datas::I()->comptes->getById(idcompte, true);
    }
    else if (m_mode == Nouv)
    {
        Compte* cpt  = Datas::I()->comptes->CreationCompte(idbanque,                           //! idBanque
                                            m_userencours->id(),                               //! idUser
                                            ui->IBANuplineEdit->text(),                        //! IBAN
                                            ui->IntituleCompteuplineEdit->text(),              //! IntituleCompte
                                            ui->NomCompteAbregeuplineEdit->text(),             //! NomCompteAbrege
                                            0,                                                 //! SoldeSurDernierReleve
                                            ui->CompteSocietecheckBox->isChecked(),            //! Partage
                                            ui->DesactiveComptecheckBox->isChecked());         //! Desactive
        idcompte = cpt->id();
        m_compteencours = Datas::I()->comptes->getById(idcompte);
    }
    m_listescomptesusr.clear();
    m_userencours     ->setlistecomptesbancaires(Datas::I()->comptes->initListeComptesByIdUser(m_userencours->id()));
    m_listescomptesusr = m_userencours->listecomptesbancaires();

    RemplirTableView();
    ui->OKModifupSmallButton->setVisible(false);
    ui->AnnulModifupSmallButton->setVisible(false);
    wdg_buttonframe->setEnabled(true);
    ui->ComptesuptableWidget->setEnabled(true);
    ui->ComptesuptableWidget->setFocus();
    ui->ComptesuptableWidget->setCurrentCell(ui->ComptesuptableWidget->findItems(QString::number(idcompte),Qt::MatchExactly).at(0)->row(),1);
    m_mode = Norm;
    ui->DesactiveComptecheckBox->setVisible(true);
}

void dlg_gestioncomptes::MetAJourListeBanques()
{
    bool ok = true;
    QList<QVariantList> listbanques = db->SelectRecordsFromTable(QStringList() << CP_NOMBANQUE_BANQUES << CP_ID_BANQUES,
                                                                  TBL_BANQUES, ok);
    ui->BanqueupcomboBox->clear();
    for (int i=0; i<listbanques.size(); i++)
        ui->BanqueupcomboBox->insertItem(0, listbanques.at(i).at(0).toString(), listbanques.at(i).at(1).toInt());
}

void dlg_gestioncomptes::RemplirTableView(int idcompte)
{
    ui->ComptesuptableWidget->disconnect();
    QTableWidgetItem    *pitem0, *pitem1;
    ui->ComptesuptableWidget->clearContents();
    ui->ComptesuptableWidget->setColumnCount(2);
    ui->ComptesuptableWidget->setColumnHidden(0,true);
    ui->ComptesuptableWidget->setColumnWidth(1,ui->ComptesuptableWidget->width());
    ui->ComptesuptableWidget->verticalHeader()->setVisible(false);
    ui->ComptesuptableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    ui->ComptesuptableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(Icons::icEuroCount(),"Comptes"));
    ui->ComptesuptableWidget->horizontalHeader()->setVisible(true);
    ui->ComptesuptableWidget->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignCenter);
    ui->ComptesuptableWidget->horizontalHeader()->setIconSize(QSize(25,25));
    ui->ComptesuptableWidget->setGridStyle(Qt::DotLine);

    m_listescomptesusr.clear();
    m_listescomptesusr = m_userencours->listecomptesbancaires(true);
    if (m_listescomptesusr.size()>0)
    {
        ui->Compteframe->setVisible(true);
        ui->ComptesuptableWidget->setRowCount(m_listescomptesusr.size());
        int i=0;
        foreach (int idcpt, m_listescomptesusr)
        {
            Compte *cpt = Datas::I()->comptes->getById(idcpt);
            if (cpt != Q_NULLPTR)
            {
                pitem0 = new QTableWidgetItem;
                pitem1 = new QTableWidgetItem;
                pitem0->setText(QString::number(cpt->id()));
                pitem1->setText(cpt->nomabrege());
                ui->ComptesuptableWidget->setItem(i,0,pitem0);
                ui->ComptesuptableWidget->setItem(i,1,pitem1);
                ui->ComptesuptableWidget->setRowHeight(i,int(QFontMetrics(qApp->font()).height()*1.3));
                i++;
            }
        }
        connect(ui->ComptesuptableWidget, &QTableWidget::currentItemChanged, this, [=] {AfficheCompte(ui->ComptesuptableWidget->currentItem(),Q_NULLPTR);});
        if (idcompte<1)
            ui->ComptesuptableWidget->setCurrentItem(ui->ComptesuptableWidget->item(0,1));
        else
            ui->ComptesuptableWidget->setCurrentItem(ui->ComptesuptableWidget->findItems(QString::number(idcompte), Qt::MatchExactly).at(0));
    }
    else
        ui->Compteframe->setVisible(false);
}

bool dlg_gestioncomptes::VerifCompte()
{
    QString msg = "";
    if (ui->BanqueupcomboBox->currentText() == "")
        msg = tr("la banque");
    else if (ui->IntituleCompteuplineEdit->text() == "")
        msg = tr("l'intitulé");
    else if (ui->IBANuplineEdit->text() == "")
        msg = tr("le code IBAN");
    else if (ui->NomCompteAbregeuplineEdit->text() == "")
        msg = tr("le nom abrégé du compte");
    if (msg != "")
    {
        UpMessageBox::Watch(this,tr("Vous n'avez pas précisé ") + msg);
        return false;
    }

    int idbanque = -1;
    bool ok = true;
    QList<QVariantList> listbanq = db->SelectRecordsFromTable(QStringList() << CP_ID_BANQUES,
                                                                  TBL_BANQUES, ok,
                                                                  "where " CP_NOMBANQUE_BANQUES " = '" + Utils::correctquoteSQL(ui->BanqueupcomboBox->currentText()) + "'");
    idbanque = listbanq.at(0).at(0).toInt();

    if (m_mode == Nouv)
    {
        QList<QVariantList> listcpt = db->SelectRecordsFromTable(QStringList() << CP_IDBANQUE_COMPTES,
                                                                      TBL_COMPTES, ok,
                                                                      "where " CP_IDUSER_COMPTES " = " + QString::number(m_userencours->id()) + " and idbanque = " + QString::number(idbanque));
        if (listcpt.size()>0)
        {
            UpMessageBox::Watch(this,tr("Vous avez déjà un compte enregistré dans cet organisme bancaire!"));
            return false;
        }
        QList<QVariantList> listnomcpt = db->SelectRecordsFromTable(QStringList() << CP_NOMABREGE_COMPTES,
                                                                      TBL_COMPTES, ok,
                                                                      "where " CP_IDUSER_COMPTES " = " + QString::number(m_userencours->id()) + " and nomcompteabrege = '" + Utils::correctquoteSQL(ui->NomCompteAbregeuplineEdit->text()) + "'");
        if (listnomcpt.size()>0)
        {
            UpMessageBox::Watch(this,tr(" Vous avez déjà un compte enregistré avec ce nom abrégé!"));
            return false;
        }
        QList<QVariantList> listiban = db->SelectRecordsFromTable(QStringList() << CP_IBAN_COMPTES, TBL_COMPTES, ok);
        if (listiban.size()>0)
        {
            QStringList ibanlist;
            for (int i=0; i<listiban.size(); i++)
                ibanlist << listiban.at(0).at(0).toString().replace(" ","").toUpper();
            if (ibanlist.contains(ui->IBANuplineEdit->text().replace(" ","").toUpper()))
            {
                UpMessageBox::Watch(this,("Un compte est déjà enregistré avec cet IBAN!"));
                return false;
            }
        }
    }
    else if (m_mode == Modif)
    {
        QList<QVariantList> listcpt = db->SelectRecordsFromTable(QStringList() << "idbanque",
                                                                    TBL_COMPTES, ok,
                                                                    "where " CP_IDUSER_COMPTES " = " + QString::number(m_userencours->id()) +
                                                                    " and " CP_IDBANQUE_COMPTES " = " + QString::number(idbanque) +
                                                                    " and " CP_ID_COMPTES " <> " + ui->idCompteupLineEdit->text());
        if (listcpt.size()>0)
        {
            UpMessageBox::Watch(this,tr(" Vous avez déjà un compte enregistré dans cet organisme bancaire!"));
            return false;
        }
        QList<QVariantList> listnomabrg = db->SelectRecordsFromTable(QStringList() << CP_NOMABREGE_COMPTES,
                                                                    TBL_COMPTES, ok,
                                                                    "where " CP_IDUSER_COMPTES " = " + QString::number(m_userencours->id()) +
                                                                    " and " CP_NOMABREGE_COMPTES " = '" + ui->NomCompteAbregeuplineEdit->text() + "'" +
                                                                    " and " CP_ID_COMPTES " <> " + ui->idCompteupLineEdit->text());
        if (listnomabrg.size()>0)
        {
            UpMessageBox::Watch(this,tr(" Vous avez déjà un compte enregistré avec ce nom abrégé!"));
            return false;
        }
        QList<QVariantList> listiban = db->SelectRecordsFromTable(QStringList() << CP_IBAN_COMPTES,
                                                                    TBL_COMPTES, ok,
                                                                    "where " CP_ID_COMPTES " <> " + ui->ComptesuptableWidget->item(ui->ComptesuptableWidget->currentRow(),0)->text());
        if (listiban.size()>0)
        {
            QStringList ibanlist;
            for (int i=0; i<listiban.size(); i++)
                ibanlist << listiban.at(i).at(0).toString().replace(" ","").toUpper();
            if (ibanlist.contains(ui->IBANuplineEdit->text().replace(" ","").toUpper()))
            {
                UpMessageBox::Watch(this,tr("Un compte est déjà enregistré avec cet IBAN!"));
                return false;
            }
        }
    }
    return true;
}
