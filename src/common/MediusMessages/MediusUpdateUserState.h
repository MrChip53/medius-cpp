//
// Created by chris on 6/25/21.
//

#ifndef MEDIUS_MLS_SERVER_MEDIUSUPDATEUSERSTATE_H
#define MEDIUS_MLS_SERVER_MEDIUSUPDATEUSERSTATE_H

#include "../PacketIds.h"
#include "../Packets.h"
#include "../MediusHandler.h"

class MediusUpdateUserState {
public:

    static std::vector<struct iovec> process(MediusHandler::MediusMessage data, const std::shared_ptr<UserData>& uData) {
        uData->State() = ((Packets::MediusUpdateUserState *) &data.mediusMessage[2])->UserAction;

        return std::vector<struct iovec>();
    }
};


#endif //MEDIUS_MLS_SERVER_MEDIUSUPDATEUSERSTATE_H
