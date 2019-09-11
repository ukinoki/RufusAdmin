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

#ifndef QLABELDELEGATE_H
#define QLABELDELEGATE_H

#include <QStyledItemDelegate>
#include "uplabel.h"

class QLabelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    QLabelDelegate(QObject* parent = Q_NULLPTR);

    QWidget*    createEditor    (QWidget* parent,   const QStyleOptionViewItem& option, const QModelIndex &index) const  Q_DECL_OVERRIDE;
    void        setEditorData   (QWidget* editor,   const QModelIndex& index) const Q_DECL_OVERRIDE;
    bool        editorEvent     (QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)  Q_DECL_OVERRIDE;

signals:
    void        focusitem(int a);
};

#endif // QLABELDELEGATE_H
