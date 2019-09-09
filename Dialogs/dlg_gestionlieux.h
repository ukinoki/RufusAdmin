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

#ifndef DLG_GESTIONLIEUX_H
#define DLG_GESTIONLIEUX_H

#include <QApplication>
#include <QDir>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTableView>
#include <QSqlQuery>
#include "dlg_message.h"
#include "database.h"
#include "macros.h"
#include "updialog.h"
#include "uplabel.h"
#include "uplineedit.h"
#include "upmessagebox.h"
#include "widgetbuttonframe.h"

/* sert à gérer les lieux d'exercice
 * IDENTIQUE POUR RUFUS ET RUFUSADMIN*/

class dlg_GestionLieux : public UpDialog
{
    Q_OBJECT

public:
    explicit dlg_GestionLieux(QWidget *parent = Q_NULLPTR);
    ~dlg_GestionLieux();
private slots:
    void                    Slot_AfficheDetails(QModelIndex, QModelIndex);

private:
    DataBase                *db;
    QTableView              *wdg_bigtable;
    UpDialog                *dlg_lieu;
    UpLabel                 *wdg_adresselbl;
    UpLineEdit              *wdg_nomledit;
    UpLineEdit              *wdg_adress1ledit;
    UpLineEdit              *wdg_adress2ledit;
    UpLineEdit              *wdg_adress3ledit;
    UpLineEdit              *wdg_cpledit;
    UpLineEdit              *wdg_villeledit;
    UpLineEdit              *wdg_telledit;
    UpLineEdit              *wdg_faxledit;
    QStandardItemModel      *m_model;
    WidgetButtonFrame       *wdg_buttonframe;
    int                     m_idlieuamodifier;
    int                     m_idlieuserveur;
    bool                    m_ok;
    void                    ChoixButtonFrame();
    void                    CreerLieu();
    void                    enregLieux();
    void                    enregNouvLieu();
    void                    enregModifLieu();
    void                    ModifLieu();
    void                    SupprLieu();
    void                    ModifLieuxDialog();
    void                    ReconstruitModel();
    bool                    ValidationFiche();
};

#endif // DLG_GESTIONLIEUX_H
