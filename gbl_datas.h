#ifndef GBL_DATAS_H
#define GBL_DATAS_H

#include <QObject>
#include <cls_users.h>

class Datas : public QObject
{
    Q_OBJECT
private:
    Datas();
    static Datas *instance;

public:
    static Datas *I();

    User *userConnected;
    Users *users;         //!< Les users
};

#endif // GBL_DATAS_H
