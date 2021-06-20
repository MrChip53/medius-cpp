//
// Created by chris on 6/20/21.
//

#ifndef MEDIUS_MUIS_SERVER_USERDATA_H
#define MEDIUS_MUIS_SERVER_USERDATA_H


class UserData {
    int socketFd;

public:
    void setSocketFd(int fd);

};


#endif //MEDIUS_MUIS_SERVER_USERDATA_H
