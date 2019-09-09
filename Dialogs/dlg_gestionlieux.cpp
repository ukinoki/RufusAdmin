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

#include "dlg_gestionlieux.h"
#include "utils.h"

dlg_GestionLieux::dlg_GestionLieux(QWidget *parent)
    : UpDialog(QDir::homePath() + FILE_INI, "PositionsFiches/PositionLieux", parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    db              = DataBase::I();
    AjouteLayButtons(UpDialog::ButtonClose);
    connect(CloseButton, &QPushButton::clicked, this, &dlg_GestionLieux::enregLieux);

    wdg_bigtable = new QTableView(this);
    wdg_adresselbl = new UpLabel();
    wdg_adresselbl->setFixedWidth(240);

    ReconstruitModel();
    wdg_buttonframe = new WidgetButtonFrame(wdg_bigtable);
    wdg_buttonframe->AddButtons(WidgetButtonFrame::PlusButton | WidgetButtonFrame::ModifButton | WidgetButtonFrame::MoinsButton);
    connect(wdg_bigtable->selectionModel(),    SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this,   SLOT(Slot_AfficheDetails(QModelIndex,QModelIndex)));
    wdg_bigtable->selectRow(0);

    QVBoxLayout *vlay   = new QVBoxLayout();
    QHBoxLayout *hlay   = new QHBoxLayout();
    vlay    ->addWidget(wdg_adresselbl);
    vlay    ->addSpacerItem(new QSpacerItem(5,5,QSizePolicy::Expanding,QSizePolicy::Expanding));
    hlay    ->addWidget(wdg_buttonframe->widgButtonParent());
    hlay    ->addLayout(vlay);
    dlglayout()     ->insertLayout(0,hlay);
    dlglayout()     ->setSizeConstraint(QLayout::SetFixedSize);

    connect(wdg_buttonframe,    &WidgetButtonFrame::choix,  this,   &dlg_GestionLieux::ChoixButtonFrame);
}

dlg_GestionLieux::~dlg_GestionLieux()
{
}

void dlg_GestionLieux::Slot_AfficheDetails(QModelIndex idx, QModelIndex)
{
    int row = idx.row();
    QString data ("");
    for (int i=2; i<9; i++)
    {
        QString datasuiv = m_model->itemData(m_model->index(row,i)).value(0).toString();
        switch (i) {
        case 2:
        case 3:
        case 4:
        case 5:
            if (data != "")
                if (datasuiv != "")
                    data += "\n";
            data += datasuiv;
            break;
        case 6:
            if (data != "")
                if (datasuiv != "")
                {
                    if (m_model->itemData(m_model->index(row,5)).value(0).toString() != "")
                        data += " ";
                    else
                        data += "\n";
                }
            data += datasuiv;
            break;
        case 7:
            if (data != "")
                if (datasuiv != "")
                    data += "\n";
            if (datasuiv != "")
                data += "Tel: " + datasuiv;
            break;
        case 8:
            if (data != "")
                if (datasuiv != "")
                    data += "\n";
            if (datasuiv != "")
                data += "Fax: " + datasuiv;
            break;
        default:
            break;
        }
    }
    wdg_adresselbl->setText(data);
    wdg_buttonframe->wdg_moinsBouton->setEnabled(db->StandardSelectSQL("select iduser from " TBL_JOINTURESLIEUX " where idlieu = " + m_model->itemData(m_model->index(row,0)).value(0).toString(), m_ok).size() == 0
                                  && m_model->itemData(m_model->index(row,0)).value(0).toInt() != m_idlieuserveur);
}

void dlg_GestionLieux::ChoixButtonFrame()
{
    switch (wdg_buttonframe->Choix()) {
    case WidgetButtonFrame::Plus:
        CreerLieu();
        break;
    case WidgetButtonFrame::Modifier:
        ModifLieu();
        break;
    case WidgetButtonFrame::Moins:
        SupprLieu();
        break;
    }
}

void dlg_GestionLieux::enregLieux()
{

    reject();
}

void dlg_GestionLieux::CreerLieu()
{
    ModifLieuxDialog();
    connect(dlg_lieu->OKButton, &QPushButton::clicked, this, &dlg_GestionLieux::enregNouvLieu);
    dlg_lieu->exec();
    delete  dlg_lieu;
}

void dlg_GestionLieux::enregNouvLieu()
{
    if (ValidationFiche())
    {
        QString req = "insert into " TBL_LIEUXEXERCICE "(NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, LieuCodePostal, LieuVille, LieuTelephone, LieuFax)  values("
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_nomledit->text())) + "', "
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_adress1ledit->text())) + "', "
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_adress2ledit->text())) + "', "
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_adress3ledit->text())) + "', "
                        ""  + Utils::correctquoteSQL(Utils::capitilize(wdg_cpledit->text())) + ", "
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_villeledit->text())) + "', "
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_telledit->text())) + "', "
                        "'" + Utils::correctquoteSQL(Utils::capitilize(wdg_faxledit->text())) + "')";
        //qDebug() << req;
        db->StandardSQL(req);
        delete m_model;
        ReconstruitModel();
        dlg_lieu->accept();
        connect(wdg_bigtable->selectionModel(),    SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this,   SLOT(Slot_AfficheDetails(QModelIndex,QModelIndex)));
        wdg_bigtable->selectRow(0);
    }
}

