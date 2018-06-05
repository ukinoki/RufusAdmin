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

#ifndef FUNCTORMESSAGE_H
#define FUNCTORMESSAGE_H

#include <QDialog>
#include "uplabel.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>
#include <QDialog>
#include <QScreen>

#include "uptextedit.h"

class FunctorMessage
{
public:
    void operator()(QStringList listmess, int pause = 1000, bool bottom = true);
};

#endif // FUNCTORMESSAGE_H
