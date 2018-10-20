#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

DataBase* DataBase::instance = Q_NULLPTR;

DataBase* DataBase::getInstance()
{
    if( !instance )
    {
        instance = new DataBase();
    }

    return instance;
}
DataBase::DataBase() {}

void DataBase::init(QSettings const &setting, int mode)
{
    m_mode = mode;
    if( m_mode == Poste )
        m_server = "localhost";
    else
        m_server = setting.value(getBase() + "/Serveur").toString();

    m_port = setting.value(getBase() + "/Port").toInt();

    m_useSSL = (m_mode == Distant);
}

void DataBase::initFromFirstConnexion(QString mode, QString Server, int Port, bool SSL)
{
    if (mode == "BDD_POSTE") m_mode = Poste;
    else if (mode == "BDD_LOCAL") m_mode = ReseauLocal;
    else if (mode == "BDD_DISTANT") m_mode = Distant;

    m_server = Server;
    m_port =Port;
    m_useSSL = SSL;
}

int DataBase::getMode() const
{
    return m_mode;
}
QString DataBase::getBase() const
{
    return getBaseFromInt( m_mode );
}
QString DataBase::getBaseFromInt( int mode ) const
{
    if (mode == ReseauLocal)
        return "BDD_LOCAL";

    if (mode == Distant)
        return "BDD_DISTANT";

    return "BDD_POSTE"; //m_mode == Poste
}
QString DataBase::getServer() const
{
    return m_server;
}
QSqlDatabase DataBase::getDataBase() const
{
    return m_db;
}
void DataBase::getInformations()
{
    UpMessageBox::Watch(Q_NULLPTR,
        tr("Connexion à la base de données!"),
        tr("Vos paramètres de connexion") +
           "\n" + tr("Serveur     ") + "\n ->\t" + m_db.hostName() +
           "\n" + tr("databaseName") + "\n ->\t" + m_db.databaseName() +
           "\n" + tr("Login       ") + "\n ->\t" + m_db.userName() +
           "\n" + tr("password    ") + "\n ->\t" + m_db.password() +
           "\n" + tr("port        ") + "\n ->\t" + QString::number(m_db.port()));
}
User* DataBase::getUserConnected() const
{
    return m_userConnected;
}

bool DataBase::traiteErreurRequete(QSqlQuery query, QString requete, QString ErrorMessage)
{
    if (query.lastError().type() != QSqlError::NoError)
    {
        Logs::ERROR(ErrorMessage, tr("\nErreur\n") + query.lastError().text() +  tr("\nrequete = ") + requete);
        return true;
    }

    return false;
}

QString DataBase::connectToDataBase(QString basename, QString login, QString password)
{

    m_db = QSqlDatabase::addDatabase("QMYSQL",basename);
    m_db.setHostName( m_server );
    m_db.setPort( m_port );

    QString connectOptions = (m_useSSL?
                              "SSL_KEY=/etc/mysql/client-key.pem;"
                              "SSL_CERT=/etc/mysql/client-cert.pem;"
                              "SSL_CA=/etc/mysql/ca-cert.pem;"
                              "MYSQL_OPT_RECONNECT=1"
                                 :
                              "MYSQL_OPT_RECONNECT=1");
    m_db.setConnectOptions(connectOptions);

    m_db.setUserName(login + (m_useSSL ? "SSL" : ""));
    m_db.setPassword(password);

    if( m_db.open() )
        return QString();

    QSqlDatabase::removeDatabase(basename);
    return m_db.lastError().text();
}

void DataBase::commit()
{
    QSqlQuery ("COMMIT;", m_db );
    QSqlQuery ("UNLOCK TABLES;", m_db );
    QString commitrequete = "SET AUTOCOMMIT = 1;";
    QSqlQuery commitquery (commitrequete,m_db );
    getInstance()->traiteErreurRequete(commitquery, commitrequete, tr("Impossible de valider les mofifications"));
}

void DataBase::rollback()
{
    QSqlQuery ("ROLLBACK;", m_db );
    QSqlQuery ("UNLOCK TABLES;", m_db );
    QString rollbackrequete = "SET AUTOCOMMIT = 1;";
    QSqlQuery rollbackquery (rollbackrequete, m_db );
    getInstance()->traiteErreurRequete(rollbackquery,rollbackrequete,"");
}