void dlg_GestionLieux::ModifLieuxDialog()
{
    dlg_lieu = new UpDialog(this);
    dlg_lieu->AjouteLayButtons(UpDialog::ButtonCancel | UpDialog::ButtonOK);
    dlg_lieu->setWindowTitle(tr("Enregistrer un nouveau lieu"));
    QVBoxLayout *laylbl = new QVBoxLayout();
    QVBoxLayout *layledit = new QVBoxLayout();
    QHBoxLayout *laycom = new QHBoxLayout();
    QVBoxLayout *lay = dynamic_cast<QVBoxLayout*>(dlg_lieu->layout());

    UpLabel *lblnom = new UpLabel(dlg_lieu, tr("Nom de la structure"));
    UpLabel *lbladr1 = new UpLabel(dlg_lieu, tr("Adresse1"));
    UpLabel *lbladr2 = new UpLabel(dlg_lieu, tr("Adresse2"));
    UpLabel *lbladr3 = new UpLabel(dlg_lieu, tr("Adresse3"));
    UpLabel *lblcp = new UpLabel(dlg_lieu, tr("Code postal"));
    UpLabel *lblville = new UpLabel(dlg_lieu, tr("Ville"));
    UpLabel *lbltel = new UpLabel(dlg_lieu, tr("Telephone"));
    UpLabel *lblfax = new UpLabel(dlg_lieu, tr("Fax"));

    int h = 22;
    lblnom  ->setFixedHeight(h);
    lbladr1 ->setFixedHeight(h);
    lbladr2 ->setFixedHeight(h);
    lbladr3 ->setFixedHeight(h);
    lblcp   ->setFixedHeight(h);
    lblville->setFixedHeight(h);
    lbltel  ->setFixedHeight(h);
    lblfax  ->setFixedHeight(h);

    laylbl->addWidget(lblnom);
    laylbl->addWidget(lbladr1);
    laylbl->addWidget(lbladr2);
    laylbl->addWidget(lbladr3);
    laylbl->addWidget(lblcp);
    laylbl->addWidget(lblville);
    laylbl->addWidget(lbltel);
    laylbl->addWidget(lblfax);
    laylbl->addSpacerItem(new QSpacerItem(5,5,QSizePolicy::Expanding,QSizePolicy::Expanding));

    wdg_nomledit   = new UpLineEdit(dlg_lieu);
    wdg_adress1ledit  = new UpLineEdit(dlg_lieu);
    wdg_adress2ledit  = new UpLineEdit(dlg_lieu);
    wdg_adress3ledit  = new UpLineEdit(dlg_lieu);
    wdg_cpledit    = new UpLineEdit(dlg_lieu);
    wdg_villeledit = new UpLineEdit(dlg_lieu);
    wdg_telledit   = new UpLineEdit(dlg_lieu);
    wdg_faxledit   = new UpLineEdit(dlg_lieu);

    wdg_nomledit    ->setFixedWidth(240);       // NomLieu
    wdg_adress1ledit   ->setFixedWidth(240);       // Adresse1
    wdg_adress2ledit   ->setFixedWidth(240);       // Adresse2
    wdg_adress3ledit   ->setFixedWidth(240);       // Adresse3
    wdg_cpledit     ->setFixedWidth(90);        // CP
    wdg_villeledit  ->setFixedWidth(240);       // Ville
    wdg_telledit    ->setFixedWidth(120);       // Telephone
    wdg_faxledit    ->setFixedWidth(120);       // Fax

    wdg_nomledit    ->setMaxLength(80);
    wdg_adress1ledit   ->setMaxLength(45);
    wdg_adress2ledit   ->setMaxLength(45);
    wdg_adress3ledit   ->setMaxLength(45);
    wdg_cpledit     ->setMaxLength(9);
    wdg_villeledit  ->setMaxLength(45);
    wdg_telledit    ->setMaxLength(17);
    wdg_faxledit    ->setMaxLength(17);

    wdg_nomledit    ->setValidator(new QRegExpValidator(Utils::rgx_ville));
    wdg_adress1ledit   ->setValidator(new QRegExpValidator(Utils::rgx_adresse));
    wdg_adress2ledit   ->setValidator(new QRegExpValidator(Utils::rgx_adresse));
    wdg_adress3ledit   ->setValidator(new QRegExpValidator(Utils::rgx_adresse));
    wdg_cpledit     ->setValidator(new QRegExpValidator(Utils::rgx_CP));
    wdg_villeledit  ->setValidator(new QRegExpValidator(Utils::rgx_ville));
    wdg_telledit    ->setValidator(new QRegExpValidator(Utils::rgx_telephone));
    wdg_faxledit    ->setValidator(new QRegExpValidator(Utils::rgx_telephone));

    layledit->addWidget(wdg_nomledit);
    layledit->addWidget(wdg_adress1ledit);
    layledit->addWidget(wdg_adress2ledit);
    layledit->addWidget(wdg_adress3ledit);
    layledit->addWidget(wdg_cpledit);
    layledit->addWidget(wdg_villeledit);
    layledit->addWidget(wdg_telledit);
    layledit->addWidget(wdg_faxledit);
    layledit->addSpacerItem(new QSpacerItem(5,5,QSizePolicy::Expanding,QSizePolicy::Expanding));

    for (int i=0; i< dlg_lieu->findChildren<UpLineEdit*>().size(); i++)
        connect(dlg_lieu->findChildren<UpLineEdit*>().at(i), &QLineEdit::textEdited, this, [=]{    dlg_lieu->OKButton->setEnabled(true);});
    dlg_lieu->OKButton->setEnabled(false);

    laycom->addLayout(laylbl);
    laylbl->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    laycom->addLayout(layledit);

    lay     ->insertLayout(0,laycom);
    lay     ->setSizeConstraint(QLayout::SetFixedSize);
}

void dlg_GestionLieux::ModifLieu()
{
    ModifLieuxDialog();
    m_idlieuamodifier = m_model->itemData(m_model->index(wdg_bigtable->currentIndex().row(),0)).value(0).toInt();
    QString req = "select idLieu, NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, LieuCodePostal, LieuVille, LieuTelephone, LieuFax from " TBL_LIEUXEXERCICE
                  " where idLieu = " + QString::number(m_idlieuamodifier);
    //qDebug() << req;
    QVariantList lieurcd= db->getFirstRecordFromStandardSelectSQL(req,m_ok);
    if (lieurcd.size()==0 || !m_ok)
        return;
    if (lieurcd.at(1).toString() != "")
        wdg_nomledit    ->setText(lieurcd.at(1).toString());
    else
        wdg_nomledit    ->setText(tr("non défini"));
    wdg_adress1ledit   ->setText(lieurcd.at(2).toString());
    wdg_adress2ledit   ->setText(lieurcd.at(3).toString());
    wdg_adress3ledit   ->setText(lieurcd.at(4).toString());
    wdg_cpledit     ->setText(lieurcd.at(5).toString());
    wdg_villeledit  ->setText(lieurcd.at(6).toString());
    wdg_telledit    ->setText(lieurcd.at(7).toString());
    wdg_faxledit    ->setText(lieurcd.at(8).toString());
    connect(dlg_lieu->OKButton, &QPushButton::clicked, this, &dlg_GestionLieux::enregModifLieu);
    dlg_lieu->exec();
    delete  dlg_lieu;
}

