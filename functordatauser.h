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

#ifndef FUNCTORDATAUSER_H
#define FUNCTORDATAUSER_H

#include <QtSql>
#include <QSqlDatabase>
#include "upmessagebox.h"

class FunctorDataUser
{
public:
    QMap<QString,QVariant> operator()(int iduser, int idLieu, QSqlDatabase db);
private:
    QMap<QString,QVariant>  DataUser;
};

#endif // FUNCTORDATAUSER_H
