//
// Created by kottonye on 2019-09-27.
//

#include "jni.h"
#include <android/log.h>

JavaVM* g_vm = nullptr;
jobject g_obj = nullptr;

extern "C" JNIEXPORT jint JNICALL
Java_com_nxyexiong_outernet_NativeHelper_test(
        JNIEnv *env,
        jobject thiz,
        jint input)
{
    return input * 2;
}