void dlg_GestionLieux::enregModifLieu()
{

    if (ValidationFiche())
    {
        QString req = "update " TBL_LIEUXEXERCICE " set "
                        "NomLieu = '"       + Utils::correctquoteSQL(Utils::capitilize(wdg_nomledit->text())) + "', "
                        "LieuAdresse1 = '"  + Utils::correctquoteSQL(Utils::capitilize(wdg_adress1ledit->text())) + "', "
                        "LieuAdresse2 = '"  + Utils::correctquoteSQL(Utils::capitilize(wdg_adress2ledit->text())) + "', "
                        "LieuAdresse3 = '"  + Utils::correctquoteSQL(Utils::capitilize(wdg_adress3ledit->text())) + "', "
                        "LieuCodePostal = " + Utils::correctquoteSQL(Utils::capitilize(wdg_cpledit->text())) + ", "
                        "LieuVille = '"     + Utils::correctquoteSQL(Utils::capitilize(wdg_villeledit->text())) + "', "
                        "LieuTelephone = '" + Utils::correctquoteSQL(Utils::capitilize(wdg_telledit->text())) + "', "
                        "LieuFax = '"       + Utils::correctquoteSQL(Utils::capitilize(wdg_faxledit->text())) + "' " +
                        "where idLieu = "   + QString::number(m_idlieuamodifier);
        //qDebug() << req;
        db->StandardSQL(req);
        delete m_model;
        ReconstruitModel();
        dlg_lieu->accept();
        connect(wdg_bigtable->selectionModel(),    SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this,   SLOT(Slot_AfficheDetails(QModelIndex,QModelIndex)));
        wdg_bigtable->selectRow(0);
    }
}

void dlg_GestionLieux::SupprLieu()
{
    int idLieuASupprimer = m_model->itemData(m_model->index(wdg_bigtable->currentIndex().row(),0)).value(0).toInt();
    QString lieu = m_model->itemData(m_model->index(wdg_bigtable->currentIndex().row(),1)).value(0).toString();
    if (UpMessageBox::Question(this,tr("Suppression d'un lieu de soins"),tr("voulez vous vraiment supprimer") + "\n" + lieu + " ?") == UpSmallButton::STARTBUTTON)
    {
        db->SupprRecordFromTable(idLieuASupprimer,"idlieu", TBL_LIEUXEXERCICE);
        ReconstruitModel();
        dlg_message(QStringList() << lieu + " supprimé", 3000);
        ReconstruitModel();
        connect(wdg_bigtable->selectionModel(),    SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this,   SLOT(Slot_AfficheDetails(QModelIndex,QModelIndex)));
        wdg_bigtable->selectRow(0);
    }
}

bool dlg_GestionLieux::ValidationFiche()
{
    QString Msg = tr("Vous n'avez pas spécifié ");
    if (wdg_nomledit->text() == QString() || wdg_nomledit->text() == tr("non défini"))
    {
        Msg += tr("le nom de la structure de soins");
        wdg_nomledit->setFocus();
        UpMessageBox::Watch(this,Msg);
        return false;
    }
    if (wdg_cpledit->text().toInt() ==  0)
    {
        Msg += tr("le code postal");
        wdg_cpledit->setFocus();
        UpMessageBox::Watch(this,Msg);
        return false;
    }
    if (wdg_villeledit->text() == QString())
    {
        Msg += tr("la ville");
        wdg_villeledit->setFocus();
        UpMessageBox::Watch(this,Msg);
        return false;
    }
    return true;
}