bool DataBase::locktables(QStringList ListTables, QString ModeBlocage)
{
    QSqlQuery ("UNLOCK TABLES;", m_db );
    QSqlQuery ("SET AUTOCOMMIT = 0;", m_db );
    QString lockrequete = "LOCK TABLES " + ListTables.at(0) + " " + ModeBlocage;
    for (int i = 1; i < ListTables.size(); i++)
        lockrequete += "," + ListTables.at(i) + " " + ModeBlocage;
    QSqlQuery lockquery (lockrequete, m_db );
    return !getInstance()->traiteErreurRequete(lockquery,lockrequete, tr("Impossible de bloquer les tables en mode ") + ModeBlocage);
}

bool DataBase::testconnexionbase() // une requete simple pour vérifier que la connexion à la base fontionne toujours
{
    QString req = "select AdresseTCPServeur from " NOM_TABLE_PARAMSYSTEME;
    QSqlQuery testbasequery(req, DataBase::getInstance()->getDataBase());
    return (testbasequery.lastError().type()==QSqlError::NoError);
}

/*
 * Users
*/
QJsonObject DataBase::login(QString login, QString password)
{
    QJsonObject jrep{};
    int userOffset = 3;

    //TODO : SQL USER : récupérer tout le reste
    QString req = "SELECT u.idUser, u.UserNom, u.UserPrenom, "
                    "uc.NomPosteconnecte "
                  " FROM " NOM_TABLE_UTILISATEURS " u "
                  " LEFT JOIN " NOM_TABLE_USERSCONNECTES " uc on uc.idUSer = u.idUser "
                  " WHERE UserLogin = '" + login + "' "
                  " AND UserMDP = '" + password + "' ";
    QSqlQuery query(req, getDataBase() );
    if( query.lastError().type() != QSqlError::NoError )
    {
        jrep["code"] = -3;
        jrep["request"] = req;
        return jrep;
    }

    if( !query.first() )
    {
        jrep["code"] = -1;
        return jrep;
    }

    if( !query.value(userOffset).isNull() &&query.value(userOffset).toString() != QHostInfo::localHostName().left(60) )
    {
        jrep["code"] = -6;
        jrep["poste"] = query.value(userOffset).toString();
        // cette erreur n'est pas exploitée parce que dans certaines structures, un même utilisateur peut travailler sur plusieurs postes
        //return jrep;
    }

    jrep["code"] = 0;

    QJsonObject userData{};
    userData["id"] = query.value(0).toInt();
    userData["nom"] = query.value(1).toString();
    userData["prenom"] = query.value(2).toString();
    query.finish();

    m_userConnected = new User(login, password, userData);
    m_userConnected->setData( loadUserData(m_userConnected->id()) );
    return jrep;
}


QJsonObject DataBase::loadUserDatabyLogin(QString login)
{
    QJsonObject userData{};

    QString req = "select iduser from " NOM_TABLE_UTILISATEURS " where UserLogin = '" + login + "'";
    QSqlQuery  query(req, getDataBase());
    if( traiteErreurRequete(query, req, tr("Impossible de retrouver les données de l'utilisateur")) )
        return userData;

    if( !query.first() )
        return userData;
    return loadUserData(query.value(0).toInt());

}

