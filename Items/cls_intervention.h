/* (C) 2020 LAINE SERGE
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

#ifndef CLS_INTERVENTION_H
#define CLS_INTERVENTION_H

#include "cls_item.h"
#include "utils.h"

/*!
 * \brief classe Intervention
 * l'ensemble des informations concernant une intervention
 */

class Intervention : public Item
{

public:
    enum                    ModeAnesthesie {Locale, LocoRegionale, Generale, NoLoSo};      Q_ENUM(ModeAnesthesie)
    explicit                Intervention(QJsonObject data = {}, QObject *parent = Q_NULLPTR);
    void                    setData(QJsonObject data = QJsonObject{});
    static ModeAnesthesie   ConvertModeAnesthesie(QString mode);
    static QString          ConvertModeAnesthesie(ModeAnesthesie mode);

private: //Données de l'intervention
    //!<m_id = Id de l'ntervention en base
    QTime m_heure               = QTime();          //! l'heure de l'intervention
    int m_idsession             = 0;                //! id de la session
    int m_idpatient             = 0;                //! id du patient
    ModeAnesthesie m_anesth     = NoLoSo;           //! le type d'anesthésie
    int m_idtypeintervention    = 0;                //! id du type d'intervention
    Utils::Cote m_cote          = Utils::NoLoSo;    //! côté de l'intervention
    int m_idIOL                 = 0;                //! id de l'IOL
    double m_pwrIOL             = 0;                //! puissance de l'implant
    double m_cylIOL             = 0;                //! cylindre de l'implant
    QString m_observation       = "";               //! observation
    int m_idacte                = 0;                //! id de l'acte correspondant
    QString m_incident          = "";               //! incident opératoire

public:
    QTime heure() const                     { return m_heure; }
    int idpatient() const                   { return m_idpatient; }
    int idsession() const                   { return m_idsession; }
    int idacte() const                      { return m_idacte; }
    ModeAnesthesie anesthesie() const       { return m_anesth; }
    int idtypeintervention() const          { return m_idtypeintervention; }
    Utils::Cote cote() const                { return m_cote; }
    int idIOL() const                       { return m_idIOL; }
    double puissanceIOL() const             { return m_pwrIOL; }
    double cylindreIOL() const              { return m_cylIOL; }
    QString observation() const             { return m_observation; }
    QString incident() const                { return m_incident; }

    void setheure(QTime time)               { m_heure = time;
                                              m_data[CP_HEURE_LIGNPRGOPERATOIRE] = time.toString("HH:mm::ss"); }
    void setidpatient(int id)               { m_idpatient = id;
                                              m_data[CP_IDPATIENT_LIGNPRGOPERATOIRE] = id; }
    void setidsession(int id)               { m_idsession = id;
                                              m_data[CP_IDSESSION_LIGNPRGOPERATOIRE] = id; }
    void setidacte(int id)                  { m_idacte = id;
                                              m_data[CP_IDACTE_LIGNPRGOPERATOIRE] = id; }
    void setanesthesie(ModeAnesthesie mode) { m_anesth = mode;
                                              m_data[CP_TYPEANESTH_LIGNPRGOPERATOIRE] = ConvertModeAnesthesie(mode); }
    void setidtypeintervention(int id)      { m_idtypeintervention = id;
                                              m_data[CP_IDTYPEINTERVENTION_LIGNPRGOPERATOIRE] = id; }
    void setcote(Utils::Cote cote)          { m_cote = cote;
                                              m_data[CP_COTE_LIGNPRGOPERATOIRE] = Utils::ConvertCote(cote); }
    void setidIOL(int id)                   { m_idIOL = id;
                                              m_data[CP_IDIOL_LIGNPRGOPERATOIRE] = id; }
    void setpuissanceIOL(double pwr)        { m_pwrIOL = pwr;
                                              m_data[CP_PWRIOL_LIGNPRGOPERATOIRE] = pwr; }
    void setcylindreIOL(double cyl)         { m_cylIOL = cyl;
                                              m_data[CP_CYLIOL_LIGNPRGOPERATOIRE] = cyl; }
    void setobservation(QString txt)        { m_observation = txt;
                                              m_data[CP_OBSERV_LIGNPRGOPERATOIRE] = txt; }
    void setincident(QString txt)           { m_incident = txt;
                                              m_data[CP_INCIDENT_LIGNPRGOPERATOIRE] = txt; }

