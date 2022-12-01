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

#include "dlg_gestionbanques.h"
#include "ui_dlg_gestionbanques.h"

dlg_gestionbanques::dlg_gestionbanques(QWidget *parent, QString nouvbanqueabrege) :
    UpDialog(parent),
    ui(new Ui::dlg_gestionbanques)
{
    ui->setupUi(this);

    db                      = DataBase::I();
    m_fermeapresvalidation   = (nouvbanqueabrege != "");
    setWindowModality(Qt::WindowModal);

    if (m_fermeapresvalidation)
    {
        NouvBanque();
        ui->NomAbregeupLineEdit->setText(nouvbanqueabrege);
        ui->NomAbregeupLineEdit->setFocusPolicy(Qt::NoFocus);
        dlglayout()->setSizeConstraint(QLayout::SetFixedSize);
    }
    else
    {
        int larg    = 350;
        int ncolvis = 12;
        int hautrow = int(QFontMetrics(qApp->font()).height()*1.3);
        int haut    = hautrow * ncolvis;
        uptablebanq = new UpTableWidget();
        uptablebanq->setFixedWidth(larg);
        uptablebanq->resize(larg, haut);
        uptablebanq->verticalHeader()->setVisible(false);
        uptablebanq->setFocusPolicy(Qt::StrongFocus);
        uptablebanq->setSelectionMode(QAbstractItemView::SingleSelection);
        uptablebanq->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        uptablebanq->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        uptablebanq->setGridStyle(Qt::DotLine);
        uptablebanq->setEditTriggers(QAbstractItemView::NoEditTriggers);
        uptablebanq->setSelectionBehavior(QAbstractItemView::SelectRows);
        uptablebanq->setColumnCount(2);
        uptablebanq->setColumnHidden(0,true);
        uptablebanq->setColumnWidth(1,uptablebanq->width()-2);
        uptablebanq->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
        uptablebanq->setHorizontalHeaderItem(1, new QTableWidgetItem(Icons::icEuro(),"Banques"));
        uptablebanq->horizontalHeader()->setVisible(true);
        uptablebanq->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignCenter);
        uptablebanq->horizontalHeader()->setIconSize(QSize(25,25));
        uptablebanq->horizontalHeader()->setFixedHeight(hautrow);

        wdg_buttonframe = new WidgetButtonFrame(uptablebanq);
        wdg_buttonframe->AddButtons(WidgetButtonFrame::Plus | WidgetButtonFrame::Modifier | WidgetButtonFrame::Moins);
        /*
        widgButtons->widgButtonParent()->setGeometry(10,10,larg, haut + widgButtons->height());
        widgButtons->widgButtonParent()->sizePolicy().setVerticalStretch(10);
        widgButtons->widgButtonParent()->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        int x = widgButtons->widgButtonParent()->sizeHint().width();
        int y = widgButtons->widgButtonParent()->sizeHint().height();
        qDebug() << (widgButtons->widgButtonParent()->sizeHint().isValid()? "Valide": "Null") << " - x = " + QString::number(x) + " - y = " + QString::number(y) + " - hauteur ligne = " + QString::number(haut + widgButtons->height()) + " Fixed";
        widgButtons->widgButtonParent()->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        x = widgButtons->widgButtonParent()->sizeHint().width();
        y = widgButtons->widgButtonParent()->sizeHint().height();
        qDebug() << (widgButtons->widgButtonParent()->sizeHint().isValid()? "Valide": "Null") << " - x = " + QString::number(x) + " - y = " + QString::number(y) + " - hauteur ligne = " + QString::number(haut + widgButtons->height()) + " Preferred";
        widgButtons->widgButtonParent()->sizePolicy().setVerticalPolicy(QSizePolicy::MinimumExpanding);
        x = widgButtons->widgButtonParent()->sizeHint().width();
        y = widgButtons->widgButtonParent()->sizeHint().height();
        qDebug() << (widgButtons->widgButtonParent()->sizeHint().isValid()? "Valide": "Null") << " - x = " + QString::number(x) + " - y = " + QString::number(y) + " - hauteur ligne = " + QString::number(haut + widgButtons->height()) + " MinimumExpanding";
        widgButtons->widgButtonParent()->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        x = widgButtons->widgButtonParent()->sizeHint().width();
        y = widgButtons->widgButtonParent()->sizeHint().height();
        qDebug() << (widgButtons->widgButtonParent()->sizeHint().isValid()? "Valide": "Null") << " - x = " + QString::number(x) + " - y = " + QString::number(y) + " - hauteur ligne = " + QString::number(haut + widgButtons->height()) + " Minimum";
        */
        dlglayout() ->insertWidget(0,wdg_buttonframe->widgButtonParent());
        int r,t,l,b;
        dlglayout() ->getContentsMargins(&r,&t,&l,&b);
        AjouteLayButtons(UpDialog::ButtonClose);
        resize(larg + r + l,
               haut + wdg_buttonframe->height() + ui->Banqueframe->height() + CloseButton->height() + t + b + (dlglayout()->spacing()*2));
        RemplirTableView();
        uptablebanq->setCurrentCell(0,1);
        AfficheBanque();
        connect(uptablebanq,        &UpTableWidget::itemSelectionChanged,   this,   &dlg_gestionbanques::AfficheBanque);
        connect(wdg_buttonframe,    &WidgetButtonFrame::choix,              this,   &dlg_gestionbanques::ChoixButtonFrame);
        connect(CloseButton,        &QPushButton::clicked,                  this,   &dlg_gestionbanques::accept);
        ui->AnnulModifupSmallButton ->setVisible(false);
        ui->OKModifupSmallButton    ->setVisible(false);
    }

    connect(ui->AnnulModifupSmallButton,    &QPushButton::clicked,    this,   &dlg_gestionbanques::AnnuleModifBanque);
    connect(ui->OKModifupSmallButton,       &QPushButton::clicked,    this,   &dlg_gestionbanques::ValideModifBanque);

    ui->NomBanqueupLineEdit ->setValidator(new QRegExpValidator(Utils::rgx_rx));
    QRegExp val             = QRegExp("[A-Z]*");
    ui->NomAbregeupLineEdit ->setValidator(new QRegExpValidator(val));
}

