//
// Created by chris on 6/20/21.
//

#ifndef MEDIUS_MUIS_SERVER_USERDATA_H
#define MEDIUS_MUIS_SERVER_USERDATA_H


#include <string>

class UserData {
    int32_t _SocketFd;
    uint32_t _WorldId;
    int32_t _AppId;
    std::string _Username;
    std::string _IP;
    bool _Closed;


public:
    explicit UserData() {
        _Closed = false;
    }

    auto SocketFd()       -> int32_t&       { return _SocketFd; }
    auto SocketFd() const -> const int32_t& { return _SocketFd; }
    auto WorldId()       -> uint32_t&       { return _WorldId; }
    auto WorldId() const -> const uint32_t& { return _WorldId; }
    auto AppId()       -> int32_t&       { return _AppId; }
    auto AppId() const -> const int32_t& { return _AppId; }
    auto Username()       -> std::string&       { return _Username; }
    auto Username() const -> const std::string& { return _Username; }
    auto IP()       -> std::string&       { return _IP; }
    auto IP() const -> const std::string& { return _IP; }
    auto Closed()       -> bool&       { return _Closed; }
    auto Closed() const -> const bool& { return _Closed; }


};


#endif //MEDIUS_MUIS_SERVER_USERDATA_H
