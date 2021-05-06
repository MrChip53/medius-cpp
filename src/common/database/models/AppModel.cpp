//
// Created by chris on 5/6/21.
//

#include "AppModel.h"

AppModel::AppModel(App* thisApp) {
    this->thisApp = thisApp;
}

AppModel::~AppModel() {
    if (thisApp)
        free(thisApp);
}