dlg_gestionbanques::~dlg_gestionbanques()
{
    delete ui;
}

void dlg_gestionbanques::AfficheBanque()
{
    UpLabel* lbl = static_cast<UpLabel*>(uptablebanq->cellWidget(uptablebanq->currentRow(),1));
    int idBanque = uptablebanq->item(lbl->Row(),0)->text().toInt();
    bool ok = true;
    QList<QVariantList> listbanques = db->SelectRecordsFromTable(QStringList() << CP_NOMBANQUE_BANQUES << CP_NOMABREGE_BANQUES << CP_ID_BANQUES,
                                                                    TBL_BANQUES, ok,
                                                                    "where " CP_ID_BANQUES " = " + QString::number(idBanque));
    if (listbanques.size()>0)
    {
        QVariantList banque = listbanques.at(0);
        ui->NomBanqueupLineEdit->setText(banque.at(0).toString());
        ui->NomAbregeupLineEdit->setText(banque.at(1).toString());
    }
    wdg_buttonframe->wdg_moinsBouton->setEnabled(true);
    QList<QVariantList> listcomptes = db->SelectRecordsFromTable(QStringList() << CP_IDBANQUE_COMPTES,
                                                                    TBL_COMPTES, ok,
                                                                    "where " CP_IDBANQUE_COMPTES " = " + listbanques.at(0).at(2).toString());
    if (listcomptes.size()>0)
        wdg_buttonframe->wdg_moinsBouton->setEnabled(false);
    lbl = Q_NULLPTR;
    delete lbl;
}

void dlg_gestionbanques::AnnuleModifBanque()
{
    if (m_fermeapresvalidation)
        reject();
    else
    {
        RemetEnNorm();
        AfficheBanque();
    }
}

void dlg_gestionbanques::ChoixButtonFrame()
{
    switch (wdg_buttonframe->Choix()) {
    case WidgetButtonFrame::Plus:
        NouvBanque();
        break;
    case WidgetButtonFrame::Modifier:
        ModifBanque();
        break;
    case WidgetButtonFrame::Moins:
        SupprBanque();
        break;
    }
}

void dlg_gestionbanques::NouvBanque()
{
    ui->AnnulModifupSmallButton->setVisible(true);
    ui->OKModifupSmallButton->setVisible(true);
    ui->Banqueframe->setEnabled(true);
    ui->NomBanqueupLineEdit->clear();
    ui->NomAbregeupLineEdit->clear();
    if (!m_fermeapresvalidation)
    {
        uptablebanq->setEnabled(false);
        wdg_buttonframe->setEnabled(false);
    }
    m_mode = Nouv;
}

