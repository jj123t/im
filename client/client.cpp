//
// Created by 25421 on 2024/5/1.
//

#include <src/client.h>
#include <iostream>
#include <common/async/asiotimer.h>
#include <common/network/networkbase.h>
#include <src/msgmanager.h>
#include <thread>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    Client* client = new Client();
    client->newConnection();
    client->run();
    return 0;
}