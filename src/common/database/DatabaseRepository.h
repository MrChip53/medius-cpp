//
// Created by chris on 5/6/21.
//

#ifndef MEDIUS_MUIS_SERVER_DATABASEREPOSITORY_H
#define MEDIUS_MUIS_SERVER_DATABASEREPOSITORY_H


#include <map>
#include "models/AppModel.h"
#include "models/UniverseModel.h"

class DatabaseRepository {
    std::map<int, AppModel*> apps;
    std::map<int, UniverseModel*> universes;

    DatabaseRepository() {

    }

    int LoadApps() {

    }

    int LoadUniverses() {

    }
private:
};


#endif //MEDIUS_MUIS_SERVER_DATABASEREPOSITORY_H