void dlg_gestionbanques::ModifBanque()
{
    ui->AnnulModifupSmallButton->setVisible(true);
    ui->OKModifupSmallButton->setVisible(true);
    ui->Banqueframe->setEnabled(true);
    uptablebanq->setEnabled(false);
    wdg_buttonframe->setEnabled(false);
    m_mode = Modif;
}

void dlg_gestionbanques::SupprBanque()
{
    UpLabel* lbl = static_cast<UpLabel*>(uptablebanq->cellWidget(uptablebanq->currentRow(),1));
    int idBanque = uptablebanq->item(lbl->Row(),0)->text().toInt();
    UpMessageBox msgbox(this);
    UpSmallButton OKBouton(tr("Supprimer"));
    UpSmallButton NoBouton(tr("Annuler"));
    msgbox.setText(tr("Supprimer la banque ") + lbl->text() + "?");
    msgbox.setIcon(UpMessageBox::Warning);
    msgbox.addButton(&NoBouton, UpSmallButton::CANCELBUTTON);
    msgbox.addButton(&OKBouton, UpSmallButton::STARTBUTTON);
    msgbox.exec();
    if (msgbox.clickedButton() != &OKBouton)
       return;
    bool ok = true;
    QList<QVariantList> listcomptes = db->SelectRecordsFromTable(QStringList() << "idBanque",
                                                                    TBL_COMPTES, ok,
                                                                    "where " CP_IDBANQUE_COMPTES " = " + QString::number(idBanque));
    if (listcomptes.size()>0)
    {
        UpMessageBox::Watch(this, tr("Impossible de supprimer la banque ") + lbl->text(), tr("Elle est utilisée par d'autres utilisateurs"));
        return;
    }
    Datas::I()->banques->initListe();
    db->SupprRecordFromTable(idBanque, CP_ID_BANQUES, TBL_BANQUES);
    RemplirTableView();
    AfficheBanque();
}

