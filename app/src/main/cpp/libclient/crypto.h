//
// Created by kottonye on 2019-10-02.
//

#ifndef OUTERNET_CRYPTO_H
#define OUTERNET_CRYPTO_H

class Buffer;

class Crypto {
    uint8_t key[32];
public:
    static Crypto& get_instance()
    {
        static Crypto instance;
        return instance;
    }
    int init(char* secret);
    void encrypt(Buffer* output, Buffer* input);
    void decrypt(Buffer* output, Buffer* input);
};

#endif //OUTERNET_CRYPTO_H
