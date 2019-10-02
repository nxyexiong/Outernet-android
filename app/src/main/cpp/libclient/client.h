//
// Created by kottonye on 2019-10-01.
//

#ifndef OUTERNET_CLIENT_H
#define OUTERNET_CLIENT_H

#include <stdint.h>

class Client {
    int inited;
    int port;
    uint8_t identification[32];

    Client();
    ~Client();
public:
    static Client& get_instance()
    {
        static Client instance;
        return instance;
    }
    int init(char* host, int port, char* username, char* secret, int fd);
    int test(int x);
};

#endif //OUTERNET_CLIENT_H
