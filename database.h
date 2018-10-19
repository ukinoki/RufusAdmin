#ifndef DataBase_H
#define DataBase_H

/**
* \file DataBase.h
* \brief Cette classe gére l'ensemble des requetes SQL
* \author Alexanre.D
* \version 0.1
* \date 6 juin 2018
*
*
*/

#include <QJsonObject>
#include <QHostInfo>
#include <QObject>
#include <QSettings>
#include <QSqlDatabase>

#include "cls_user.h"
#include "cls_villes.h"
#include "log.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    enum m_mode { Poste, ReseauLocal, Distant };

private:
    DataBase();
    static DataBase *instance;

    User *m_userConnected = nullptr;


    int m_mode;
    QString m_base;
    QString m_server;
    int m_port;
    bool m_useSSL;

    QSqlDatabase m_db;


public:
    static DataBase *getInstance();

    void init(QSettings const &setting, int mode);
    void initFromFirstConnexion(QString mode, QString Server, int Port, bool SSL);

    int getMode() const;
    QString getBase() const;
    QString getBaseFromInt( int mode ) const;
    QString getServer() const;
    QSqlDatabase getDataBase() const;
    void getInformations();
    User* getUserConnected() const;

    bool traiteErreurRequete(QSqlQuery query, QString requete, QString ErrorMessage = "");

    QString connectToDataBase(QString basename, QString login, QString password);

    void    commit();
    void    rollback();
    bool    locktables(QStringList ListTables, QString ModeBlocage = "WRITE");
    bool    testconnexionbase();

    /*
     * Users
    */
    QJsonObject login(QString login, QString password);
    QJsonObject loadUserData(int idUser);
    QJsonObject loadUserDatabyLogin(QString logint);
    QList<User*> loadUsersAll();


    /*
     * Compta
    */
    QList<Compte*> loadComptesByUser(int idUser);

    /*
     * Sites
    */
    QList<Site*> loadSitesByUser(int idUser);
    QList<Site*> loadSitesAll();
private:
    QList<Site*> loadSites(QString req);

public:
    /*
     * Villes
    */
    Villes* loadVillesAll();

};

#endif // DataBase_H
