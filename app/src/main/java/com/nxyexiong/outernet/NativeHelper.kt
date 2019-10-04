package com.nxyexiong.outernet

import android.util.Log
import java.lang.Exception
import kotlin.system.exitProcess

object NativeHelper {
    const val TAG = "NativeHelper"

    var newTunnelCallback: ((String, String) -> Unit)? = null

    init {
        try {
            java.lang.System.loadLibrary("jni")
        } catch (e: Exception) {
            Log.e(TAG, "load libjni failed with error $e")
            exitProcess(-1)
        }
    }

    // java to native
    external fun initClient(jhost: String, jport: Int, juser: String, jsecret: String): Int
    external fun uninitClient()
    external fun runClient(): Int
    external fun stopClient()
    external fun establishClient(jfd: Int)
    external fun checkHandshake()

    // native to java
    @JvmStatic
    fun onNewTunnel(tunIp: String, dstIp: String) {
        Log.i(TAG, "new tunnel $tunIp $dstIp")
        newTunnelCallback?.invoke(tunIp, dstIp)
    }
}