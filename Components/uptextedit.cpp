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

#include "uptextedit.h"

UpTextEdit::UpTextEdit(QWidget *parent) : QTextEdit(parent)
{
    ValeurAvant = "";
    ValeurApres = "";
    Champ       = "";
    Table       = "";
    id          = -1;
    idUser      = -1;
    installEventFilter(this);
    setContextMenuPolicy(Qt::NoContextMenu);
}

UpTextEdit::UpTextEdit(QString txt, QWidget *parent) : QTextEdit(txt, parent)
{
    ValeurAvant = "";
    ValeurApres = "";
    Champ       = "";
    Table       = "";
    id          = -1;
    idUser      = -1;
    installEventFilter(this);
    setContextMenuPolicy(Qt::NoContextMenu);
}

UpTextEdit::~UpTextEdit()
{
}

bool UpTextEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn )
    {
        UpTextEdit* objUpText = static_cast<UpTextEdit*>(obj);
        objUpText->setValeurAvant(objUpText->toHtml());
    }

    // Ctrl-Return ou Ctrl-Enter ou Ctrl-Tab sur un TextEdit- On ne fait rien - idem avec Shift-------------------------------------------------------------------------
    if (event->type() == QEvent::KeyPress )
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if ((keyEvent->key()==Qt::Key_Return || keyEvent->key() == Qt::Key_Enter || keyEvent->key()==Qt::Key_Tab)
                && (keyEvent->modifiers() == Qt::ShiftModifier || keyEvent->modifiers() == Qt::MetaModifier))
                return true;
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // Ctrl-Return ou Ctrl-Enter ou Ctrl-Tab sur un TextEdit- On va sur la tabulation suivante ---------------------------------------------------------------------------------------------
        if ((keyEvent->key()==Qt::Key_Return || keyEvent->key() == Qt::Key_Enter || keyEvent->key()==Qt::Key_Tab)
                && keyEvent->modifiers() == Qt::MetaModifier)
            {
                UpTextEdit *textw = static_cast<UpTextEdit*>(obj);
                UpTextEdit *textnext = dynamic_cast<UpTextEdit*>(textw->nextInFocusChain());
                if (textnext){
                    textnext->setFocus();
                    textnext->moveCursor(QTextCursor::End);
                }
                else
                    focusNextChild();
                return true;
            }

        // Shift-Return, Shift-Tab ou Shift-Enter - On va au Tab précédent --------------------------------------------------------------------------------------------------------------------
        if ((keyEvent->key()==Qt::Key_Return || keyEvent->key() == Qt::Key_Enter || keyEvent->key()==Qt::Key_Tab)
                && (keyEvent->modifiers() == Qt::ShiftModifier || keyEvent->modifiers() == Qt::ShiftModifier + Qt::MetaModifier))
        {
           {
               UpTextEdit *textw = static_cast<UpTextEdit*>(obj);
               UpTextEdit *textprev = dynamic_cast<UpTextEdit*>(textw->previousInFocusChain());
               if (textprev)
               {
                   textprev->setFocus();
                   textprev->moveCursor(QTextCursor::End);
               }
               else
                   focusNextChild();
               return true;
           }
        }
    }
    return QWidget::eventFilter(obj, event);
}
void UpTextEdit::mouseDoubleClickEvent(QMouseEvent * event )
{
    emit dblclick(getId());
    event->ignore();
}

void UpTextEdit::setId(int idadef)
{
    id = idadef;
}

int UpTextEdit::getId() const
{
    return id;
}

void UpTextEdit::setIdUser(int idadef)
{
    idUser = idadef;
}

int UpTextEdit::getIdUser() const
{
    return idUser;
}

void UpTextEdit::setValeurAvant(QString valprec)
{
    ValeurAvant = valprec;
}

QString UpTextEdit::getValeurAvant() const
{
    return ValeurAvant;
}

void UpTextEdit::setValeurApres(QString valpost)
{
    ValeurApres = valpost;
}

QString UpTextEdit::getValeurApres() const
{
    return ValeurApres;
}

void UpTextEdit::setChampCorrespondant(QString champcorrespondant)
{
    Champ = champcorrespondant;
}

QString UpTextEdit::getChampCorrespondant() const
{
    return Champ;
}

void UpTextEdit::setTableCorrespondant(QString tablecorrespondant)
{
    Table = tablecorrespondant;
}

QString UpTextEdit::getTableCorrespondant() const
{
    return Table;
}