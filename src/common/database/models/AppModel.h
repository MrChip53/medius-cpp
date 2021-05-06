//
// Created by chris on 5/6/21.
//

#ifndef MEDIUS_MUIS_SERVER_APPMODEL_H
#define MEDIUS_MUIS_SERVER_APPMODEL_H


#include "../entities/App.h"

class AppModel {
private:
    App* thisApp;

    AppModel(App* thisApp);
    ~AppModel();

    int GetUniverses() {

    }
};


#endif //MEDIUS_MUIS_SERVER_APPMODEL_H
