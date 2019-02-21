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

#include "cls_docexterne.h"
#include <QDebug>

DocExterne::DocExterne(QJsonObject data, QObject *parent) : Item(parent)
{
    setData(data);
}

bool DocExterne::isAllLoaded() const                { return m_isAllLoaded;}

int DocExterne::id() const                          { return m_idimpression;}
int DocExterne::iduser() const                      { return m_iduser;}
int DocExterne::idpatient() const                   { return m_idpatient;}
QString DocExterne::typedoc() const                 { return m_typedoc;}
QString DocExterne::soustypedoc() const             { return m_soustypedoc;}
QString DocExterne::titre() const                   { return m_titre;}

QString DocExterne::textentete() const              { return m_textentete;}
QString DocExterne::textcorps() const               { return m_textcorps;}
QString DocExterne::textorigine() const             { return m_textorigine;}
QString DocExterne::textpied() const                { return m_textpied;}
QDateTime DocExterne::date() const                  { return m_dateimpression;}

int DocExterne::compression() const                 { return m_compression;}
QString DocExterne::lienversfichier() const         { return m_lienversfichier;}
bool DocExterne::isALD() const                      { return m_ald;}
int DocExterne::useremetteur() const                { return m_useremetteur;}
QString DocExterne::format() const                  { return m_formatdoc;}

int DocExterne::importance() const                  { return m_importance;}

void DocExterne::setimportance(int imptce)          { m_importance = imptce;}
void DocExterne::setAllLoaded(bool AllLoaded)       { m_isAllLoaded = AllLoaded;}

void DocExterne::setData(QJsonObject data)
{
    if( data.isEmpty() )
        return;
    setDataBool(data, "isallloaded", m_isAllLoaded);

    setDataInt(data, "id", m_idimpression);
    setDataInt(data, "iduser", m_iduser);
    setDataInt(data, "idpat", m_idpatient);
    setDataString(data, "typedoc", m_typedoc);
    setDataString(data, "soustypedoc", m_soustypedoc);

    setDataString(data, "titre", m_titre);
    setDataString(data, "textentete", m_textentete);
    setDataString(data, "textcorps", m_textcorps);
    setDataString(data, "textorigine", m_textorigine);
    setDataString(data, "textpied", m_textpied);

    setDataDateTime(data, "dateimpression", m_dateimpression);
    setDataInt(data, "compression", m_compression);
    setDataString(data, "lienversfichier", m_lienversfichier);
    setDataBool(data, "ALD", m_ald);
    setDataInt(data, "useremetteur", m_useremetteur);

    setDataString(data, "formatdoc", m_formatdoc);
    setDataInt(data, "importance", m_importance);
}