void dlg_gestionbanques::ValideModifBanque()
{
    QString msg = "";
    QString nombanque = Utils::capitilize(ui->NomBanqueupLineEdit->text());
    if (ui->NomBanqueupLineEdit->text() == "")
        msg = tr("le nom de la banque");
    else if (ui->NomAbregeupLineEdit->text() == "")
        msg = tr("le nom abrégé de la banque");
    if (msg != "")
    {
        UpMessageBox::Watch(this,tr("Vous n'avez pas précisé ") + msg);
        return;
    }

    if (m_mode == Nouv)
    {
        for (auto it = Datas::I()->banques->banques()->constBegin(); it != Datas::I()->banques->banques()->constEnd(); ++it)
        {
            Banque* bq = const_cast<Banque*>(it.value());
            if (bq->nom().toUpper() == ui->NomBanqueupLineEdit->text().toUpper())
            {
                UpMessageBox::Watch(this,tr("Cette banque est déjà enregistrée!"));
                return;
            }
        }
        bool ok = true;
        QList<QVariantList> listabreges = db->SelectRecordsFromTable(QStringList() << CP_NOMABREGE_BANQUES, TBL_BANQUES, ok);
        if (listabreges.size()>0)
            for (int i=0; i<listabreges.size(); i++)
                if (listabreges.at(i).at(0).toString() == ui->NomAbregeupLineEdit->text())
                {
                    msg = tr("il y a déjà un organisme bancaire enregistré avec cette abréviation");
                    UpMessageBox::Watch(this,msg);
                    return;
                }
        listabreges = db->SelectRecordsFromTable(QStringList() << CP_NOMABREGE_BANQUES,
                                                                      TBL_BANQUES, ok,
                                                                      "where " CP_NOMABREGE_BANQUES " = '" + ui->NomAbregeupLineEdit->text() + "'");
        if(listabreges.size()>0)
        {
            UpMessageBox::Watch(this,tr("Cette abréviation est déjà utilisée!"));
            ui->NomAbregeupLineEdit->setFocus();
            return;
        }
        QHash<QString, QString> listsets;
        listsets.insert(CP_NOMABREGE_BANQUES,   ui->NomAbregeupLineEdit->text());
        listsets.insert(CP_NOMBANQUE_BANQUES,        nombanque);
        db->InsertIntoTable(TBL_BANQUES, listsets);
        Datas::I()->banques->initListe();
        if (m_fermeapresvalidation)
        {
            UpMessageBox::Watch(this,tr("La banque ") + nombanque + tr(" a été enregistrée"));
            accept();
            return;
        }
    }

    else if (m_mode == Modif)
    {
        UpLabel* lbl = static_cast<UpLabel*>(uptablebanq->cellWidget(uptablebanq->currentRow(),1));
        int idBanque = uptablebanq->item(lbl->Row(),0)->text().toInt();
        bool ok = true;
        QList<QVariantList> listabreges = db->SelectRecordsFromTable(QStringList() << CP_NOMBANQUE_BANQUES,
                                                                                             TBL_BANQUES, ok,
                                                                                             "where " CP_ID_BANQUES " <> " + QString::number(idBanque));
        if (listabreges.size()>0)
            for (int i=0; i<listabreges.size(); i++)
                if (listabreges.at(i).at(0).toString().toUpper() == nombanque.toUpper())
                {
                    msg = tr("il y a déjà un organisme bancaire enregistré avec ce nom");
                    UpMessageBox::Watch(this,msg);
                    return;
                }
        listabreges = db->SelectRecordsFromTable(QStringList() << CP_NOMABREGE_BANQUES,
                                                                      TBL_BANQUES, ok,
                                                                      "where " CP_ID_BANQUES " <> " + QString::number(idBanque));
        if (listabreges.size()>0)
            for (int i=0; i<listabreges.size(); i++)
                if (listabreges.at(i).at(0).toString() == ui->NomAbregeupLineEdit->text())
                {
                    msg = tr("il y a déjà un organisme bancaire enregistré avec cette abréviation");
                    UpMessageBox::Watch(this,msg);
                    return;
                }
        QHash<QString, QVariant> listsets;
        listsets.insert(CP_NOMBANQUE_BANQUES,      nombanque);
        listsets.insert(CP_NOMABREGE_BANQUES, ui->NomAbregeupLineEdit->text());
        DataBase:: I()->UpdateTable(TBL_BANQUES,
                                              listsets,
                                              "where " CP_ID_BANQUES " = " + QString::number(idBanque));
        Datas::I()->banques->initListe();
    }
    RemplirTableView();
    UpLabel *lbl;
    for (int i=0; i<uptablebanq->rowCount(); i++)
    {
        lbl = static_cast<UpLabel*>(uptablebanq->cellWidget(i,1));
        if (lbl->text() == nombanque)
        {
            uptablebanq->setCurrentCell(i,1);
            break;
        }
    }
    lbl = Q_NULLPTR;
    delete lbl;
    AfficheBanque();
    RemetEnNorm();
}

void dlg_gestionbanques::RemetEnNorm()
{
    m_mode = Norm;
    ui->AnnulModifupSmallButton->setVisible(false);
    ui->OKModifupSmallButton->setVisible(false);
    ui->Banqueframe->setEnabled(false);
    uptablebanq->setEnabled(true);
    wdg_buttonframe->setEnabled(true);
}

void dlg_gestionbanques::RemplirTableView()
{
    QTableWidgetItem    *pitem0;
    UpLabel             *label1;
    bool ok = true;
    QList<QVariantList> listbanques = db->SelectRecordsFromTable(QStringList() << CP_ID_BANQUES << CP_NOMBANQUE_BANQUES,
                                                                                         TBL_BANQUES, ok,
                                                                                         "",
                                                                                         "order by " CP_NOMBANQUE_BANQUES);
    if (listbanques.size() > 0)
    {
        uptablebanq->setRowCount(listbanques.size());
        for (int i=0; i<listbanques.size(); i++)
        {
            pitem0 = new QTableWidgetItem;
            label1 = new UpLabel;
            pitem0->setText(listbanques.at(i).at(0).toString());
            label1->setText(listbanques.at(i).at(1).toString());
            label1->setRow(i);
            uptablebanq->setItem(i,0,pitem0);
            uptablebanq->setCellWidget(i,1,label1);
            uptablebanq->setRowHeight(i,int(QFontMetrics(qApp->font()).height()*1.3));
        }
    }
}
