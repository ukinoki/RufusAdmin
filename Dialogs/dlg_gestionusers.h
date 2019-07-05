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

#ifndef DLG_GESTIONUSERS_H
#define DLG_GESTIONUSERS_H

#include <QDesktopWidget>
#include <QInputDialog>
#include <QNetworkInterface>
#include <QTableWidgetItem>
#include <QtSql>
#include <QSqlDatabase>

#include "database.h"
#include "dlg_gestioncomptes.h"
#include "dlg_gestionlieux.h"
#include "uplineedit.h"
#include "uptextedit.h"
#include "widgetbuttonframe.h"
#include "macros.h"
#include "icons.h"
#include "utils.h"
#include "cls_user.h"
#include "upheaderview.h"

/* sert à gérer les comptes utilisateurs
 * QUASI-IDENTIQUE POUR RUFUS ET RUFUSADMIN*/

namespace Ui {
class dlg_gestionusers;
}

class dlg_gestionusers : public UpDialog
{
    Q_OBJECT

public:
    explicit dlg_gestionusers(int idUser, int idlieu, bool mdpverified=true, QWidget *parent = Q_NULLPTR);
    /*
     * la variable mdpverified est utilisée pour l'appel de la fiche dlg_gestionlieux
     * Cette fiche est parfois appelée alors que le mdp administrateur a déjà eté vérifié, parfois non
     */
    ~dlg_gestionusers();
    Ui::dlg_gestionusers    *ui;
    int                     Mode;
    enum                    Mode {PREMIERUSER, ADMIN, MODIFUSER};
    /* correspond aux 3 modes d'utilisation de la fiche
     * PREMIERUSER -> la fiche est appelée au premier lancement du programme pour créer le premier utilisateur -> on peut tout modifier
     * MODIFUSER   -> appelé par l'utilisateur dans le premier onglet de la fiche dlg_param -> on ne peut modifier que les données d'identité, geographiques et bancaires
     * ADMIN       -> appelé par l'administrateur, on peut tout modidier, y compris le statut
    */
    void                    setConfig(enum Mode);
    bool                    isMDPverified();

private:
    bool                    MDPverified;
    bool                    ok;

    bool                    ophtalmo;
    bool                    orthoptist;
    bool                    autresoignant;
    bool                    medecin;
    bool                    soccomptable;
    bool                    assistant;
    bool                    liberal;
    bool                    pasliberal;
    bool                    retrocession;
    bool                    cotation;
    bool                    soignant;
    bool                    responsable;
    bool                    respsalarie;
    bool                    respliberal;
    bool                    soigntnonrplct;

    dlg_gestioncomptes      *Dlg_GestComptes;
    UpDialog                *gAsk;
    DataBase                *db;
    QBrush                  gcolor;
    int                     gMode;
        enum gMode          {Creer, Modifier, PremierUsr};
    int                     gidUserDepart;
    int                     gidLieu;
    User                    *OtherUser;
    QString                 gLoginupLineEdit, gMDPupLineEdit, gConfirmMDPupLineEdit;
    QString                 gLibActiv, gNoLibActiv;
    QString                 gNouvMDP, gAncMDP, gConfirmMDP;
    QStringList             gListBanques;
    UpDialog                *gAskMDP;
    void                    ActualiseRsgnmtBanque(bool soccomptable);
    bool                    AfficheParamUser(int idUser);
    void                    CalcListitemsCompteActescomboBox(int iduser);
    void                    CalcListitemsCompteComptacomboBox(int iduser, bool soccomptable);
    void                    CalcListitemsEmployeurcomboBox(int iduser);
    void                    DefinitLesVariables();
    bool                    ExisteEmployeur(int iduser);
    void                    RemplirTableWidget(int iduser);
    void                    ReconstruitListeLieuxExercice();
    bool                    VerifFiche();

    void                    setDataUser(int id);
    WidgetButtonFrame       *widgButtons;
    void                    CreerUser();
    void                    ModifUser();
    void                    SupprUser();

private slots:
    void                    Slot_Annulation();
    void                    Slot_EnableOKpushButton();
    void                    Slot_EnregistreNouvMDP();
    void                    Slot_EnregistreUser();
    void                    Slot_FermeFiche();
    void                    Slot_GestionComptes();
    void                    Slot_GestLieux();
    void                    Slot_ModifMDP();
    void                    Slot_RegleAffichage();
    void                    Slot_EnregistreNouvUser();
    void                    Slot_ChoixButtonFrame(int);
};

#endif // DLG_gestionusers_H