QJsonObject DataBase::loadUserData(int idUser)
{
    QJsonObject userData{};

    QString req = "select UserDroits, UserAGA, UserLogin, UserFonction, UserTitre, "                    // 0,1,2,3,4
            " UserNom, UserPrenom, UserMail, UserNumPS, UserSpecialite,"                                // 5,6,7,8,9
            " UserNoSpecialite, UserNumCO, idCompteParDefaut, UserEnregHonoraires, UserMDP,"            // 10,11,12,13,14
            " UserPortable, UserPoste, UserWeb, UserMemo, UserDesactive,"                               // 15,16,17,18,19
            " UserPoliceEcran, UserPoliceAttribut, UserSecteur, Soignant, ResponsableActes,"            // 20,21,22,23,24
            " UserCCAM, UserEmployeur, DateDerniereConnexion, idCompteEncaissHonoraires, Medecin,"      // 25,26,27,28,29
            " OPTAM, cpt.nomcompteabrege, cpt2.nomcompteabrege "                                        // 30,31,32
            " from " NOM_TABLE_UTILISATEURS " usr "
            " left outer join " NOM_TABLE_COMPTES " cpt on usr.idcompteencaisshonoraires = cpt.idCompte"
            " left outer join " NOM_TABLE_COMPTES " cpt2 on usr.idCompteParDefaut = cpt2.idCompte"
            " where usr.idUser = " + QString::number(idUser);

            //+ "  and userdesactive is null";
            // SL cette ligne est retirée parce qu'elle bloque l'affichage des utilisateurs désactivés dans dlg_gestionsusers

    QSqlQuery  query(req, getDataBase());
    if( traiteErreurRequete(query, req, tr("Impossible de retrouver les données de l'utilisateur")) )
        return userData;

    if( !query.first() )
        return userData;

    userData["isAllLoaded"]                 = true;
    userData["id"]                          = idUser;
    userData["droits"]                      = query.value(0).isNull() ? "" : query.value(0).toString();
    userData["AGA"]                         = (query.value(1).toInt() == 1);
    userData["login"]                       = query.value(2).isNull() ? "" : query.value(2).toString();
    userData["fonction"]                    = query.value(3).isNull() ? "" : query.value(3).toString();
    userData["titre"]                       = query.value(4).isNull() ? "" : query.value(4).toString();
    userData["nom"]                         = query.value(5).isNull() ? "" : query.value(5).toString();
    userData["prenom"]                      = query.value(6).isNull() ? "" : query.value(6).toString();
    userData["mail"]                        = query.value(7).isNull() ? "" : query.value(7).toString();
    userData["numPS"]                       = query.value(8).toInt();
    userData["specialite"]                  = query.value(9).isNull() ? "" : query.value(9).toString();
    userData["noSpecialite"]                = query.value(10).toInt();
    userData["numCO"]                       = query.value(11).isNull() ? "" : query.value(11).toString();
    userData["idCompteParDefaut"]           = query.value(12).toInt();
    userData["enregHonoraires"]             = query.value(13).toInt();
    userData["password"]                    = query.value(14).toString();
    userData["portable"]                    = query.value(15).isNull() ? "" : query.value(15).toString();
    userData["poste"]                       = query.value(16).toInt();
    userData["web"]                         = query.value(17).isNull() ? "" : query.value(17).toString();
    userData["memo"]                        = query.value(18).isNull() ? "" : query.value(18).toString();
    userData["desactive"]                   = (query.value(19).toInt() == 1);
    userData["policeEcran"]                 = query.value(20).isNull() ? "" : query.value(20).toString();
    userData["policeAttribut"]              = query.value(21).isNull() ? "" : query.value(21).toString();
    userData["secteur"]                     = query.value(22).toInt();
    userData["OPTAM"]                       = (query.value(30).toInt() == 1);
    userData["soignant"]                    = query.value(23).toInt();
    userData["responsableActes"]            = query.value(24).toInt();
    userData["cotation"]                    = (query.value(25).toInt() == 1);
    userData["employeur"]                   = query.value(26).toInt();
    userData["dateDerniereConnexion"]       = QDateTime(query.value(27).toDate(), query.value(27).toTime()).toMSecsSinceEpoch();
    userData["medecin"]                     = query.value(29).toInt();
    userData["idCompteEncaissHonoraires"]   = query.value(28).toInt();

    if( userData["idCompteEncaissHonoraires"].isNull() )
    {
        userData["idUserEncaissHonoraires"] = -1;
    }
    else
    {
        userData["idUserEncaissHonoraires"]    = idUser;
        userData["nomCompteEncaissHonoraires"] = query.value(31).toString();
        userData["nomUserEncaissHonoraires"]   = query.value(2).toString();
    }

    if( !query.value(32).isNull() )
        userData["nomCompteParDefaut"] = query.value(32).toString();

    query.finish();
    return userData;
}
QList<User*> DataBase::loadUsersAll()
{
    QList<User*> users;
    QString req = "select usr.iduser, usr.userlogin, usr.soignant, usr.responsableactes, "          //0,1,2,3
                    " usr.UserEnregHonoraires, usr.idCompteEncaissHonoraires, usr.UserCCAM, "       //4,5,6
                    " usr.UserEmployeur, cpt.nomcompteabrege "                                      //7,8
                  " from " NOM_TABLE_UTILISATEURS " usr "
                  " left outer join " NOM_TABLE_COMPTES " cpt on usr.idcompteencaisshonoraires = cpt.idCompte "
                  " where userdesactive is null";

    QSqlQuery query(req, getDataBase() );
    if( traiteErreurRequete(query, req) || !query.first())
        return users;

    do
    {
        QJsonObject jData{};
        jData["id"] = query.value(0).toInt();
        jData["login"] = query.value(1).toString();
        jData["soignant"] = query.value(2).toInt();
        jData["responsableActes"] = query.value(3).toInt();
        jData["enregHonoraires"] = query.value(4).toInt();
        jData["idCompteEncaissHonoraires"] = query.value(5).toInt();
        jData["cotation"] = (query.value(6).toInt() == 1);
        jData["employeur"] = query.value(7).toInt();
        jData["nomCompteAbrege"] = query.value(8).toString();
        User *usr = new User(jData);
        users << usr;
    } while( query.next() );

    return users;
}


