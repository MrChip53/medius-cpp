//
// Created by chris on 6/20/21.
//

#ifndef MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_COMPLETE_H
#define MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_COMPLETE_H


class RT_MSG_SERVER_CONNECT_COMPLETE {
public:
    const static uint8_t Request = 0x21;
    const static uint8_t Response = 0x1a;

    static struct iovec process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        iovec iov;

        auto packet_data = Util::HexToBytes("0100");

        auto len = std::get<0>(packet_data);
        auto packet = std::get<1>(packet_data);

        iov.iov_len = len + 3;
        char *buffer = new char[len + 3];
        memset(buffer, 0, len + 3);
        buffer[0] = Response;
        buffer[1] = (uint16_t)len;
        memcpy(&buffer[3], packet, len);
        delete[] packet;
        iov.iov_base = buffer;
        return iov;
    }
};


#endif //MEDIUS_MUIS_SERVER_RT_MSG_SERVER_CONNECT_COMPLETE_H
