//
// Created by chris on 6/18/21.
//

#ifndef MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H
#define MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H

#include <vector>
#include <array>

class MediusHandler {
public:
    typedef struct {
        int command;
        unsigned short length;
        bool encrypted;
        unsigned char mediusMessage[2048];
    } MediusMessage;

    void ParseMessages(char * message);
    std::vector<struct iovec> ProcessMessages();

    void RegisterMessageHandler(int msgType, struct iovec(*msg_func)(char* data));

private:
    std::vector<MediusMessage> rt_messages;
    std::array<struct iovec(*)(char* data), 55> msgCallbacks;
};


#endif //MEDIUS_MUIS_SERVER_MEDIUSHANDLER_H
