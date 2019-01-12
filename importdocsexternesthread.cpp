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

#include "importdocsexternesthread.h"

ImportDocsExternesThread::ImportDocsExternesThread(int iduser, int idlieu, bool local)
{
    thread          = new QThread;
    moveToThread(thread);
    db              = DataBase::getInstance();
    idAdminDocs     = iduser;
    idLieuExercice  = idlieu;
    Acces           = (local? Local : Distant);
    EnCours         = false;
    gnomFichIni     = QDir::homePath() + NOMFIC_INI;
    gsettingsIni    = new QSettings(gnomFichIni, QSettings::IniFormat);
    a = 0;
    thread          ->start();
}

void ImportDocsExternesThread::RapatrieDocumentsThread(QList<QList<QVariant> > listdocs)     // INCORPORATION DES FICHIERS IMAGE DANS LA BASE  =====
{
    /* req = "select distinct list.TitreExamen, list.NomAPPareil from " NOM_TABLE_APPAREILSCONNECTESCENTRE " appcon, " NOM_TABLE_LISTEAPPAREILS " list"
          " where list.idappareil = appcon.idappareil and idLieu = " + QString::number(idlieuExercice);

    -> listdocs.at(itr).at(0) = le titre de l'examen
    -> listdocs.at(itr).at(1) = le nom de l'appareil*/
    if (EnCours)
        return;
    EnCours = true;
    listmsg.clear();
    datetransfer            = QDate::currentDate().toString("yyyy-MM-dd");
    if (!DefinitDossiers())
    {
        EnCours = false;
        return;
    }
    for (int itr=0; itr<listdocs.size(); itr++)
    {
        QString NomDirDoc         = getDossierDocuments(listdocs.at(itr).at(1).toString());  // le dossier où sont exportés les documents d'un appareil donné
        if (NomDirDoc == "")
            NomDirDoc = "Triumph Speed Triple 1050 2011";
        if (QDir(NomDirDoc).exists())
        {
            /* Retrouver
                 * Titre du document
                 * Date du document
                 * contenu du fichier
                 * idpatient
                */
            // Titre du document------------------------------------------------------------------------------------------------------------------------------------------------
            QString Titredoc    = listdocs.at(itr).at(0).toString();
            QString Typedoc     = Titredoc;
            QString SousTypeDoc = Titredoc;
            QString Appareil    = listdocs.at(itr).at(1).toString();
            QStringList listfich = QDir(NomDirDoc).entryList(QDir::Files | QDir::NoDotAndDotDot);
            for (int k=0; k<listfich.size(); k++)
            {
                QString nomdoc  = listfich.at(k);
                if (nomdoc.contains("smbtest"))
                    continue;
                QString CheminFichierImage      = NomDirDoc + "/" + nomdoc;
                QFile   jnaltrsferfile(CheminOKTransfrDir + "/0JournalTransferts - " + datetransfer + ".txt");
                QString commentechec;

                FichierOrigine.setFileName(CheminFichierImage);
                QString datetimecreation = QFileInfo(FichierOrigine).created().toString("yyyyMMdd-HHmmss");

                // Date et type du document------------------------------------------------------------------------------------------------------------------------------------------------
                QString datestring  = "";
                if (Appareil == "TOPCON ALADDIN")
                {
                    QDateTime datefic   = QFileInfo(FichierOrigine).created();
                    datestring          = datefic.toString("yyyyMMdd");
                    Titredoc            = "Biométrie - Aladdin";
                    Typedoc             = "Biométrie";
                    SousTypeDoc         = "Aladdin";
                }
                else if (Appareil == "TOPCON ALADDIN II")
                {
                    //1051_MIGUEL_JEAN-ROBERT_01-06-1948_Aladdin_06_06_2018_13_16.pdf
                    QStringList listn   = nomdoc.split("_");
                    int n               = listn.size();
                    QString jour        = listn.at(n-5);
                    QString mois        = listn.at(n-4);
                    QString annee       = listn.at(n-3);
                    datestring          = annee + mois + jour;
                    Titredoc            = "Biométrie - Aladdin";
                    Typedoc             = "Biométrie";
                    SousTypeDoc         = "Aladdin";
                }
                else if (Appareil == "TOPCON TRITON")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring  = nomdoc.split("_").at(3);
                        datestring  = datestring.left(8);
                    }
                    Titredoc    = "OCT - Topcon";
                    Typedoc     = "OCT";
                    SousTypeDoc = "Topcon";
                }
                else if (Appareil == "CANON CR-2")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring  = nomdoc.split("_").at(1);
                        datestring  = datestring.left(8);
                    }
                    Titredoc    = "RNM - Canon";
                    Typedoc     = "RNM";
                    QString cote = ((nomdoc.split("_").at(2) == "R")? tr("OD") : tr("OG"));
                    SousTypeDoc = "Canon " + cote;
                }
                else if (Appareil == "OTI SLO") {
                    datestring  = nomdoc.mid(nomdoc.indexOf("-")+1,8);
                    Titredoc    = "OCT - OTI";
                    Typedoc     = "OCT";
                    SousTypeDoc = "OTI";
                }
                else if (Appareil == "OPTOVUE")
                {
                    QStringList list = nomdoc.split("_");
                    if (list.size()>7)
                    {
                        datestring  = list.at(7);
                        datestring  = datestring.replace("-","");
                    }
                    Titredoc    = "OCT - Optovue";
                    Typedoc     = "OCT";
                    SousTypeDoc = "Optovue";
                }
                else if (Appareil == "NIDEK-OCT")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring = nomdoc.split("_").at(1);
                        datestring = datestring.left(8);
                    }
                    Titredoc    = "OCT - Nidek";
                    Typedoc     = "OCT";
                    SousTypeDoc = "Nidek";
                }
                else if (Appareil == "CANON-OCT")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring = nomdoc.split("_").at(1);
                        datestring = datestring.left(8);
                    }
                    Titredoc    = "OCT - Canon";
                    Typedoc     = "OCT";
                    QString cote = (nomdoc.contains("BothEyes")? tr("ODG") : ((nomdoc.split("_").at(4) == "R")? tr("OD") : tr("OG")));
                    QString typeexam = nomdoc.split("_").at(3);
                    if (typeexam == "OCTA")     typeexam = "AngioOCT";
                    if (typeexam == "Disc3D")   typeexam = "Glaucome";
                    SousTypeDoc = "Canon " + typeexam + " " + cote;
                    if (typeexam == "Disc3D")   typeexam = "Glaucome";
                    datetimecreation = datestring + "-" + nomdoc.split("_").at(2);
                }
                else if (Appareil == "NIDEK-RNM")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring = nomdoc.split("_").at(1);
                        datestring = datestring.left(8);
                    }
                    Titredoc    = "RNM - Nidek";
                    Typedoc     = "RNM";
                    SousTypeDoc = "Nidek";
                }
                else if (Appareil == "SPECTRALIS")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring = nomdoc.split("_").at(1);
                        datestring = datestring.left(8);
                    }
                    Titredoc    = "OCT - Heidelberg";
                    Typedoc     = "OCT";
                    SousTypeDoc = "Heidelberg";
                }
                else if (Appareil == "NAVIS-EX")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring = nomdoc.split("_").at(1);
                        datestring = datestring.left(8);
                    }
                    QString AbregeTitre = nomdoc.split("_").at(3);
                    if (AbregeTitre == "OT") {
                        Titredoc    = "OCT - Nidek";
                        Typedoc     = "OCT";
                        SousTypeDoc = "Nidek";
                    }
                    else if (AbregeTitre == "AO") {
                        Titredoc    = "OCT SA - Nidek";
                        Typedoc     = "OCT SA";
                        SousTypeDoc = "Nidek";
                    }
                    else if (AbregeTitre == "CO") {
                        Titredoc    = "RNM - Nidek";
                        Typedoc     = "RNM";
                        SousTypeDoc = "Nidek";
                    }
                }
                else if (Appareil == "EIDON")
                {
                    // il faut découper le nom de fichier à partir de la fin parce que c'est invariable
                    // si on fait l'inverse et qu'on met un tiret dans le nom de famille p.e.,
                    // la fonction plante dans ses découpages de QStringList et le programme avec
                    /*
                     * 368_Zammit-Sauveur-2017-06-14T17_39_21Z-eidon_20129-right-0-visible-2017-06-14T17_39_26Z--report.pdf
                     * 368_Zammit-Sauveur-2017-06-16T12_27_13Z-eidon_20129-right-0-af-2017-06-16T12_27_18Z--image.jpg
                    */
                    if (nomdoc.split("-").size()<11)
                    {
                        commentechec =  tr("nom invalide");
                        EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                        continue;
                    }
                    QStringList listitems = nomdoc.split("-");
                    datestring = listitems.at(listitems.size()-5);                  //année
                    datestring += listitems.at(listitems.size()-4);                 //mois
                    datestring += listitems.at(listitems.size()-3).split("T").at(0);//jour
                    QString cote = ((listitems.at(listitems.size()-8)=="right")? tr("OD") : tr("OG"));
                    Titredoc    = "RNM - Eidon ";
                    Typedoc     = "RNM";
                    SousTypeDoc = "Eidon " + cote;
                    if (listitems.at(listitems.size()-6) == "infrared")
                        SousTypeDoc += " IR";
                    else if (listitems.at(listitems.size()-6) == "af")
                        SousTypeDoc += " Autofluo";
                    QString time = listitems.at(listitems.size()-3).split("T").at(1);
                    time = time.split("Z").at(0);
                    datetimecreation   = datestring + "-"
                                       + time.split("_").at(0)
                                       + time.split("_").at(1)
                                       + time.split("_").at(2);
                }
                else if (Appareil == "OPTOS Daytona series")
                {
                    if (nomdoc.split("-").size()>1)
                    {
                        datestring = nomdoc.split("-").at(1);
                        datestring = datestring.left(8);
                    }
                    Titredoc    = "RNM - Optos";
                    Typedoc     = "RNM";
                    SousTypeDoc = "Optos";
                }
                else if (Appareil == "ZEISS CIRRUS 5000")
                {
                    if (nomdoc.split("_").size()>1)
                    {
                        datestring = nomdoc.split("_").at(7);
                        datestring = datestring.left(8);
                    }
                    Titredoc    = "OCT - Zeiss";
                    Typedoc     = "OCT";
                    SousTypeDoc = "Zeiss";
                }
                if (!QDate().fromString(datestring,"yyyyMMdd").isValid())
                {
                    commentechec =  tr("date invalide") + " -> " + datestring;
                    EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    continue;
                }
                datestring = QDate().fromString(datestring,"yyyyMMdd").toString("yyyy-MM-dd");
                // Format du document------------------------------------------------------------------------------------------------------------------------------------------------
                QString formatdoc = QFileInfo(listfich.at(k)).suffix().toLower();
                if (formatdoc != "pdf" && formatdoc != "jpg" && formatdoc != "jpeg")
                {
                    commentechec = tr("format invalide") + " -> " + formatdoc;
                    EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    continue;
                }
                // Contenu du document------------------------------------------------------------------------------------------------------------------------------------------------
                QByteArray ba;
                QString nomfichresize = NomDirStockageProv + "/resize" + nomdoc;
                QString szorigin, szfinal;
                // on vide le dossier provisoire
                QStringList listfichresize = QDir(NomDirStockageProv).entryList(QDir::Files | QDir::NoDotAndDotDot);
                for (int t=0; t<listfichresize.size(); t++)
                {
                    QString nomdocrz  = listfichresize.at(t);
                    QString CheminFichierResize = NomDirStockageProv + "/" + nomdocrz;
                    QFile(CheminFichierResize).remove();
                }
                if (FichierOrigine.open(QIODevice::ReadOnly))
                {
                    double sz = FichierOrigine.size();
                    if (sz/(1024*1024) > 1)
                        szorigin = QString::number(sz/(1024*1024),'f',1) + "Mo";
                    else
                        szorigin = QString::number(sz/1024,'f',1) + "Ko";
                    szfinal = szorigin;
                    FichierOrigine.copy(nomfichresize);
                    FichierImage.setFileName(nomfichresize);
                    if (formatdoc == "jpg" && sz > TAILLEMAXIIMAGES)
                    {
                        QImage  img(nomfichresize);
                        FichierImage.remove();
                        QPixmap pixmap;
                        pixmap = pixmap.fromImage(img.scaledToWidth(2560,Qt::SmoothTransformation));
                        int     tauxcompress = 90;
                        while (sz > TAILLEMAXIIMAGES && tauxcompress > 1)
                        {
                            pixmap.save(nomfichresize, "jpeg",tauxcompress);
                            sz = FichierImage.size();
                            tauxcompress -= 10;
                        }
                        if (sz/(1024*1024) > 1)
                            szfinal = QString::number(sz/(1024*1024),'f',0) + "Mo";
                        else
                            szfinal = QString::number(sz/1024,'f',0) + "Ko";
                    }
                    FichierImage.open(QIODevice::ReadOnly);
                    ba = FichierImage.readAll();
                }
                else
                {
                    commentechec =  tr("Impossible d'ouvrir le fichier");
                    EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    continue;
                }
                //qDebug()<< "ba.size() = " + QString::number(ba.size());

                // IdPatient------------------------------------------------------------------------------------------------------------------------------------------------
                QString req(""), idPatient("");
                if (Appareil == "TOPCON ALADDIN")   {
                    QStringList listn = nomdoc.split("_");
                    if (listn.size()<5)
                    {
                        commentechec =  tr("Impossible d'ouvrir le fichier");
                        EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                        continue;
                    }
                    QString nom     = Utils::capitilize(listn.at(0));
                    QString prenom  = Utils::capitilize(listn.at(1));
                    QString jour    = Utils::capitilize(listn.at(2));
                    QString mois    = Utils::capitilize(listn.at(3));
                    QString annee   = Utils::capitilize(listn.at(4));
                    req             = "select idpat from " NOM_TABLE_PATIENTS
                                      " where patnom like '" + nom + "'"
                                      " and patprenom like '" + prenom  + "'"
                                      " and patDDN = '" + annee + "-" + mois + "-" + jour + "'";
                    //qDebug() << req;
                    QList<QVariant> patlst = db->getFirstRecordFromStandardSelectSQL(req, ok);
                    if (!ok || patlst.size()==0)
                    {
                        commentechec =  tr("Impossible d'ouvrir le fichier");
                        EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                        continue;
                    }
                    idPatient = patlst.at(0).toString();
                }
                else if (Appareil == "TOPCON ALADDIN II")
                    idPatient           = nomdoc.split("_").at(0);
                else if (Appareil == "TOPCON TRITON")
                    idPatient           = nomdoc.split("_").at(2);
                else if (Appareil == "CANON CR-2")
                    idPatient           = nomdoc.split("_").at(0);
                else if (Appareil == "OTI SLO")
                    idPatient           = nomdoc.split("-").at(0);
                else if (Appareil == "OPTOVUE")
                {
                    if (nomdoc.split("__").size()>0)
                    {
                        idPatient       = nomdoc.split("__").at(1);
                        if (idPatient.split("_").size()>0)
                            idPatient   = idPatient.split("_").at(0);
                    }
                }
                else if (Appareil == "NIDEK-OCT")   {
                    idPatient           = nomdoc.split("_").at(0);
                }
                else if (Appareil == "CANON-OCT")   {
                    idPatient           = nomdoc.split("_").at(0);
                }
                else if (Appareil == "NIDEK-RNM")   {
                    idPatient           = nomdoc.split("_").at(0);
                }
                else if (Appareil == "SPECTRALIS")   {
                    idPatient           = nomdoc.split("_").at(0);
                }
                else if (Appareil == "NAVIS-EX")   {
                    idPatient           = nomdoc.split("_").at(0);
                }
                else if (Appareil == "EIDON")   {
                    idPatient           = nomdoc.split("-").at(0);
                }
                else if (Appareil == "OPTOS Daytona series")   {
                    idPatient           = nomdoc.split("-").at(0);
                }
                else if (Appareil == "ZEISS CIRRUS 5000")
                {
                    idPatient           = nomdoc.split("_").at(3);
                }
                bool b=true;
                if (idPatient.toInt(&b)<1)
                {
                    commentechec = tr("idPatient invalide") + " -> " + idPatient;
                    EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    continue;
                }
                if (!b)
                {
                    commentechec = tr("idPatient invalide") + " -> " + idPatient;
                    EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    continue;
                }
                QString identpat;
                QList<QVariant> patlst = db->getFirstRecordFromStandardSelectSQL("select patnom, patprenom from " NOM_TABLE_PATIENTS " where idpat = " + idPatient, ok);
                if (!ok || patlst.size()==0)
                {
                    commentechec =  tr("Pas de patient pour cet idPatient") + " -> " + idPatient;
                    EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    continue;
                }
                identpat = patlst.at(0).toString() + " " + patlst.at(1).toString();

                /* _______________________________________________________________________________________________________________________________________________________
                 * Enregistrement du fichier dans la base
                 * Si on est sur le réseau local, l'enregistrement se fait dans la table Impressions et le fichier est copié à son adresse définitive
                 * Si on est en accès distant, l'enregistrement se fait dans la table Impressions et le contenu du fichier est copié dans le champ blob de la table de la table
                 * _______________________________________________________________________________________________________________________________________________________
                */
                db->locktables(QStringList() << NOM_TABLE_IMPRESSIONS);
                int idimpr(0);
                idimpr = db->selectMaxFromTable("idimpression",  NOM_TABLE_IMPRESSIONS);

                QString NomFileDoc = idPatient + "_"
                        + Typedoc + "_"
                        + SousTypeDoc + "_"
                        + datetimecreation
                        + "-" + QString::number(idimpr)
                        + "." + QFileInfo(nomdoc).suffix();

                if (Acces == Local)
                {
                    req = "insert into " NOM_TABLE_IMPRESSIONS " (idimpression, idUser,  idpat,  TypeDoc,  SousTypeDoc, Titre, Dateimpression,"
                                                               " UserEmetteur, lienversfichier, EmisRecu, FormatDoc, idLieu)"
                                                               " values("
                            + QString::number(idimpr) + ", "
                            + QString::number(idAdminDocs) + ", "
                            + idPatient + ", '"
                            + Typedoc + "', '"
                            + SousTypeDoc + "', '"
                            + Titredoc + "', '"
                            + datestring + " " + QTime::currentTime().toString("HH:mm:ss") + "', "
                            + QString::number(idAdminDocs) + ", '"
                            + "/" + datetransfer + "/" + NomFileDoc + "', "
                            + "0" + ", '"
                            IMAGERIE "', "
                            + QString::number(idLieuExercice) + ")";

                    if(db->StandardSQL(req))
                    {
                        QString CheminOKTransfrDoc          = CheminOKTransfrDir + "/" + NomFileDoc;
                        QString CheminOKTransfrDocOrigin    = CheminOKTransfrDirOrigin + "/" + nomdoc;
                        FichierImage.copy(CheminOKTransfrDoc);
                        QFile CC(CheminOKTransfrDoc);
                        CC.open(QIODevice::ReadWrite);
                        CC.setPermissions(QFileDevice::ReadOther
                                          | QFileDevice::ReadGroup
                                          | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                                          | QFileDevice::ReadUser   | QFileDevice::WriteUser);
                        FichierImage.remove();
                        FichierOrigine.copy(CheminOKTransfrDocOrigin);
                        QFile CO(CheminOKTransfrDocOrigin);
                        CO.open(QIODevice::ReadWrite);
                        CO.setPermissions(QFileDevice::ReadOther
                                          | QFileDevice::ReadGroup
                                          | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                                          | QFileDevice::ReadUser   | QFileDevice::WriteUser);
                        if (jnaltrsferfile.open(QIODevice::Append))
                        {
                            QTextStream out(&jnaltrsferfile);
                            out << Titredoc << " - " << nomdoc << " - " << idPatient << " - " << identpat << " - " << QHostInfo::localHostName() << "\n" ;
                            jnaltrsferfile.close();
                        }
                        emit emitmsg(idPatient + TCPMSG_MAJDocsExternes);
                        if (FichierOrigine.remove())
                        {
                            QString msg = tr("Enregistrement d'un cliché") + " <font color=\"red\"><b>" + Titredoc + "</b></font>"
                                          " " + tr("pour") + " <font color=\"green\"><b>" + identpat + "</b></font> " + tr("dans la base de données");
                            if (formatdoc == "jpg" && szorigin != szfinal)
                                msg += "<br />" + tr("le cliché a été compressé de ") + szorigin + tr(" à ") + szfinal;
                            else
                                msg += "<br />" + tr("la taille du fichier est de ") + szorigin;
                            //qDebug() << "ba size = "  + QString::number(ba.size()) << "ba compressé size = " + QString::number(qCompress(ba).size());
                            listmsg << msg;
                            //qDebug() << "xx = " + QString::number(xx) << "x = " + QString::number(xx-DlgMess->width()-50) << "yy = " + QString::number(yy)  << "y = " + QString::number(yy-DlgMess->height()*(k+1))  << "itr = " << QString::number(k);
                        }
                        else
                            db->SupprRecordFromTable(idimpr, "idimpression", NOM_TABLE_IMPRESSIONS);
                    }
                    else
                    {
                        commentechec = tr("impossible d'enregistrer ") + nomdoc;
                        EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    }
                }
                else if (Acces == Distant)
                {
                    // on doit passer par les bindvalue pour incorporer le bytearray dans la requête
                    QHash<QString,QVariant> listbinds;
                    listbinds["idimpression"] =    QString::number(idimpr);
                    listbinds["iduser"] =          QString::number(idAdminDocs);
                    listbinds["idpat"] =           idPatient;
                    listbinds["typeDoc"] =         Typedoc;
                    listbinds["soustypedoc"] =     SousTypeDoc;
                    listbinds["titre"] =           Titredoc;
                    listbinds["dateimpression"] =  datestring + " " + QTime::currentTime().toString("HH:mm:ss");
                    listbinds["useremetteur"] =    QString::number(idAdminDocs);
                    listbinds[formatdoc] =         ba;
                    listbinds["emisrecu"] =        "0";
                    listbinds["formatdoc"] =       IMAGERIE;
                    listbinds["idlieu"] =          QString::number(idLieuExercice);

                    if(db->InsertSQLByBinds(NOM_TABLE_IMPRESSIONS, listbinds))
                    {
                        FichierImage.remove();
                        QString CheminOKTransfrDocOrigin    = CheminOKTransfrDirOrigin + "/" + nomdoc;
                        FichierOrigine.copy(CheminOKTransfrDocOrigin);
                        QFile CO(CheminOKTransfrDocOrigin);
                        CO.open(QIODevice::ReadWrite);
                        CO.setPermissions(QFileDevice::ReadOther
                                          | QFileDevice::ReadGroup
                                          | QFileDevice::ReadOwner  | QFileDevice::WriteOwner
                                          | QFileDevice::ReadUser   | QFileDevice::WriteUser);
                        if (jnaltrsferfile.open(QIODevice::Append))
                        {
                            QTextStream out(&jnaltrsferfile);
                            out << Titredoc << " - " << nomdoc << " - " << idPatient << " - " << identpat << " - " << QHostInfo::localHostName() << "\n" ;
                            jnaltrsferfile.close();
                        }
                        emit emitmsg(idPatient + TCPMSG_MAJDocsExternes);
                        if (FichierOrigine.remove())
                        {
                            QString msg = tr("Enregistrement d'un cliché") + " <font color=\"red\"><b>" + Titredoc + "</b></font>"
                                          " " + tr("pour") + " <font color=\"green\"><b>" + identpat + "</b></font> " + tr("dans la base de données");
                            if (formatdoc == "jpg" && szorigin != szfinal)
                                msg += "<br />" + tr("le cliché a été compressé de ") + szorigin + tr(" à ") + szfinal;
                            else
                                msg += "<br />" + tr("la taille du fichier est de ") + szorigin;
                            //qDebug() << "ba size = "  + QString::number(ba.size()) << "ba compressé size = " + QString::number(qCompress(ba).size());
                            listmsg << msg;
                            //qDebug() << "xx = " + QString::number(xx) << "x = " + QString::number(xx-DlgMess->width()-50) << "yy = " + QString::number(yy)  << "y = " + QString::number(yy-DlgMess->height()*(k+1))  << "itr = " << QString::number(k);
                        }
                        else
                            db->SupprRecordFromTable(idimpr, "idimpression", NOM_TABLE_IMPRESSIONS);
                    }
                    else
                    {
                        commentechec = tr("impossible d'enregistrer ") + nomdoc;
                        EchecImport(Titredoc + " - " + nomdoc + " - " + commentechec + " - " + QHostInfo::localHostName());
                    }
                }
                db->unlocktables();
            }
        }
    }

    if (listmsg.size()>0)
        emit emitmsg(listmsg, 3000, true);
    EnCours = false;
}

