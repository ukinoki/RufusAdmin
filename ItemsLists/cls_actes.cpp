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

#include "cls_actes.h"

Actes::Actes(QObject *parent) : ItemsList(parent)
{
    m_actes = new QMap<int, Acte*>();
}

QMap<int, Acte *> *Actes::actes() const
{
    return m_actes;
}

/*!
 * \brief Actes::initListe
 * Charge l'ensemble des actes
 * et les ajoute à la classe Actes
 */
void Actes::initListeByPatient(Patient *pat, Item::UPDATE upd, bool quelesid)
{
    if (upd == Item::NoUpdate)
        clearAll(m_actes);
    QList<Acte*> listActes;
    if (quelesid)
        listActes = DataBase::I()->loadIdActesByPat(pat);
    else
        listActes = DataBase::I()->loadActesByPat(pat);
    addList(listActes, upd);
}

void Actes::addList(QList<Acte*> listActes, Item::UPDATE upd)
{
    QList<Acte*>::const_iterator it;
    for( it = listActes.constBegin(); it != listActes.constEnd(); ++it )
    {
        Acte* item = const_cast<Acte*>(*it);
        add( m_actes, item->id(), item, upd );
    }
}

void Actes::sortActesByDate()  /*! cette fonction et les 2 qui suivent ne sont pour l'instant pas utilisées.
                                 * elles sont prévues pour réorganiser le tri des actes en fonction de leur date et pas en fonction de leur id
                                 * parce qu'il arrive (rarement) qu'on saisisse un acte a posteriori dont la date sera antérieure à celle du dernier acte
                                 * si on continue à défiler par id, cet acte n'apparaîtra pas en ordre chronologique mais en dernier. */
{
    // toute la manip qui suit sert à remetre les acteses par ordre chronologique - si vous trouvez plus simple, ne vous génez pas
    if (m_actesmodel == Q_NULLPTR)
        m_actesmodel = new QStandardItemModel();
    else
        m_actesmodel->clear();
    for (QMap<int, Acte*>::const_iterator itact = m_actes->constBegin(); itact != m_actes->constEnd(); ++itact)
    {
        QList<QStandardItem *> items;
        Acte* act = const_cast<Acte*>(itact.value());
        UpStandardItem *itemact = new UpStandardItem(QString::number(act->id()));
        itemact->setItem(act);
        items << new UpStandardItem(act->date().toString("yyyymmss"))
              << new UpStandardItem(act->heure().toString("HHmm"))
              << itemact;
        m_actesmodel->appendRow(items);
    }
    m_heuresortmodel = new QSortFilterProxyModel();
    m_heuresortmodel->setSourceModel(m_actesmodel);
    m_heuresortmodel->sort(1);

    m_actesortmodel = new QSortFilterProxyModel();
    m_actesortmodel->setSourceModel(m_heuresortmodel);
    m_actesortmodel->sort(0);
}

Acte* Actes::getActeFromRow(int row)
{
    QModelIndex psortindx = m_actesortmodel->index(row, 2);
    return getActeFromIndex(psortindx);
}

Acte* Actes::getActeFromIndex(QModelIndex idx)
{
    QModelIndex heureindx   = m_actesortmodel->mapToSource(idx);                      //  -> m_heuresortmodel
    QModelIndex pindx       = m_heuresortmodel->mapToSource(heureindx);               //  -> m_actesmodel

    UpStandardItem *item = dynamic_cast<UpStandardItem *>(m_actesmodel->itemFromIndex(pindx));
    if (item == Q_NULLPTR)
        return Q_NULLPTR;
    if (item->item() == Q_NULLPTR)
    {
        qDebug() << "erreur sur l'item - row = " << item->row() << " - col = " << item->column() << item->text();
        return Q_NULLPTR;
    }
    Acte *act = dynamic_cast<Acte *>(item->item());
    return act;
}

Acte* Actes::getById(int id, ADDTOLIST add)
{
    QMap<int, Acte*>::const_iterator itact = m_actes->find(id);
    if( itact == m_actes->constEnd() )
    {
        Acte * act = Q_NULLPTR;
        if (add == AddToList)
            act = DataBase::I()->loadActeById(id);
        return act;
    }
    return itact.value();
}

QMap<int, Acte*>::const_iterator Actes::getLast()
{
    return actes()->find(actes()->lastKey());
}

QMap<int, Acte*>::const_iterator Actes::getAt(int idx)
{
    return actes()->find( actes()->keys().at(idx) );
}

void Actes::updateActe(Acte* acte)
{
    acte->setData(DataBase::I()->loadActeAllData(acte->id()));
}

void Actes::setMontantCotation(Acte *act, QString Cotation, double montant)
{
    if ( act == Q_NULLPTR )
        return;
    //on modifie la table Actes avec la nouvelle cotation
    QString cotsql = Cotation;
    if (cotsql == "")
    {
        cotsql = "null";
        montant = 0.00;
    }
    else
        cotsql = "'" + Utils::correctquoteSQL(Cotation) + "'";
    QString requete = "UPDATE " TBL_ACTES
                      " SET ActeCotation = " + cotsql +
                      ", ActeMontant = " + QString::number(montant) +
                      " WHERE idActe = " + QString::number(act->id());
    DataBase::I()->StandardSQL(requete);
    act->setcotation(Cotation);
    act->setmontant(montant);
}

