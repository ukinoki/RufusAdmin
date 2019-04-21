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

Motifs::Motifs()
{
    m_motifs = new QMap<int, Motif*>();
}

QMap<int, Motif *> *Motifs::motifs() const
{
    return m_motifs;
}

void Motifs::add(Motif *Motif)
{
    if( m_motifs->contains(Motif->id()) )
        return;
    m_motifs->insert(Motif->id(), Motif);
}

void Motifs::addList(QList<Motif*> listMotifs)
{
    QList<Motif*>::const_iterator it;
    for( it = listMotifs.constBegin(); it != listMotifs.constEnd(); ++it )
        add( *it );
}

Motif* Motifs::getById(int id)
{
    QMap<int, Motif*>::const_iterator itcpt = m_motifs->find(id);
    if( itcpt == m_motifs->constEnd() )
        return Q_NULLPTR;
    return itcpt.value();
}

void Motifs::clearAll()
{
    QList<Motif*> listmotifs;
    for( QMap<int, Motif*>::const_iterator itmtf = m_motifs->constBegin(); itmtf != m_motifs->constEnd(); ++itmtf)
        delete itmtf.value();
    m_motifs->clear();
}

void Motifs::remove(Motif *motif)
{
    if (motif == Q_NULLPTR)
        return;
    QMap<int, Motif*>::const_iterator itmotf = m_motifs->find(motif->id());
    if( itmotf == m_motifs->constEnd() )
        return;
    m_motifs->remove(motif->id());
    delete motif;
}

/*!
 * \brief Motifs::initListe
 * Charge l'ensemble des motifs
 * et les ajoute à la classe Motifss
 */
void Motifs::initListe()
{
    clearAll();
    QList<Motif*> listmotifs = DataBase::getInstance()->loadMotifs();
    QList<Motif*>::const_iterator itmtf;
    for( itmtf = listmotifs.constBegin(); itmtf != listmotifs.constEnd(); ++itmtf )
    {
        Motif *mtf = const_cast<Motif*>(*itmtf);
        add(mtf);
    }
}

