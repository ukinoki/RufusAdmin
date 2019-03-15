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

signals:
    void                        emitmsg(QStringList listmsg, int pause, bool bottom);
    void                        emitmsg(QString msg);

private:
    int                         idAdminDocs;
    int                         idLieuExercice;
    int                         a;
    void                        EchecImport(QString txt);
    bool                        EnCours;
    QString                     gnomFichIni;
    QSettings                   *gsettingsIni;
    QString                     getDossierDocuments(QString Appareil);
    bool                        DefinitDossiers();
    bool                        ok;
    DataBase                    *db;
    QThread                     *thread;

    int                         Acces;
    enum Acces                  {Local, Distant};

    QString                     NomDirStockageProv, NomDirStockageImagerie, CheminOKTransfrDir, CheminOKTransfrDirOrigin;
    QString                     datetransfer, CheminEchecTransfrDir;
    QStringList                 listmsg;
    QFile                       FichierImage, FichierOrigine;
};

#endif // IMPORTDOCSEXTERNESTHREAD_H
