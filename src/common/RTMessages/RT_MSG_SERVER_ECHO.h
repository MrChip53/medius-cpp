//
// Created by chris on 6/20/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_ECHO_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_ECHO_H


class RT_MSG_SERVER_ECHO {
public:
    const static uint8_t Request = 0x21;
    const static uint8_t Response = 0x1f;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        auto packet_data = Util::HexToBytes("a0180000e9fb0c00");

        auto len = std::get<0>(packet_data);
        auto packet = std::get<1>(packet_data);

        return Util::CreateIovec(packet, Response, len);
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_ECHO_H
