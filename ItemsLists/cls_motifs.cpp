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

#include "cls_motifs.h"

Motifs::Motifs(QObject *parent) : ItemsList(parent)
{
    map_motifs = new QMap<int, Motif*>();
}

QMap<int, Motif *> *Motifs::motifs() const
{
    return map_motifs;
}

Motif* Motifs::getById(int id)
{
    QMap<int, Motif*>::const_iterator itcpt = map_motifs->find(id);
    if( itcpt == map_motifs->constEnd() )
        return Q_NULLPTR;
    return itcpt.value();
}

Motif* Motifs::getMotifFromRaccourci(QString txt)
{
    foreach (Motif* mtf, motifs()->values())
        if (mtf->raccourci() == txt)
            return mtf;
    return Q_NULLPTR;
}

/*!
 * \brief Motifs::initListe
 * Charge l'ensemble des motifs
 * et les ajoute à la classe Motifss
 */
void Motifs::initListe()
{
    QList<Motif*> listmotifs = DataBase::I()->loadMotifs();
    epurelist(map_motifs, &listmotifs);
    addList(map_motifs, &listmotifs);
}

void Motifs::SupprimeMotif(Motif *mf)
{
    if (mf == Q_NULLPTR)
        return;
    DataBase::I()->SupprRecordFromTable(mf->id(), "idMotifsRDV", TBL_MOTIFSRDV);
    remove(map_motifs, mf);
}

Motif*  Motifs::CreationMotif(QString Motf, QString Raccourci, QString Couleur, int Duree, bool ParDefaut, bool Utiliser, int NoOrdre)
{
    Motif *motf = Q_NULLPTR;
    bool ok;
    QString motif           = (Motf == ""?              "null" : "'" + Utils::correctquoteSQL(Motf) + "'");
    QString raccourci       = (Raccourci == ""?         "null" : "'" + Utils::correctquoteSQL(Raccourci) + "'");
    QString couleur         = (Couleur == ""?           "null" : "'" + Utils::correctquoteSQL(Couleur) + "'");
    QString duree           = (Duree == 0?              "null" : QString::number(Duree));
    QString pardefaut       = (!ParDefaut?              "null" : "1");
    QString utiliser        = (!Utiliser?               "null" : "1");
    QString noordre         = (NoOrdre == 0?            "null" : QString::number(NoOrdre));
    QString req =     "INSERT INTO " TBL_MOTIFSRDV
                        " (Motif,Raccourci, Couleur, Duree, ParDefaut, Utiliser, NoOrdre)"
                        " VALUES (" +   motif + "," +
                                        raccourci + "," +
                                        couleur + "," +
                                        duree   + "," +
                                        pardefaut   + "," +
                                        utiliser   + "," +
                                        noordre + ")";
    QString MsgErreur           = tr("Impossible de créer ce motif");
    DataBase::I()->locktables(QStringList() << TBL_MOTIFSRDV);
    if (!DataBase::I()->StandardSQL(req, MsgErreur))
    {
        DataBase::I()->unlocktables();
        return Q_NULLPTR;
    }
    // Récupération de l'idMotif créé ------------------------------------
    int idmotif = DataBase::I()->selectMaxFromTable("idMotifsRDV", TBL_MOTIFSRDV, ok, tr("Impossible de sélectionner les enregistrements"));
    DataBase::I()->unlocktables();
    QJsonObject jmotif{};
    jmotif["id"] = idmotif;
    jmotif["motif"] = Motf;
    jmotif["raccourci"] = Raccourci;
    jmotif["couleur"] = Couleur;
    jmotif["duree"] = Duree;
    jmotif["pardefaut"] = ParDefaut;
    jmotif["utiliser"] = Utiliser;
    jmotif["noordre"] = NoOrdre;
    motf = new Motif(jmotif);
    add(map_motifs, motf);
    return motf;
}
