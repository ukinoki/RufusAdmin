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

#ifndef DLG_GESTIONUSERS_H
#define DLG_GESTIONUSERS_H

#include <QDesktopWidget>
#include <QInputDialog>
#include <QNetworkInterface>
#include <QTableWidgetItem>

#include "dlg_gestioncomptes.h"
#include "dlg_listelieux.h"
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
    enum                    UserMode {PREMIERUSER, ADMIN, MODIFUSER};     Q_ENUM(UserMode)
    /* correspond aux 3 façons dont la fiche a été appelée
     * PREMIERUSER -> la fiche est appelée au premier lancement du programme pour créer le premier utilisateur -> on peut tout modifier
     * MODIFUSER   -> appelé par l'utilisateur dans le premier onglet de la fiche dlg_param -> on ne peut modifier que les données d'identité, geographiques et bancaires
     * ADMIN       -> appelé par l'administrateur, on peut tout modidier, y compris le statut, de chaque utilisateur, sauf les données bancaires
    */
    explicit dlg_gestionusers(int idlieu, UserMode  mode, bool mdpverified=true, QWidget *parent = Q_NULLPTR);
    /*
     * la variable mdpverified est utilisée pour l'appel de la fiche dlg_gestionlieux
     * Cette fiche est parfois appelée alors que le mdp administrateur a déjà eté vérifié, parfois non
     */
    ~dlg_gestionusers();
    Ui::dlg_gestionusers    *ui;
    enum                    Mode {Creer, Modifier, PremierUsr};
    void                    setConfig(UserMode mode);
    bool                    isMDPverified();

private:
    DataBase                *db;
    bool                    m_MDPverified;
    UserMode                m_usermode;
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

    bool                    m_ok;

    UpDialog                *dlg_ask;
    QBrush                  m_color;
    Mode                    m_mode;
    int                     m_idlieu;
    User                    *m_userencours;
    QString                 m_loginledit, m_MDPledit, m_confirmMDPledit;
    QString                 m_nouvMDP, m_ancMDP, m_confirmMDP;
    QStringList             gListBanques;
    UpDialog                *gAskMDP;
    void                    ActualiseRsgnmtBanque(bool soccomptable);
    void                    Annulation();
    bool                    AfficheParamUser(int idUser);
    void                    CalcListitemsCompteActescomboBox(User *usr);
    void                    CalcListitemsCompteComptacomboBox(User *usr, bool soccomptable);
    void                    CalcListitemsEmployeurcomboBox(int iduser);
    void                    DefinitLesVariables();
    void                    EnregistreNouvMDP();
    void                    EnregistreNouvUser();
    void                    EnregistreUser();
    bool                    ExisteEmployeur(int iduser);
    void                    FermeFiche();
    void                    GestionComptes();
    void                    GestionLieux();
    void                    Inactifs();
    void                    ModifMDP();
    void                    RegleAffichage();
    void                    RemplirTableWidget();
    void                    ReconstruitListeLieuxExercice();
    bool                    VerifFiche();

    void                    setDataCurrentUser(int id);
    WidgetButtonFrame       *wdg_buttonframe;
    void                    ChoixButtonFrame();
    void                    CreerUser();
    void                    ModifUser();
    void                    SupprUser();
};

#endif // DLG_gestionusers_H