void Actes::updateActeData(Acte *act, QString nomchamp, QVariant value)
{
    QString newvalue;
    if (nomchamp == CP_MOTIF_ACTES)
    {
        act->setmotif(value.toString());
        newvalue = (value.toString() != ""? "'" + Utils::correctquoteSQL(value.toString()) + "'" : "null");
    }
    else if (nomchamp == CP_TEXTE_ACTES)
    {
        act->settexte(value.toString());
        newvalue = (value.toString() != ""? "'" + Utils::correctquoteSQL(value.toString()) + "'" : "null");
    }
    else if (nomchamp == CP_CONCLUSION_ACTES)
    {
        act->setconclusion(value.toString());
        newvalue = (value.toString() != ""? "'" + Utils::correctquoteSQL(value.toString()) + "'" : "null");
    }
    else if (nomchamp == CP_ACTEDATE_ACTES)
    {
        act->setdate(value.toDate());
        newvalue = (value.toDate().isValid()? "'" + value.toDate().toString("yyyy-MM-dd") + "'" : "null");
    }
    else if (nomchamp == CP_COURRIERAFAIRE_ACTES)
    {
        act->setcourrierafaire(value.toString()== "T" || value.toString()== "1");
        newvalue = ((value.toString() == "T" || value.toString()== "1")? "'T'" : "null");
    }
    else if (nomchamp == CP_IDUSER_ACTES)
    {
        act->setiduser(value.toInt());
        newvalue = (value.toInt() != 0? value.toString() : "null");

    }
    else if (nomchamp == CP_IDUSERPARENT_ACTES)
    {
        act->setiduserparent(value.toInt());
        newvalue = (value.toInt() != 0? value.toString() : "null");

    }
    else if (nomchamp == CP_IDUSERCOMPTABLE_ACTES)
    {
        act->setidusercomptable(value.toInt());
        newvalue = (value.toInt() != 0? value.toString() : "null");

    }
    QString requete = "UPDATE " TBL_ACTES
                      " SET " + nomchamp + " = " + newvalue +
                      " WHERE idActe = " + QString::number(act->id());
    DataBase::I()->StandardSQL(requete);
}

void Actes::SupprimeActe(Acte* act)
{
    DataBase::I()->StandardSQL("DELETE FROM " TBL_ACTES " WHERE idActe = " + QString::number(act->id()));
    remove(m_actes, act);
}

Acte* Actes::CreationActe(Patient *pat, int idcentre)
{
    if (pat == Q_NULLPTR)
        return Q_NULLPTR;
    Acte *act = Q_NULLPTR;
    bool ok;
    User* usr = DataBase::I()->getUserConnected();
    QString rempla = (usr->getEnregHonoraires()==3? "1" : "null");
    QString creerrequete =
            "INSERT INTO " TBL_ACTES
            " (idPat, idUser, ActeDate, ActeHeure, CreePar, UserComptable, UserParent, SuperViseurRemplacant, NumCentre, idLieu)"
            " VALUES (" +
            QString::number(pat->id()) + ", " +
            QString::number(usr->getIdUserActeSuperviseur()) + ", "
            "NOW(), "
            "NOW(), " +
            QString::number(usr->id()) + ", " +
            QString::number(usr->getIdUserComptable()) + ", " +
            QString::number(usr->getIdUserParent()) + ", " +
            rempla + ", " +
            QString::number(idcentre) + ", " +
            QString::number(usr->getSite()->id()) +")";
    //qDebug() << creerrequete;
    DataBase::I()->locktables(QStringList() << TBL_ACTES);
    if (!DataBase::I()->StandardSQL(creerrequete,tr("Impossible de créer cette consultation dans ") + TBL_ACTES))
    {
        DataBase::I()->unlocktables();
        return Q_NULLPTR;
    }
    int idacte = DataBase::I()->selectMaxFromTable("idActe", TBL_ACTES, ok, tr("Impossible de retrouver l'acte qui vient d'être créé"));
    if (!ok || idacte == 0)
    {
        DataBase::I()->unlocktables();
        return Q_NULLPTR;
    }
    DataBase::I()->unlocktables();
    QJsonObject data{};
    data["id"] = idacte;
    data["idPatient"] = pat->id();
    data["idUser"] = usr->getIdUserActeSuperviseur();
    data["date"] = QDateTime(QDate::currentDate()).toMSecsSinceEpoch();
    data["heure"] = QTime::currentTime().toString("HH:mm:ss");
    data["idCreatedBy"] = usr->id();
    data["idUserComptable"] = usr->getIdUserComptable();
    data["idUserParent"] = usr->getIdUserParent();
    data["remplacant"] = (rempla == "1");
    data["NumCentre"] = idcentre;
    data["idLieu"] = usr->getSite()->id();
    act = new Acte(data);
    add(m_actes, idacte, act);
    return act;
}