/*
 * Compta
*/
QList<Compte*> DataBase::loadComptesByUser(int idUser)
{
    QList<Compte*> comptes;
    QString req = "SELECT idCompte, NomCompteAbrege, desactive, IBAN, cmpt.idbanque, intitulecompte, NomBanque "
                  " FROM " NOM_TABLE_COMPTES " as cmpt "
                  " left outer join " NOM_TABLE_BANQUES " as bank on cmpt.idbanque = bank.idbanque "
                  " WHERE idUser = " + QString::number(idUser);
    QSqlQuery query(req, getDataBase() );
    if( traiteErreurRequete(query, req) || !query.first())
        return comptes;

    do
    {
        QJsonObject jData{};
        jData["id"] = query.value(0).toInt();
        jData["nom"] = query.value(1).toString();
        jData["desactive"] = (query.value(2).toInt() == 1);
        jData["IBAN"] = query.value(3).toString();
        jData["IntituleCompte"] = query.value(5).toString();
        jData["NomBanque"] = query.value(6).toString();
        Compte *cpt = new Compte(jData);
        comptes << cpt;
    } while( query.next() );

    return comptes;
}


/*
 * Sites
*/
QList<Site*> DataBase::loadSitesAll()
{
    QString req = "select joint.idLieu, NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, "
                    "LieuCodePostal, LieuVille, LieuTelephone, LieuFax "
                  "from " NOM_TABLE_JOINTURESLIEUX;

    return loadSites( req );
}
QList<Site*> DataBase::loadSitesByUser(int idUser)
{
    QString req = "select joint.idLieu, NomLieu, LieuAdresse1, LieuAdresse2, LieuAdresse3, "
                    "LieuCodePostal, LieuVille, LieuTelephone, LieuFax "
                  "from " NOM_TABLE_JOINTURESLIEUX " joint "
                  "left join " NOM_TABLE_LIEUXEXERCICE " lix on joint.idlieu = lix.idLieu "
                  "where iduser = " + QString::number(idUser);

    return loadSites( req );
}
QList<Site*> DataBase::loadSites(QString req)
{
    QList<Site*> etabs;
    QSqlQuery query(req, getDataBase() );
    if( traiteErreurRequete(query, req) || !query.first())
        return etabs;

    do
    {
        QJsonObject jEtab{};
        jEtab["idLieu"] = query.value(0).toInt();
        jEtab["nomLieu"] = query.value(1).toString();
        jEtab["adresse1"] = query.value(2).toString();
        jEtab["adresse2"] = query.value(3).toString();
        jEtab["adresse3"] = query.value(4).toString();
        jEtab["codePostal"] = query.value(5).toInt();
        jEtab["ville"] = query.value(6).toString();
        jEtab["telephone"] = query.value(7).toString();
        jEtab["fax"] = query.value(8).toString();
        Site *etab = new Site(jEtab);
        etabs << etab;
    } while( query.next() );

    return etabs;
}


/*
 * Villes
*/
Villes* DataBase::loadVillesAll()
{
    Villes *villes = new Villes();

    QString req = "select ville_id, codePostal, ville "
                  "from " NOM_TABLE_VILLES;
    QSqlQuery query(req, getDataBase() );
    if( traiteErreurRequete(query, req) || !query.first())
        return villes;

    do
    {
        QJsonObject jEtab{};
        jEtab["ville_id"] = query.value(0).toInt();
        jEtab["codePostal"] = query.value(1).toString();
        jEtab["ville"] = query.value(2).toString();
        Ville *ville = new Ville(jEtab);
        villes->addVille(ville);
    } while( query.next() );

    return villes;
}


