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

#ifndef IMPORTDOCSEXTERNESTHREAD_H
#define IMPORTDOCSEXTERNESTHREAD_H

#include <QDir>
#include <QDateTime>
#include <QHostInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QThread>
#include <QMessageBox>
#include "macros.h"
#include "upmessagebox.h"
#include "database.h"
#include "utils.h"

/* Cette classe tourne en tache de fond et importe les documents d'imagerie dans la base de données
 * DIFFERENTE POUR RUFUS ET RUFUSADMIN
*/

class ImportDocsExternesThread : public QObject
{
    Q_OBJECT
public:
    explicit ImportDocsExternesThread(int iduser, int idlieu, bool local = true);
    void                        RapatrieDocumentsThread(QList<QVariantList> listdocs);
    enum Acces                  {Local, Distant}; Q_ENUM(Acces)

signals:
    void                        emitmsg(QStringList m_listmsg, int pause, bool bottom);
    void                        emitmsg(QString msg);

private:
    int                         m_idadmindocs;
    int                         m_idlieuexercice;
    void                        EchecImport(QString txt);
    bool                        m_encours;
    QString                     m_nomfichierini;
    QSettings                   *m_settings;
    QString                     pathdossierdocuments(QString Appareil) const;
    bool                        DefinitDossiers();
    bool                        m_ok;
    DataBase                    *db;
    QThread                     *thread;

    Acces                       m_acces;

    QString                     m_pathdirstockageprovisoire;
    QString                     m_pathdirstockageimagerie;
    QString                     m_pathdirOKtransfer;
    QString                     m_pathdirOKtransferorigin;
    QString                     m_datetransfer;
    QString                     m_pathdirEchectransfer;
    QStringList                 m_listmsg;
    QFile                       file_image, file_origine;
};

#endif // IMPORTDOCSEXTERNESTHREAD_H
