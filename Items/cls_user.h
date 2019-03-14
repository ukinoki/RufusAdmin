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

#ifndef CLS_USER_H
#define CLS_USER_H

#include <QMap>
#include <QObject>
#include <QVariant>
#include <QDate>
#include <QJsonObject>
#include "cls_item.h"
#include "cls_site.h"
#include "cls_compte.h"
#include "macros.h"

/*!
 * \brief The User class
 * l'ensemble des informations concernant
 * une personne utilisant le logiciel
 * ex: Praticien, Secretaire, ...
 */

class User : public Item
{
    Q_OBJECT
public: //static
    static int ROLE_NON_RENSEIGNE; //-1
    static int ROLE_VIDE; //-2
    static int ROLE_INDETERMINE; //-3

    static int COMPTA_AVEC_COTATION_AVEC_COMPTABILITE; //0
    static int COMPTA_SANS_COTATION_SANS_COMPTABILITE; //1
    static int COMPTA_AVEC_COTATION_SANS_COMPTABILITE; //2
    static int COMPTA_SANS_COTATION_AVEC_COMPTABILITE; //3




private:
    bool m_isAllLoaded = false;

    int m_id; //!< Id de l'utilsateur en base

    QString m_login; //!< Identifiant de l'utilisateur
    QString m_password;
    QString m_prenom; //!< prénom de l'utilisateur
    QString m_nom; //!< nom de l'utilisateur
    QString m_droits;
    QString m_mail;
    QString m_titre; //!< titre, ex: Docteur
    QString m_fonction;
    QString m_specialite;
    QString m_numCO;
    QString m_portable;
    QString m_web;
    QString m_memo;
    QString m_policeEcran;
    QString m_policeAttribut;
    QString m_nomUserEncaissHonoraires;
    QString m_nomCompteEncaissHonoraires;
    QString m_nomCompteParDefaut;
    int m_iduserComptePardefaut; //!< id du user propriétaire du compte bancaire par défaut

    int m_soignant;
    int m_responsableActes; //!< 1 : responsable
                            //!< 2 : responsable et assistant
                            //!< 3 : assistant
    int m_userenreghonoraires;
    int m_userccam;
    qlonglong m_numPS;
    int m_noSpecialite;
    int m_idCompteParDefaut;
    int m_poste;
    int m_employeur;
    int m_medecin;
    int m_idCompteEncaissHonoraires;
    int m_enregHonoraires;
    int m_secteur;

    int m_typeCompta = COMPTA_AVEC_COTATION_AVEC_COMPTABILITE;

    bool m_AGA;
    bool m_desactive = false;
    bool m_OPTAM;
    bool m_cotation;

    bool m_useCompta;

    QDateTime m_dateDerniereConnexion;

    Site *m_Site        = Q_NULLPTR;
    Comptes *m_comptes  = Q_NULLPTR;


    User *m_userSuperviseur     = Q_NULLPTR;
    int m_idUserActeSuperviseur = ROLE_INDETERMINE; //!< son id s'il est responsable de ses actes
                                                  //!< un idUser s'il est assistant
    User *m_userParent = Q_NULLPTR;
    int m_idUserParent = ROLE_INDETERMINE; //!< son id s'il n'est pas remplaçant
                                         //!< un idUser qui correspond au soigant remplacé
    User *m_userComptable = Q_NULLPTR;
    int m_idUserComptable = ROLE_INDETERMINE; //!< son id s'il est responsable et libéral
                                         //!< l'id de son employeur s'il est responsable et salarié
                                         //!< s'il est remplaçant (retrocession) on lui demande qui il remplace et le comptable devient
                                         //!< . celui qu'il remplace si celui qu'il remplace est libéral
                                         //!< . l'employeur de celui qu'il remplace si celui qu'il remplace est salarié



public:
    explicit User(QJsonObject data = {}, QObject *parent = Q_NULLPTR);
    explicit User(QString login, QString password, QJsonObject data = {}, QObject *parent = Q_NULLPTR);

    void setData(QJsonObject data);

    QString getLogin() const;
    QString getPassword() const;
    void setPassword(QString psswd);

    int id() const;
    void setId( int value );

    QString getNom() const;
    QString getPrenom() const;
    int getSoignant() const;
    int getResponsableactes() const;
    int getUserenreghonoraires() const;
    int getUserccam() const;
    int getEnregHonoraires() const;
    QString getTitre() const;
    int getNoSpecialite() const;
    QString getSpecialite() const;
    qlonglong getNumPS() const;
    QString getNumCO() const;
    bool getAGA() const;
    int getEmployeur() const;
    int getIdLieu() const;
    int getIdCompteEncaissHonoraires() const;
    QString getNomUserEncaissHonoraires() const;
    QString getNomCompteEncaissHonoraires() const;
    QString getFonction() const;

    int getIdUserActeSuperviseur() const;
    void setIdUserActeSuperviseur(int idUserActeSuperviseur);
    int getIdUserParent() const;
    void setIdUserParent(int idUserParent);
    int getIdUserComptable() const;
    void setIdUserComptable(int idUserComptable);
    int getidUserCompteParDefaut() const;

    int getSecteur() const;
    int getIdCompteParDefaut() const;
    QString getMail() const;
    QString getLoginComptable() const;
    QString getNomCompteParDefaut() const;
    QString getPortable() const;
    QString getNomCompteAbrege() const;


    QString getStatus() const;

    Site* getSite() const;
    void setSite(Site *Site);

    Comptes *getComptes() const;
    void setComptes(Comptes *comptes);

    int getTypeCompta() const;
    void setTypeCompta(int typeCompta);

    bool isAllLoaded() const;

    bool isOPTAM();
    bool isCotation();


    bool isSecretaire();
    bool isAutreFonction();
    bool isMedecin();
    bool isOpthalmo();
    bool isOrthoptist();
    bool isAutreSoignant();
    bool isNonSoignant();
    bool isSocComptable();
    bool isSoignant();
    bool isComptable();
    bool isLiberal();
    bool isSalarie();
    bool isRemplacant();
    bool isSansCompta();
    bool isResponsable();
    bool isResponsableEtAssistant();
    bool isAssistant();

    bool isDesactive();

    User *getUserSuperviseur() const;
    void setUserSuperviseur(User *userSuperviseur);

    User *getUserParent() const;
    void setUserParent(User *userParent);

    User *getUserComptable() const;
    void setUserComptable(User *userComptable);

signals:

public slots:
};

#endif // CLS_USER_H
