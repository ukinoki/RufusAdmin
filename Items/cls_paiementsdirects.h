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

#ifndef CLS_PAIEMENTSDIRECTS_H
#define CLS_PAIEMENTSDIRECTS_H

#include <QObject>
#include "cls_paiementdirect.h"

/*!
 * \brief Cette classe gére la liste des paiements directs
 *  ( = actes dont le paiement est en attente d'être enregistré )
 * = tous les actes effectués pour lequel il faut encore enregistré le moyen de paiement:
 *  . paiement direct proprement dit
 *  . ou paiement par tiers
 */

class PaiementsDirects
{
public:
    explicit PaiementsDirects();
};

#endif // CLS_PAIEMENTSDIRECTS_H
