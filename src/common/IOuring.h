//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_IOURING_H
#define MEDIUS_MUIS_SERVER_IOURING_H

#include <liburing.h>
#include <memory>
#include <unordered_map>
#include "MediusHandler.h"
#include "UserData.h"

class IOuring {
private:
    int server_socket;
    MediusHandler *mediusHandler;
    std::unordered_map<int, std::shared_ptr<UserData>> userDatas;

    int SetupListeningSocket(int port);

public:
    struct request {
        int event_type;
        int iovec_count;
        int client_socket;
        struct iovec iov[];
    };

    io_uring ring;

    explicit IOuring(int port);

    void ServerLoop();

    int PostWriteRequest(struct IOuring::request *req);
    int PostReadRequest(int client_socket);
    int PostAcceptRequest(int server_socket, struct sockaddr_in *client_addr, socklen_t *client_addr_len);

    void SetMediusHandler(MediusHandler *handler);
    int GetServerSocket();

    int Close();
};


#endif //MEDIUS_MUIS_SERVER_IOURING_H
