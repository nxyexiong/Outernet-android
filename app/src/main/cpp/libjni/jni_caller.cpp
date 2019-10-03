//
// Created by kottonye on 2019-09-27.
//

#include "jni.h"
#include <android/log.h>
#include <client.h>

JavaVM* g_vm = nullptr;
jobject g_obj = nullptr;

extern "C" JNIEXPORT jint JNICALL
Java_com_nxyexiong_outernet_NativeHelper_initClient(
        JNIEnv *env,
        jobject thiz,
        jstring jhost,
        jint jport,
        jstring juser,
        jstring jsecret)
{
    const char* host = env->GetStringUTFChars(jhost, 0);
    const char* user = env->GetStringUTFChars(juser, 0);
    const char* secret = env->GetStringUTFChars(jsecret, 0);

    int ret = Client::get_instance().init((char*)host, jport, (char*)user, (char*)secret);

    env->ReleaseStringChars(jhost, (jchar*)host);
    env->ReleaseStringChars(juser, (jchar*)user);
    env->ReleaseStringChars(jsecret, (jchar*)secret);

    return ret;
}

extern "C" JNIEXPORT void JNICALL
Java_com_nxyexiong_outernet_NativeHelper_uninitClient(
        JNIEnv *env,
        jobject thiz)
{
    Client::get_instance().uninit();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_nxyexiong_outernet_NativeHelper_runClient(
        JNIEnv *env,
        jobject thiz)
{
    return Client::get_instance().run();
}

extern "C" JNIEXPORT void JNICALL
Java_com_nxyexiong_outernet_NativeHelper_stopClient(
        JNIEnv *env,
        jobject thiz)
{
    Client::get_instance().stop();
}

extern "C" JNIEXPORT void JNICALL
Java_com_nxyexiong_outernet_NativeHelper_establishClient(
        JNIEnv *env,
        jobject thiz,
        jint jfd)
{
    return Client::get_instance().establish(jfd);
}

extern "C" JNIEXPORT void JNICALL
Java_com_nxyexiong_outernet_NativeHelper_checkHandshake(
        JNIEnv *env,
        jobject thiz)
{
    if (Client::get_instance().handshaked == 1) {
        jclass cls = env->FindClass("com/nxyexiong/outernet/NativeHelper");
        jmethodID func = env->GetStaticMethodID(cls, "onNewTunnel", "(Ljava/lang/String;Ljava/lang/String;)V");
        jstring tunIp = env->NewStringUTF(Client::get_instance().tun_ip);
        jstring dstIp = env->NewStringUTF(Client::get_instance().dst_ip);
        env->CallStaticVoidMethod(cls, func, tunIp, dstIp);
        env->DeleteLocalRef(tunIp);
        env->DeleteLocalRef(dstIp);
    }
}
