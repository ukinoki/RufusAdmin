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

#ifndef DLG_GESTIONCOMPTES_H
#define DLG_GESTIONCOMPTES_H

#include "dlg_gestionbanques.h"
#include "ui_dlg_gestionbanques.h"

/* sert à gérer les comptes bancaires des utilisateurs
 * IDENTIQUE POUR RUFUS ET RUFUSADMIN*/

namespace Ui {
class dlg_gestioncomptes;
}

class dlg_gestioncomptes : public UpDialog
{

public:
    explicit dlg_gestioncomptes(User *DataUser, QWidget *parent = Q_NULLPTR);
    ~dlg_gestioncomptes();
    enum Mode              {Norm, Modif, Nouv};

private:
    Ui::dlg_gestioncomptes  *ui;
    DataBase                *db;
    User                    *m_userencours;
    QList<int>              m_listescomptesusr;
    Compte                  *m_compteencours;
    bool                    m_visible;
    Mode                    m_mode;
    int                     m_idcomptepardefaut;
    QString                 m_userlogin;
    QTimer                  *t_timer;
    void                    closeEvent(QCloseEvent *);
    void                    MetAJourListeBanques();
    void                    RemplirTableView(int idcompte = -1);
    bool                    VerifCompte();
    WidgetButtonFrame       *wdg_buttonframe;
    UpSmallButton           *wdg_NouvBanqupPushButton;
    void                    ModifCompte();
    void                    NouvCompte();
    void                    SupprCompte();

    void                    AfficheCompte(QTableWidgetItem *, QTableWidgetItem *);
    void                    AnnulModif();
    void                    Banques();
    void                    ChoixButtonFrame();
    void                    Clign();
    void                    CompteFactice();
    void                    DesactiveCompte();
    void                    Fermer();
    void                    ValidCompte();
};

#endif // DLG_GSETIONCOMPTES_H
