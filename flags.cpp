#include "flags.h"


Flags* Flags::instance =  Q_NULLPTR;
Flags* Flags::I()
{
    if( !instance )
        instance = new Flags();

    return instance;
}

Flags::Flags(QObject *parent) : QObject (parent)
{
}

Flags::~Flags()
{
}

int Flags::flagCorrespondants()
{
    QVariantList flag = DataBase::getInstance()->getFirstRecordFromStandardSelectSQL("select MAJflagMG from " NOM_TABLE_FLAGS, ok);
    if (ok && flag.size() > 0)
        return flag.at(0).toInt();
    return 0;
}

int Flags::flagMessages()
{
    QVariantList flag = DataBase::getInstance()->getFirstRecordFromStandardSelectSQL("select MAJflagMessages from " NOM_TABLE_FLAGS, ok);
    if (ok && flag.size() > 0)
        return flag.at(0).toInt();
    return 0;
}

int Flags::flagSalleDAttente()
{
    QVariantList flag = DataBase::getInstance()->getFirstRecordFromStandardSelectSQL("select MAJflagSalDat from " NOM_TABLE_FLAGS, ok);
    if (ok && flag.size() > 0)
        return flag.at(0).toInt();
    return 0;
}

void Flags::MAJflagCorrespondants()
{
    int a = 1;
    /* mise à jour du flag en cas de non utilisation du TCP ou pour les utilisateurs distants qui le surveillent et mettent ainsi à jour leur liste de correspondants  */
    QString MAJreq = "insert into " NOM_TABLE_FLAGS " (MAJflagMG) VALUES (1)";
    QVariantList flagdata = DataBase::getInstance()->getFirstRecordFromStandardSelectSQL("select MAJflagMG from " NOM_TABLE_FLAGS, ok);
    if (ok && flagdata.size()>0) {
        a = flagdata.at(0).toInt() + 1;
        MAJreq = "update " NOM_TABLE_FLAGS " set MAJflagMG = " + QString::number(a);
    }
    DataBase::getInstance()->StandardSQL(MAJreq);
    /* envoi du signal de MAJ de la liste des correspondants */
    emit UpdCorrespondants(a);
}

void Flags::MAJFlagSalleDAttente()
{
    int a = 1;
    /* mise à jour du flag en cas de non utilisation du TCP ou pour les utilisateurs distants qui le surveillent et mettent ainsi à jour leur salle d'attente  */
    QString MAJreq = "insert into " NOM_TABLE_FLAGS " (MAJflagSalDat) VALUES (1)";
    QVariantList flagdata = DataBase::getInstance()->getFirstRecordFromStandardSelectSQL("select MAJflagSalDat from " NOM_TABLE_FLAGS, ok);
    if (ok && flagdata.size()>0) {
        a = flagdata.at(0).toInt() + 1;
        MAJreq = "update " NOM_TABLE_FLAGS " set MAJflagSalDat = " + QString::number(a);
    }
    DataBase::getInstance()->StandardSQL(MAJreq);
    /* envoi du signal de MAJ de la salle d'attente */
    emit UpdSalleDAttente(a);
}

void Flags::MAJflagMessages()
{
    int a = 1;
    /* mise à jour du flag en cas de non utilisation du TCP ou pour les utilisateurs distants qui le surveillent et mettent ainsi à jour leur liste de messages  */
    QString MAJreq = "insert into " NOM_TABLE_FLAGS " (MAJflagMessages) VALUES (1)";
    QVariantList flagdata = DataBase::getInstance()->getFirstRecordFromStandardSelectSQL("select MAJflagMessages from " NOM_TABLE_FLAGS, ok);
    if (ok && flagdata.size()>0) {
        a = flagdata.at(0).toInt() + 1;
        MAJreq = "update " NOM_TABLE_FLAGS " set MAJflagMessages = " + QString::number(a);
    }
    DataBase::getInstance()->StandardSQL(MAJreq);
    /* envoi du signal de MAJ de la messagerie */
    emit UpdMessages(a);
}

