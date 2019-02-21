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

#include "uptoolbar.h"
#include "icons.h"

UpToolBar::UpToolBar(bool AvecFinDebut, bool AvecReload, QWidget *parent) : QToolBar(parent)
{
    if (AvecFinDebut)
    {
        debut   = new QAction(Icons::icPageAvant(),tr("Début"),this);
        fin     = new QAction(Icons::icPageApres(),tr("Fin"),this);
        connect(debut,  &QAction::triggered,  [=] {TBChoix(debut);});
        connect(fin,    &QAction::triggered,  [=] {TBChoix(fin);});
    }
    prec    = new QAction(Icons::icAvant(),tr("Précédent"),this);
    suiv    = new QAction(Icons::icApres(),tr("Suivant"),this);

    connect(prec,   &QAction::triggered,  [=] {TBChoix(prec);});
    connect(suiv,   &QAction::triggered,  [=] {TBChoix(suiv);});

    if (AvecFinDebut)
        addAction(debut);
    addAction(prec);
    addAction(suiv);
    if (AvecFinDebut)
        addAction(fin);

    if (AvecReload)
    {
        reload  = new QAction(Icons::icPageRefresh(),tr("Recharger"),this);
        connect(reload, &QAction::triggered,  [=] {TBChoix(reload);});
        addSeparator();
        addAction(reload);
    }

    setFixedHeight(46);
    setIconSize(QSize(35,35));
}

UpToolBar::~UpToolBar()
{
}

void UpToolBar::TBChoix(QAction *choix)
{
    action = choix->text();
    emit TBSignal();
}

QAction* UpToolBar::First()
{
    return debut;
}

QAction* UpToolBar::Last()
{
    return fin;
}

QAction* UpToolBar::Next()
{
    return suiv;
}

QAction* UpToolBar::Prec()
{
    return prec;
}

QAction* UpToolBar::Reload()
{
    return reload;
}


