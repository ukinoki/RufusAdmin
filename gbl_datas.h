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

#ifndef GBL_DATAS_H
#define GBL_DATAS_H

#include <QObject>
#include "cls_banques.h"
#include "cls_comptes.h"
#include "cls_motifs.h"
#include "cls_users.h"
#include "cls_patientsencours.h"
#include "cls_postesconnectes.h"
#include "cls_sites.h"

class Datas : public QObject
{
    Q_OBJECT
private:
    Datas();
    static Datas *instance;

public:
    static Datas *I();

    Users *users;                       //!< Les users
    Comptes *comptes;                   //!< tous les comptes bancaires
    Banques *banques;                   //!< toutes les banques
    Motifs *motifs;                     //!< tous les motifs d'actes
    PatientsEnCours *patientsencours;   //!> les patients en cours : patients dont les dossiers sont ouverts, ou présents en salle d'attente ou à l'accueil ou en cours d'examen
    PostesConnectes *postesconnectes;   //!> tous les postes connectes
    Sites *sites;                       //!< Les lieux de travail
};

#endif // GBL_DATAS_H
