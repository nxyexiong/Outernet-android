//
// Created by kottonye on 2019-10-01.
//

#ifndef OUTERNET_CLIENT_H
#define OUTERNET_CLIENT_H

#include <stdint.h>
#include <ev.h>

class Crypto;
class Buffer;

class Client {
    ev_loop* loop;
    ev_io sock_io;
    ev_io iface_io;
    ev_timer timer;
    int inited;
    int running;
    int handshaked;
    int port;
    uint8_t identification[32];
    Crypto* crypto;
    int sock;
    sockaddr_in server_addr;
    int iface;

    Client();
    ~Client();
    void wrap_data(Buffer* buf);
    void unwrap_data(Buffer* buf);
public:
    static Client& get_instance()
    {
        static Client instance;
        return instance;
    }
    int init(char* host, int port, char* username, char* secret);
    void uninit();
    int run();
    void stop();
    void establish(int fd);
    ev_loop* get_loop();
    void on_recv();
    void on_read();
    void on_timeout();
};

#endif //OUTERNET_CLIENT_H