    void resetdatas();
    bool isnull() const                     { return m_id == 0; }
 };

class SessionOperatoire : public Item
{

public:
    explicit SessionOperatoire(QJsonObject data = {}, QObject *parent = Q_NULLPTR);
    void setData(QJsonObject data = QJsonObject{})
    {
        if( data.isEmpty() )
            return;
        Utils::setDataInt(data, CP_ID_SESSIONOPERATOIRE, m_id);
        Utils::setDataInt(data, CP_IDLIEU_SESSIONOPERATOIRE, m_idlieu);
        Utils::setDataDate(data, CP_DATE_SESSIONOPERATOIRE, m_date);
        Utils::setDataInt(data, CP_IDUSER_SESSIONOPERATOIRE, m_iduser);
        Utils::setDataInt(data, CP_IDAIDE_SESSIONOPERATOIRE, m_idaide);
        Utils::setDataString(data, CP_INCIDENT_SESSIONOPERATOIRE, m_incident);
        m_data = data;
    }


private: //Données de la session
    //!<m_id = Id de la session en base
    QDate m_date            = QDate();          //!< Date de la session
    int m_idlieu            = 0;                //! id du lieu de la session
    int m_iduser            = 0;                //! id du chirurgien
    int m_idaide            = 0;                //! id de l'aide opératoire
    QString m_incident      = "";               //! incident druant la session

public:
    QDate date() const                      { return m_date; }
    int idlieu() const                      { return m_idlieu; }
    int iduser() const                      { return m_iduser; }
    int idaide() const                      { return m_idaide; }
    QString incident() const                { return m_incident; }

    void setdate(QDate date)                { m_date = date;
                                              m_data[CP_DATE_SESSIONOPERATOIRE] = date.toString("yyyy-MM-dd"); }
    void setidlieu(int id)                  { m_idlieu = id;
                                              m_data[CP_IDLIEU_SESSIONOPERATOIRE] = id; }
    void setiduser(int id)                  { m_iduser = id;
                                              m_data[CP_IDUSER_SESSIONOPERATOIRE] = id; }
    void setidaide(int id)                  { m_iduser = id;
                                              m_data[CP_IDAIDE_SESSIONOPERATOIRE] = id; }
    void setincident(QString txt)           { m_incident = txt;
                                              m_data[CP_INCIDENT_SESSIONOPERATOIRE] = txt; }
    void resetdatas()
    {
        QJsonObject data;
        data[CP_ID_SESSIONOPERATOIRE]           = 0;
        data[CP_IDLIEU_SESSIONOPERATOIRE]       = 0;
        data[CP_IDUSER_SESSIONOPERATOIRE]       = 0;
        data[CP_IDAIDE_SESSIONOPERATOIRE]       = 0;
        data[CP_INCIDENT_SESSIONOPERATOIRE]     = "";
        setData(data);
    }
    bool isnull() const                     { return m_id == 0; }
 };


/*!
 * \brief classe IOL
 * l'ensemble des informations concernant un IOL
 */

