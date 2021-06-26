//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSUPDATELADDERSTATSWIDEREQUEST_H
#define MEDIUS_MLS_SERVER_MEDIUSUPDATELADDERSTATSWIDEREQUEST_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusUpdateLadderStatsWideRequest {
public:
    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, MediusHandler* handler, const std::shared_ptr<UserData>& uData) {
        // TODO figure how and where to save this
        //memcpy(uData->LadderStats, ((Packets::MediusUpdateLadderStatsWideRequest *) &data.mediusMessage[2])->Stats, LADDERSTATSWIDE_MAXLEN);

        return std::vector<struct iovec>();
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSUPDATELADDERSTATSWIDEREQUEST_H