void ImportDocsExternesThread::EchecImport(QString txt)
{
    QString msg = tr("Impossible d'enregistrer le fichier ") + "<font color=\"red\"><b>" + QFileInfo(FichierImage).fileName() + "</b></font>" + tr(" dans la base de données");
    QStringList listmsg;
    listmsg << msg;
    emit emitmsg(listmsg, 3000, false);

    QString CheminEchecTransfrDoc   = CheminEchecTransfrDir + "/" + QFileInfo(FichierImage).fileName();
    FichierImage.copy(CheminEchecTransfrDoc);
    FichierImage.remove();
    QString echectrsfername         = CheminEchecTransfrDir + "/0EchecTransferts - " + datetransfer + ".txt";
    QFile   echectrsfer(echectrsfername);
    if (echectrsfer.open(QIODevice::Append))
    {
        QTextStream out(&echectrsfer);
        out << txt << "\n" ;
        echectrsfer.close();
    }
}

bool ImportDocsExternesThread::DefinitDossiers()
{
    QString Base = (Acces==Distant? "BDD_DISTANT/" : "");
    NomDirStockageImagerie  = gsettingsIni->value(Base + "DossierImagerie").toString();
    if (!QDir(NomDirStockageImagerie).exists() || NomDirStockageImagerie == "")
    {
        QString msg = tr("Le dossier de sauvegarde d'imagerie") + " <font color=\"red\"><b>" + NomDirStockageImagerie + "</b></font>" + tr(" n'existe pas");
        msg += "<br />" + tr("Renseignez un dossier valide dans") + " <font color=\"green\"><b>" + tr("Emplacement de stockage des documents archivés") + "</b></font>";
        QStringList listmsg;
        listmsg << msg;
        emit emitmsg(listmsg, 6000, true);
        return false;
    }
    NomDirStockageProv = NomDirStockageImagerie + NOMDIR_PROV;
    QDir DirStockProv;
    if (!Utils::mkpath(NomDirStockageProv))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + NomDirStockageProv + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        emit emitmsg(listmsg, 3000, true);
        return false;
    }
    CheminOKTransfrDir = NomDirStockageImagerie + NOMDIR_IMAGES + "/" + datetransfer;
    if (!Utils::mkpath(CheminOKTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDir + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        emit emitmsg(listmsg, 3000, true);
        return false;
    }
    CheminEchecTransfrDir   = NomDirStockageImagerie + NOMDIR_ECHECSTRANSFERTS;
    if (!Utils::mkpath(CheminEchecTransfrDir))
    {
        QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminEchecTransfrDir + "</b></font>" + tr(" invalide");
        QStringList listmsg;
        listmsg << msg;
        emit emitmsg(listmsg, 3000, true);
        return false;
    }
    
    if (Acces == Local)
    {
        CheminOKTransfrDirOrigin    = NomDirStockageImagerie + NOMDIR_ORIGINAUX NOMDIR_IMAGES + "/" + datetransfer;
        if (!Utils::mkpath(CheminOKTransfrDirOrigin))
        {
            QString msg = tr("Dossier de sauvegarde ") + "<font color=\"red\"><b>" + CheminOKTransfrDirOrigin + "</b></font>" + tr(" invalide");
            QStringList listmsg;
            listmsg << msg;
            emit emitmsg(listmsg, 3000, true);
            return false;
        }
    }
    return true;
}

QString ImportDocsExternesThread::getDossierDocuments(QString Appareil)
{
    return gsettingsIni->value("DossierEchangeImagerie/" + Appareil).toString();
}
