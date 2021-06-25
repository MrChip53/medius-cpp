//
// Created by chris on 6/20/21.
//

#ifndef MEDIUS_MUIS_SERVER_USERDATA_H
#define MEDIUS_MUIS_SERVER_USERDATA_H

#include <string>

#include "./Packets.h"

class UserData {
    int32_t SocketFd_;
    uint32_t WorldId_;
    int32_t AppId_;
    std::string Username_;
    std::string IP_;
    bool Closed_;
    Packets::MediusUserAction State_;
    std::string SKey_;


public:
    Packets::AccountStats Stats;

    explicit UserData() {
        Closed_ = false;
    }

    auto SocketFd()       -> int32_t&       { return SocketFd_; }
    auto SocketFd() const -> const int32_t& { return SocketFd_; }
    auto WorldId()       -> uint32_t&       { return WorldId_; }
    auto WorldId() const -> const uint32_t& { return WorldId_; }
    auto AppId()       -> int32_t&       { return AppId_; }
    auto AppId() const -> const int32_t& { return AppId_; }
    auto Username()       -> std::string&       { return Username_; }
    auto Username() const -> const std::string& { return Username_; }
    auto SessionKey()       -> std::string&       { return SKey_; }
    auto SessionKey() const -> const std::string& { return SKey_; }
    auto IP()       -> std::string&       { return IP_; }
    auto IP() const -> const std::string& { return IP_; }
    auto Closed()       -> bool&       { return Closed_; }
    auto Closed() const -> const bool& { return Closed_; }
    auto State()       -> Packets::MediusUserAction&       { return State_; }
    auto State() const -> const Packets::MediusUserAction& { return State_; }


};


#endif //MEDIUS_MUIS_SERVER_USERDATA_H
