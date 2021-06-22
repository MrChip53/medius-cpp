#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/IOuring.h"
#include "../common/Util.h"

#include "../common/MediusMessages/Messages.h"

#define DEFAULT_SERVER_PORT     10071

IOuring* ring;

void sigint_handler(int signo) {
    printf("^C pressed. Shutting down.\n");
    ring->Close();
    exit(0);
}

int main() {
    MediusHandler* mHandler;

    mHandler = new MediusHandler();

    ring = new IOuring(DEFAULT_SERVER_PORT);
    signal(SIGINT, sigint_handler);

    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_HELLO::Request, RT_MSG_SERVER_HELLO::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_CONNECT_ACCEPT_TCP::Request, RT_MSG_SERVER_CONNECT_ACCEPT_TCP::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_CONNECT_COMPLETE::Request, RT_MSG_SERVER_CONNECT_COMPLETE::process);
    mHandler->RegisterRTMessageHandler(RT_MSG_SERVER_ECHO::Request, RT_MSG_SERVER_ECHO::process);

    ring->SetMediusHandler(mHandler);
    ring->ServerLoop();

    return 0;
}