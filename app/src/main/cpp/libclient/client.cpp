//
// Created by kottonye on 2019-10-01.
//

#include "client.h"

Client::Client() {
    inited = false;
}

Client::~Client() {
}

int Client::init(char* host, int port, char* username, char* secret, int fd) {


    inited = true;
    return 0;
}

int Client::test(int x) {
    return x * 2;
}