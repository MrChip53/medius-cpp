//
// Created by chris on 6/18/21.
//

#include "IOuring.h"
#include "Util.h"
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#define QUEUE_DEPTH             256
#define READ_SZ                 8192

#define EVENT_TYPE_ACCEPT       0
#define EVENT_TYPE_READ         1
#define EVENT_TYPE_WRITE        2

int IOuring::PostWriteRequest(struct IOuring::request *req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    req->event_type = EVENT_TYPE_WRITE;
    io_uring_prep_writev(sqe, req->client_socket, req->iov, req->iovec_count, 0);
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
    return 0;
}

int IOuring::PostReadRequest(int client_socket) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    struct IOuring::request *req = (IOuring::request*)malloc(sizeof(*req) + sizeof(struct iovec));
    req->iov[0].iov_base = malloc(READ_SZ);
    req->iov[0].iov_len = READ_SZ;
    req->event_type = EVENT_TYPE_READ;
    req->client_socket = client_socket;
    memset(req->iov[0].iov_base, 0, READ_SZ);
    /* Linux kernel 5.5 has support for readv, but not for recv() or read() */
    io_uring_prep_readv(sqe, client_socket, &req->iov[0], 1, 0);
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
    return 0;
}

int IOuring::PostAcceptRequest(int server_socket, struct sockaddr_in *client_addr, socklen_t *client_addr_len) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_accept(sqe, server_socket, (struct sockaddr *) client_addr,
                         client_addr_len, 0);
    auto *req = (IOuring::request*)malloc(sizeof(IOuring::request));
    req->event_type = EVENT_TYPE_ACCEPT;
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
    return 0;
}

int IOuring::SetupListeningSocket(int port) {
    int sock;
    struct sockaddr_in srv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        Util::fatal_error("socket()");

    int enable = 1;
    if (setsockopt(sock,
                   SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0)
        Util::fatal_error("setsockopt(SO_REUSEADDR)");


    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* We bind to a port and turn this socket into a listening
     * socket.
     * */
    if (bind(sock,
             (const struct sockaddr *)&srv_addr,
             sizeof(srv_addr)) < 0)
        Util::fatal_error("bind()");

    if (listen(sock, 10) < 0)
        Util::fatal_error("listen()");

    return (sock);
}

IOuring::IOuring(int port) {
    server_socket = SetupListeningSocket(port);

    io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
}

int IOuring::GetServerSocket() {
    return server_socket;
}

int IOuring::Close() {
    io_uring_queue_exit(&ring);
    return 0;
}

void IOuring::ServerLoop() {
    struct io_uring_cqe *cqe;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    PostAcceptRequest(server_socket, &client_addr, &client_addr_len);

    while (true) {
        int ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0)
            Util::fatal_error("io_uring_wait_cqe");
        auto *req = (struct IOuring::request *) cqe->user_data;
        if (cqe->res < 0) {
            fprintf(stderr, "Async request failed: %s for event: %d\n",
                    strerror(-cqe->res), req->event_type);
            exit(1);
        }

        switch (req->event_type) {
            case EVENT_TYPE_ACCEPT: {
                PostAcceptRequest(server_socket, &client_addr, &client_addr_len);
                PostReadRequest(cqe->res);

                std::shared_ptr<UserData> user(new UserData);
                user->SocketFd() = cqe->res;

                struct sockaddr_in addr;
                socklen_t addr_size = sizeof(struct sockaddr_in);
                int res = getpeername(cqe->res, (struct sockaddr *)&addr, &addr_size);
                if (res == -1)
                    std::cerr << "Error retrieving peers address!" << std::endl;
                user->IP() = inet_ntoa(addr.sin_addr);

                userDatas[cqe->res] = std::move(user);

                free(req);
            }
            break;
            case EVENT_TYPE_READ: {
                int client_socket;
                char buffer[READ_SZ];

                std::vector<struct iovec> iovs;

                if (!cqe->res) {
                    fprintf(stderr, "Empty packet!\n");
                    break;
                }

                memset(buffer, 0, READ_SZ);
                client_socket = req->client_socket;
                memcpy(buffer, req->iov[0].iov_base, req->iov[0].iov_len);

                mediusHandler->ParseMessages(buffer);
                iovs = mediusHandler->ProcessMessages(userDatas[client_socket]);

                auto *outReq = static_cast<request *>(Util::cmalloc(sizeof(request) + sizeof(struct iovec) * iovs.size()));
                outReq->iovec_count = iovs.size();
                outReq->client_socket = client_socket;
                for (int i = 0; i < iovs.size(); i++) {
                    outReq->iov[i].iov_base = iovs[i].iov_base;
                    outReq->iov[i].iov_len = iovs[i].iov_len;
                }
                PostWriteRequest(outReq);

                free(req->iov[0].iov_base);
                free(req);

                PostReadRequest(client_socket);
            }
            break;
            case EVENT_TYPE_WRITE: {
                for (int i = 0; i < req->iovec_count; i++) {
                    //TODO change this back to a malloc/free. IDK what this warning actually means
                    delete[] req->iov[i].iov_base;
                }
                free(req);
            }
            break;
        }
        /* Mark this request as processed */
        io_uring_cqe_seen(&ring, cqe);
    }
}

void IOuring::SetMediusHandler(MediusHandler *handler) {
    mediusHandler = handler;
}
