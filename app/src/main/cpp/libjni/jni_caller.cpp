//
// Created by kottonye on 2019-09-27.
//

#include "jni.h"
#include <android/log.h>
#include <client.h>

JavaVM* g_vm = nullptr;
jobject g_obj = nullptr;

extern "C" JNIEXPORT jint JNICALL
Java_com_nxyexiong_outernet_NativeHelper_test(
        JNIEnv *env,
        jobject thiz,
        jint input)
{
    Client client;
    client.init("119.28.23.25", 6666, "nxyexiong-sub0", "nxyexiong", 0);
    return client.test(input) * 2;
}
