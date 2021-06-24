//
// Created by chris on 6/22/21.
//

#ifndef MEDIUS_MUIS_SERVER_MEDIUSUNIVERSEVARIABLEINFORMATIONRESPONSE_H
#define MEDIUS_MUIS_SERVER_MEDIUSUNIVERSEVARIABLEINFORMATIONRESPONSE_H


#include <sstream>

class MediusUniverseVariableInformationResponse {
public:
    const static uint8_t ReqMsgType = 0x01;
    const static uint8_t ReqMsgId = 0xC8;
    const static uint8_t RespType = 0x07;
    const static uint8_t RespId = 0x07;

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        std::vector<struct iovec> iovs;

        //TODO make this packet fully dynamic in its own class
        std::string svoUrl = "http://socomftb2.psp.online.scea.com:10061/SOCOMFTB2_SVML/index.jsp?languageid=1";

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
               << Util::ToHexFixedLength("v1.00 http://socomftb2.psp.online.scea.com:10061/SOCOMFTB2_SVML/download/EBOOT.PBP", 128)
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
