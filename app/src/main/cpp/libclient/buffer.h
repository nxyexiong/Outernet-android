//
// Created by kottonye on 2019-10-02.
//

#ifndef OUTERNET_BUFFER_H
#define OUTERNET_BUFFER_H

#include <stdint.h>

class Buffer {
    uint8_t* buf;
    int len;
    int size;

    void expand();

public:
    void init();
    void uninit();
    void set_len(int len);
    int get_len();
    uint8_t* get_buf();
    void insert_front(Buffer* buffer);
    void insert_front(uint8_t* buf, int len);
    void insert_back(Buffer* buffer);
    void insert_back(uint8_t* buf, int len);
    void copy(Buffer* buffer);
    void copy(uint8_t* buf, int len);
    void alloc(int len);
};

#endif //OUTERNET_BUFFER_H
