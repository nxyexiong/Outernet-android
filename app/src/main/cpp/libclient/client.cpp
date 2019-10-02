//
// Created by kottonye on 2019-10-01.
//

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "client.h"
#include "crypto.h"
#include "buffer.h"
#include "utils.h"


#define BUF_SIZE 2048

void* loop_thread(void *) {
    int code = ev_run(Client::get_instance().get_loop(), 0);
    return nullptr;
}

static void recv_cb(EV_P_ ev_io *w, int revents)
{
    Client::get_instance().on_recv();
}

static void read_cb(EV_P_ ev_io *w, int revents)
{
    Client::get_instance().on_read();
}

static void timeout_cb(EV_P_ ev_timer *timer, int revents)
{
    Client::get_instance().on_timeout();
}

Client::Client() {
    sock = 0;
    iface = 0;
    inited = 0;
    running = 0;
    handshaked = 0;
    loop = NULL;
}

Client::~Client() {
}

int Client::init(char* host, int port, char* username, char* secret) {
    Utils::sha256(identification, (uint8_t*)username, strlen(username));

    crypto = new Crypto;
    crypto->init(secret);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_aton(host, &server_addr.sin_addr) == 0) {
        return -1;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock <= 0) {
        return -1;
    }
    if (bind(sock, (sockaddr *)&addr, sizeof(addr)) != 0) {
        return -1;
    }
    int flags;
    if (-1 == (flags = fcntl(sock, F_GETFL, 0))) {
        flags = 0;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        return -1;
    }

    inited = 1;
    return 0;
}

void Client::uninit() {
    delete crypto;
    crypto = NULL;

    close(sock);

    ev_loop_destroy(loop);
    loop = NULL;
}

int Client::run() {
    loop = ev_loop_new(0);
    if (!loop) {
        return  -1;
    }

    pthread_create(&loop_thread, nullptr, loop_thread, nullptr);

    ev_io_init(&sock_io, recv_cb, sock, EV_READ);
    ev_timer_init(&timer, timeout_cb, 1, 1);
    ev_io_start(loop, &sock_io);
    ev_timer_start(loop, &timer);
}

void Client::stop() {
    ev_io_stop(loop, &sock_io);
    ev_io_stop(loop, &iface_io);
    ev_timer_stop(loop, &timer);
}

void Client::establish(int fd) {
    iface = fd;
    ev_io_init(&iface_io, read_cb, iface, EV_READ);
    ev_io_start(loop, &iface_io);
}

ev_loop* Client::get_loop() {
    return loop;
}

void Client::on_recv() {
    sockaddr addr;
    int addrlen;
    Buffer buf;
    buf.init();
    buf.alloc(BUF_SIZE);
    int r = recvfrom(sock, buf.get_buf(), BUF_SIZE, 0, &addr, &addrlen);
    if (r <= 0) {
        buf.uninit();
        return;
    }
    buf.set_len(r);
    unwrap_data(&buf);

    if (handshaked == 0) {
        if (buf.get_len() != 11 || buf.get_buf()[0] != 0x01) {
            buf.uninit();
            return;
        }
        uint32_t* ipv4 = (uint32_t*)(buf.get_buf() + 1);
        char tun_ip[50] = {0};
        Utils::ipv4_to_str(*ipv4, tun_ip);
        ipv4 = (uint32_t*)(buf.get_buf() + 5);
        char dst_ip[50] = {0};
        Utils::ipv4_to_str(*ipv4, dst_ip);
        int port = buf.get_buf()[9] * 256 + buf.get_buf()[10];

        server_addr.sin_port = htons(port);
        // TODO: send tun_ip, dst_ip to kotlin to establish vpn

        handshaked = 1;
    } else {
        write(iface, buf.get_buf(), buf.get_len());
    }

    buf.uninit();
}

void Client::on_read() {
    Buffer buf;
    buf.init();
    buf.alloc(BUF_SIZE);
    int r = read(iface, buf.get_buf(), BUF_SIZE);
    buf.set_len(r);

    wrap_data(&buf);
    sendto(sock, buf.get_buf(), buf.get_len(), 0, (sockaddr*)&server_addr, sizeof(server_addr));
}

void Client::on_timeout() {
    if (handshaked == 1) {
        ev_timer_stop(loop, &timer);
        return;
    }

    Buffer buf;
    buf.init();
    buf.get_buf()[0] = 0x01;
    buf.set_len(1);
    buf.insert_back(identification, 32);

    wrap_data(&buf);
    sendto(sock, buf.get_buf(), buf.get_len(), 0, (sockaddr*)&server_addr, sizeof(server_addr));

    buf.uninit();
}

void Client::wrap_data(Buffer* buf) {
    Buffer tmp;
    tmp.init();
    crypto->encrypt(&tmp, &buf);
    buf->copy(&tmp);
    tmp.uninit();
}

void Client::unwrap_data(Buffer* buf) {
    Buffer tmp;
    tmp.init();
    crypto->decrypt(&tmp, &buf);
    buf->copy(&tmp);
    tmp.uninit();
}