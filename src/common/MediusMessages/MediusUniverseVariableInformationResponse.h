//
// Created by chris on 6/22/21.
//

#ifndef MEDIUS_MUIS_SERVER_MEDIUSUNIVERSEVARIABLEINFORMATIONRESPONSE_H
#define MEDIUS_MUIS_SERVER_MEDIUSUNIVERSEVARIABLEINFORMATIONRESPONSE_H


#include <sstream>

class MediusUniverseVariableInformationResponse {
//0a9700041e0000000000000000000000000000000000000000006868747470733a2f2f73766f2e707332726577697265642e636f6d3a3434332f53564f2f534f434f4d465442315f53564d4c2f696e646578312e6a7370000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000afe0100020411e8010200000000212a77b502000000000000000000000000000000000000000000000000000000000000007e03000025000000534f434f4d2046544231000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003133352e3134382e3134342e32353300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005b270000416e20656d756c61746564207365727665722070726f6a6563742062792031555020616e64204b337262657230732e000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ae30000020411cd000200010000212a77b5020000e80100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000001027000076302e3020687474703a2f2f70617463682e70733267616d6573746174732e636f6d2f667462312f63757272656e7470617463682f000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000a220101c9000000000000000000000000000000000000000000000000000000004e65777321000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000
public:
    const static uint8_t ReqMsgType = 0x01;
    const static uint8_t ReqMsgId = 0xC8;
    const static uint8_t RespType = 0x07;
    const static uint8_t RespId = 0x07;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;

        //TODO make this packet fully dynamic in its own class
        std::string svoUrl = "http://192.168.1.93:8080/";

        std::ostringstream svoStr;
        svoStr << "041e000000000000000000000000000000000000000000" << std::hex << (int)svoUrl.size() << Util::ToHex(svoUrl);
        auto svo_packet = Util::HexToBytes(svoStr.str());
        auto len = std::get<0>(svo_packet);
        auto packet = std::get<1>(svo_packet);
        std::vector<iovec> svoUrlIov = Util::CreateIovec(packet, 0x0a, len);
        iovs.insert(iovs.begin(), svoUrlIov.begin(), svoUrlIov.end());

        std::ostringstream uniStr;
        uniStr << "0afe0100020411e8010200000000212a77b502000000000000000000000000000000000000000000000000000000000000007e03000025000000"
               << Util::ToHexFixedLength("Socom Cafe", 128)
               << Util::ToHexFixedLength("192.168.1.93", 128)
               << Util::ReverseHexString(Util::Int32ToHex(10075)) // Port
               << Util::ToHexFixedLength("Chip's Server Yo!", 60)
               << "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ae30000020411cd000200010000212a77b5020000e801000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               << Util::ReverseHexString(Util::Int32ToHex(1)) // Status
               << Util::ReverseHexString(Util::Int32ToHex(17)) // Online
               << Util::ReverseHexString(Util::Int32ToHex(69420)) // Max
               << Util::ToHexFixedLength("v1.0 http://192.168.1.92:8080/patch", 128)
               << "01000000";
        auto universe_packet = Util::HexToBytes(uniStr.str());
        len = std::get<0>(universe_packet);
        packet = std::get<1>(universe_packet);
        iovec universePacket = Util::CreateStaticIovec(packet, len);
        iovs.push_back(universePacket);

        //TODO make this packet fully dynamic in its own class
        auto news_packet = Util::HexToBytes("0a220101c9000000000000000000000000000000000000000000000000000000004e65777321000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000");
        len = std::get<0>(news_packet);
        packet = std::get<1>(news_packet);
        iovec serverNews = Util::CreateStaticIovec(packet, len);
        iovs.push_back(serverNews);

        return iovs;
    }

};


#endif //MEDIUS_MUIS_SERVER_MEDIUSUNIVERSEVARIABLEINFORMATIONRESPONSE_H