void dlg_GestionLieux::ReconstruitModel()
{
    m_model = new QStandardItemModel();

    QStandardItem *pitem0;
    QStandardItem *pitem1;
    QStandardItem *pitem2;
    QStandardItem *pitem3;
    QStandardItem *pitem4;
    QStandardItem *pitem5;
    QStandardItem *pitem6;
    QStandardItem *pitem7;
    QStandardItem *pitem8;

    QList<QVariantList> listlieux = db->StandardSelectSQL("select idLieu, NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, LieuCodePostal, LieuVille, LieuTelephone, LieuFax from " TBL_LIEUXEXERCICE, m_ok);
    for (int i=0; i<listlieux.size(); i++)
    {
        pitem0 = new QStandardItem(listlieux.at(i).at(0).toString());
        pitem1 = new QStandardItem(listlieux.at(i).at(1).toString()==""? tr("non défini") : listlieux.at(i).at(1).toString());
        pitem2 = new QStandardItem(listlieux.at(i).at(2).toString());
        pitem3 = new QStandardItem(listlieux.at(i).at(3).toString());
        pitem4 = new QStandardItem(listlieux.at(i).at(4).toString());
        pitem5 = new QStandardItem(listlieux.at(i).at(5).toString());
        pitem6 = new QStandardItem(listlieux.at(i).at(6).toString());
        pitem7 = new QStandardItem(listlieux.at(i).at(7).toString());
        pitem8 = new QStandardItem(listlieux.at(i).at(8).toString());
        m_model->appendRow(QList<QStandardItem*>() << pitem0 << pitem1 << pitem2 << pitem3 << pitem4 << pitem5 << pitem6 << pitem7 << pitem8);
    }

    wdg_bigtable->setModel(m_model);

    m_model->setHeaderData(0, Qt::Horizontal, tr("idLieu"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Structure de soins"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Adresse1"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("Adresse2"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("Adresse3"));
    m_model->setHeaderData(5, Qt::Horizontal, tr("Code postal"));
    m_model->setHeaderData(6, Qt::Horizontal, tr("Ville"));
    m_model->setHeaderData(7, Qt::Horizontal, tr("Telephone"));
    m_model->setHeaderData(8, Qt::Horizontal, tr("Fax"));

    wdg_bigtable->setColumnWidth(0,1);         // idLieu
    wdg_bigtable->setColumnWidth(1,240);       // NomLieu
    wdg_bigtable->setColumnWidth(2,240);       // Adresse1
    wdg_bigtable->setColumnWidth(3,240);       // Adresse2
    wdg_bigtable->setColumnWidth(4,240);       // Adresse3
    wdg_bigtable->setColumnWidth(5,90);        // CP
    wdg_bigtable->setColumnWidth(6,240);       // Ville
    wdg_bigtable->setColumnWidth(7,120);       // Telephone
    wdg_bigtable->setColumnWidth(8,120);       // Fax
    wdg_bigtable->hideColumn(0);
    for (int i=2; i<9; i++)
        wdg_bigtable->setColumnHidden(i, true);
    wdg_bigtable->verticalHeader()->setVisible(false);
    wdg_bigtable->setFocusPolicy(Qt::StrongFocus);
    wdg_bigtable->setGridStyle(Qt::NoPen);
    wdg_bigtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    wdg_bigtable->setSelectionMode(QAbstractItemView::SingleSelection);
    wdg_bigtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    wdg_bigtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    wdg_bigtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    wdg_bigtable->setStyleSheet("QTableView {selection-color: rgb(0,0,0); selection-background-color: rgb(164, 205, 255);}");

    int larg = 0;
    for (int i=0; i < m_model->columnCount(); i++)
        if (!wdg_bigtable->isColumnHidden(i))
            larg += wdg_bigtable->columnWidth(i);
    wdg_bigtable   ->setFixedWidth(larg+2);
    int h = int(QFontMetrics(qApp->font()).height()*1.1);
    for (int i=0; i < m_model->rowCount(); i++)
        wdg_bigtable->setRowHeight(i, h);
    wdg_bigtable   ->setFixedWidth(larg+2);
    m_idlieuserveur = -1;
    m_idlieuserveur = db->parametres()->idlieupardefaut();
}