class IOL : public Item
{

public:
    explicit IOL(QMap<QString, QVariant> map = {}, QObject *parent = Q_NULLPTR);
    void setData(QMap<QString, QVariant> map = QMap<QString, QVariant>{});
    QMap<QString, QVariant> datas() const       { return m_map; }

private: //Données de l'intervention
    //!<m_id = Id de l'ntervention en base
    int m_idmanufacturer    = 0;                //! id du fabricant
    QString m_modele        = "";               //! modèle de l'implant
    bool m_inactif          = false;
    double m_pwrmax         = 0;                //! puissance maximale de l'implant
    double m_pwrmin         = 0;                //! puissance minimale de l'implant
    double m_pwrstp         = 0;                //! pas de variation de la puissance l'implant
    double m_cylmax         = 0;                //! cylindre maximale de l'implant
    double m_cylmin         = 0;                //! cylinde minimale de l'implant
    double m_cylstp         = 0;                //! pas de variation du cylindre l'implant
    double m_csteAopt       = 0;                //! constante A optique
    double m_csteAEcho      = 0;                //! constante A echographique
    double m_haigisa0       = 0;                //! constante Haigis a0
    double m_haigisa1       = 0;                //! constante Haigis a1
    double m_haigisa2       = 0;                //! constante Haigis a2
    double m_holladay       = 0;                //! constante Holladay
    double m_acd            = 0;                //! ACD
    double m_diainjecteur   = 0;                //! diamètre minimal de l'injecteur
    double m_diaall         = 0;                //! diamètre hors tout
    double m_diaoptique     = 0;                //! diamètre optique
    QByteArray m_imgiol     = QByteArray();     //! le cliché de l'IOL
    QString m_imageformat     = "";               //! le type de cliché, jpg ou pdf
    QString m_materiau      = "";               //! le matériau de l'IOL
    QString m_remarque      = "";               //! remarque à propos de l'IOL
    bool m_precharge        = false;            //! préchargé
    bool m_jaune            = false;            //! jaune ou clair
    bool m_multifocal       = false;            //! multifocal
    QMap<QString, QVariant> m_map = QMap<QString, QVariant>{};

public:
    int idmanufacturer() const                  { return m_idmanufacturer; }
    QString modele() const                      { return m_modele; }
    double pwrmax() const                       { return m_pwrmax; }
    double pwrmin() const                       { return m_pwrmin; }
    double pwrstp() const                       { return m_pwrstp; }
    double cylmax() const                       { return m_cylmax; }
    double cylmin() const                       { return m_cylmin; }
    double cylstp() const                       { return m_cylstp; }
    double csteAopt() const                     { return m_csteAopt; }
    double csteAEcho() const                    { return m_csteAEcho; }
    double haigisa0() const                     { return m_haigisa0; }
    double haigisa1() const                     { return m_haigisa1; }
    double haigisa2() const                     { return m_haigisa2; }
    double holladay() const                     { return m_holladay; }
    double acd() const                          { return m_acd; }
    double diainjecteur() const                 { return m_diainjecteur; }
    double diaall() const                       { return m_diaall; }
    double diaoptique() const                   { return m_diaoptique; }
    QByteArray imgiol() const                   { return m_imgiol; }
    QString imageformat() const                 { return m_imageformat; }
    QString materiau() const                    { return m_materiau; }
    QString remarque() const                    { return m_remarque; }
    bool isactif() const                        { return !m_inactif; }
    bool isprecharge() const                    { return m_precharge; }
    bool isjaune() const                        { return m_jaune; }
    bool ismultifocal() const                   { return m_multifocal; }
    QString tooltip() const;


