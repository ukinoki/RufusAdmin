#include "gbl_datas.h"

Datas* Datas::instance = Q_NULLPTR;
Datas* Datas::I()
{
    if( !instance )
        instance = new Datas();

    return instance;
}
Datas::Datas()
{
    userConnected = nullptr;
    users = new Users();
}


