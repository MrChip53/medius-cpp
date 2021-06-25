//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSACCOUNTSTATSREQUEST_H
#define MEDIUS_MLS_SERVER_MEDIUSACCOUNTSTATSREQUEST_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusAccountStatsRequest {
public:
    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        // TODO verify session key
        memcpy(uData->Stats, ((Packets::MediusAccountUpdateStatsRequest *) &data.mediusMessage[2])->Stats, ACCOUNTSTATS_MAXLEN);

        return std::vector<struct iovec>();
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSACCOUNTSTATSREQUEST_H
