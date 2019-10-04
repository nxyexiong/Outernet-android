//
// Created by kottonye on 2019-10-01.
//

#ifndef OUTERNET_CLIENT_H
#define OUTERNET_CLIENT_H

#include <stdint.h>
#include <pthread.h>
#include <ev.h>

class JavaHelper;
class Crypto;
class Buffer;

class Client {
    pthread_t loop_thread;
    struct ev_loop* loop;
    ev_io sock_io;
    ev_io iface_io;
    ev_timer timer;
    int inited;
    int running;
    int port;
    uint8_t identification[32];
    Crypto* crypto;
    int sock;
    struct sockaddr_in* server_addr;

    Client();
    ~Client();
    void wrap_data(Buffer* buf);
    void unwrap_data(Buffer* buf);
public:
    int loop_running;
    int handshaked;
    char tun_ip[32];
    char dst_ip[32];
    int iface;

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
    struct ev_loop* get_loop();
    void on_recv();
    void on_read();
    void on_timeout();
};

#endif //OUTERNET_CLIENT_H
