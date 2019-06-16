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


#include "cls_depenses.h"
#include <QDebug>

/*
 * GETTER
*/
QMap<int, Depense *> *Depenses::depenses() const
{
    return m_Depenses;
}


/*!
 * \brief Depenses::Depenses
 * Initialise la map Depenses
 */
Depenses::Depenses(QObject *parent) : ItemsList(parent)
{
    m_Depenses = new QMap<int, Depense*>();
}

void Depenses::addList(QList<Depense*> listDepense)
{
    QList<Depense*>::const_iterator it;
    for( it = listDepense.constBegin(); it != listDepense.constEnd(); ++it )
    {
        Depense* item = const_cast<Depense*>(*it);
        add( m_Depenses, item->id(), item );
    }
}

/*!
 * \brief Depenses::getById
 * \param id l'id du Depense recherché
 * \return Q_NULLPTR si aucune Depense trouvée
 * \return Depense* le Depense Depense à l'id
 */
Depense* Depenses::getById(int id)
{
    QMap<int, Depense*>::const_iterator Depense = m_Depenses->find(id);
    if( Depense == m_Depenses->constEnd() )
        return Q_NULLPTR;
    return Depense.value();
}

/*!
 * \brief Depenseses::initListeByUser
 * Charge l'ensemble des cotations pour le user
 * et les ajoute à la classe Correspondants
 */
void Depenses::initListeByUser(int iduser)
{
    clearAll(m_Depenses);
    QList<Depense*> listdepenses = DataBase::I()->loadDepensesByUser(iduser);
    addList(listdepenses);
}

void Depenses::SupprimeDepense(Depense *dep)
{
    if (dep == Q_NULLPTR)
        return;
    DataBase::I()->SupprRecordFromTable(dep->id(), CP_IDDEPENSE_DEPENSES, TBL_DEPENSES);
    remove(m_Depenses, dep);
}
Depense* Depenses::CreationDepense(int idUser, QDate DateDep, QString RefFiscale, QString Objet, double Montant, QString FamFiscale,
                                   QString Monnaie, int idRec, QString ModePaiement, int Compte, int Nocheque, int  idFacture)
{
    Depense *dep = Q_NULLPTR;
    bool ok;
    QString idusr           = (idUser == 0?                 "null" : QString::number(idUser));
    QString date            = (DateDep.isValid()?           "NOW()" : "'" + DateDep.toString("yyyy-MM-dd") + "'");
    QString ref             = (RefFiscale == ""?            "" : "'" + Utils::correctquoteSQL(RefFiscale) + "'");
    QString objet           = (Objet == ""?                 "" : "'" + Utils::correctquoteSQL(Objet) + "'");
    QString montant         = QString::number(Montant, 'f',2);
    QString fam             = (FamFiscale == ""?            "null" : "'" + Utils::correctquoteSQL(FamFiscale) + "'");
    QString monnaie         = (Monnaie == ""?               "null" : "'" + Utils::correctquoteSQL(Monnaie) + "'");
    QString idrec           = (idRec == 0?                  "null" : QString::number(idRec));
    QString modpaiement     = (ModePaiement == ""?          "null" : "'" + Utils::correctquoteSQL(ModePaiement) + "'");
    QString idcpt           = (Compte == 0?                 "null" : QString::number(Compte));
    QString nochq           = (Nocheque == 0?               "null" : QString::number(Nocheque));
    QString idfacture       = (idFacture == 0?              "null" : QString::number(idFacture));
    QString req =     "INSERT INTO " TBL_DEPENSES
            " (idUser, DateDep, RefFiscale, Objet, Montant, FamFiscale,"
            " Monnaie, idRec, ModePaiement, Compte, Nocheque, idFacture)"
            " VALUES (" +   idusr + "," +
            date + "," +
            ref + "," +
            objet   + "," +
            montant   + "," +
            fam   + "," +
            monnaie + "," +
            idrec   + "," +
            modpaiement   + "," +
            idcpt   + "," +
            nochq   + "," +
            idfacture + ")";
    //qDebug() << req;
    QString MsgErreur           = tr("Impossible d'enregistrer cette dépense");
    DataBase::I()->locktables(QStringList() << TBL_DEPENSES);
    if (!DataBase::I()->StandardSQL(req, MsgErreur))
    {
        DataBase::I()->unlocktables();
        return Q_NULLPTR;
    }
    // Récupération de l'idMotif créé ------------------------------------
    int iddep = DataBase::I()->selectMaxFromTable(CP_IDDEPENSE_DEPENSES, TBL_DEPENSES, ok, tr("Impossible de sélectionner les enregistrements"));
    DataBase::I()->unlocktables();
    dep = new Depense();
    dep->setid(iddep);
    dep->setiduser(idUser);
    dep->setdate(DateDep);
    dep->setrubriquefiscale(RefFiscale);
    dep->setobjet(Objet);
    dep->setmontant(Montant);
    dep->setfamillefiscale(FamFiscale);
    dep->setmonnaie(Monnaie);
    dep->setidrecette(idRec);
    dep->setmodepaiement(ModePaiement);
    dep->setidcomptebancaire(Compte);
    dep->setnocheque(Nocheque);
    dep->setidfacture(idFacture);
    add(m_Depenses, dep->id(), dep);
    return dep;
}