    void setidmanufacturer(int &id)             { m_idmanufacturer = id;            m_map[CP_IDMANUFACTURER_IOLS] = id; }
    void setmodele(const QString &txt)          { m_modele = txt;                   m_map[CP_MODELNAME_IOLS] = txt; }
    void setactif(bool &actif)                  { m_inactif = !actif;               m_map[CP_INACTIF_IOLS] = !actif; }
    void setPwrmax(double pwrmax)               { m_pwrmax = pwrmax;                m_map[CP_MAXPWR_IOLS] = pwrmax; }
    void setPwrmin(double pwrmin)               { m_pwrmin = pwrmin;                m_map[CP_MINPWR_IOLS] = pwrmin; }
    void setPwrstp(double pwrstp)               { m_pwrstp = pwrstp;                m_map[CP_PWRSTEP_IOLS] = pwrstp; }
    void setCylmax(double cylmax)               { m_cylmax = cylmax;                m_map[CP_MAXCYL_IOLS] = cylmax; }
    void setCylmin(double cylmin)               { m_cylmin = cylmin;                m_map[CP_MINCYL_IOLS] = cylmin; }
    void setCylstp(double cylstp)               { m_cylstp = cylstp;                m_map[CP_CYLSTEP_IOLS] = cylstp; }
    void setCsteAopt(double csteAopt)           { m_csteAopt = csteAopt;            m_map[CP_CSTEAOPT_IOLS] = csteAopt; }
    void setCsteAEcho(double csteAEcho)         { m_csteAEcho = csteAEcho;          m_map[CP_CSTEAOPT_IOLS] = csteAEcho; }
    void setHaigisa0(double haigisa0)           { m_haigisa0 = haigisa0;            m_map[CP_HAIGISA0_IOLS] = haigisa0; }
    void setHaigisa1(double haigisa1)           { m_haigisa1 = haigisa1;            m_map[CP_HAIGISA1_IOLS] = haigisa1; }
    void setHaigisa2(double haigisa2)           { m_haigisa2 = haigisa2;            m_map[CP_HAIGISA2_IOLS] = haigisa2; }
    void setHolladay(double holladay)           { m_holladay = holladay;            m_map[CP_HOLL1_IOLS] = holladay; }
    void setAcd(double acd)                     { m_acd = acd;                      m_map[CP_ACD_IOLS] = acd; }
    void setDiainjecteur(double diainjecteur)   { m_diainjecteur = diainjecteur;    m_map[CP_DIAINJECTEUR_IOLS] = diainjecteur; }
    void setDiaall(double diaall)               { m_diaall = diaall;                m_map[CP_DIAALL_IOLS] = diaall; }
    void setDiaoptique(double diaoptique)       { m_diaoptique = diaoptique;        m_map[CP_DIAOPT_IOLS] = diaoptique; }
    void setImgiol(const QByteArray &imgiol)    { m_imgiol = imgiol;                m_map[CP_IMG_IOLS] = imgiol; }
    void setTypeImage(const QString &typeimage) { m_imageformat = typeimage;        m_map[CP_TYPIMG_IOLS] = typeimage; }
    void setMateriau(const QString &materiau)   { m_materiau = materiau;            m_map[CP_MATERIAU_IOLS] = materiau; }
    void setRemarque(const QString &remarque)   { m_remarque = remarque;            m_map[CP_REMARQUE_IOLS] = remarque; }
    void setprecharge(bool &precharge)          { m_precharge = precharge;          m_map[CP_PRECHARGE_IOLS] = precharge; }
    void setjaune(bool &jaune)                  { m_jaune = jaune;                  m_map[CP_JAUNE_IOLS] = jaune; }

    void resetdatas();
    bool isnull() const                         { return m_id == 0; }

};

/*!
 * \brief classe TypeIntervention
 * l'ensemble des informations concernant un type d'intervention
 */

class TypeIntervention : public Item
{

public:
    explicit TypeIntervention(QJsonObject data = {}, QObject *parent = Q_NULLPTR);
    void setData(QJsonObject data = QJsonObject{});

private: //Données de l'intervention
    //!<m_id = Id du type d'intervention en base
    QString m_typeintervention  = "";               //! le type d'intervention
    QString m_codeCCAM          = "";               //! code CCAM
    QTime   m_duree             = QTime();          //! la durée de l'intervention

public:

    QString typeintervention() const        { return m_typeintervention; }
    QString codeCCAM() const                { return m_codeCCAM; }
    QTime duree() const                     { return m_duree; }

    void settypeintervention(QString txt)   { m_typeintervention = txt;
                                              m_data[CP_TYPEINTERVENTION_TYPINTERVENTION] = txt; }
    void setcodeCCAM(QString txt)           { m_codeCCAM = txt;
                                              m_data[CP_CODECCAM_TYPINTERVENTION] = txt; }
    void setduree(QTime duree)               { m_duree = duree;
                                              m_data[CP_DUREE_TYPINTERVENTION] = duree.toString("HH:mm::ss"); }

    void resetdatas();
    bool isnull() const                     { return m_id == 0; }

 };

#endif // CLS_INTERVENTION_H
