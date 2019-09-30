package com.nxyexiong.outernet

import android.util.Log
import java.lang.Exception

class NativeHelper {
    companion object {
        const val TAG = "NativeHelper"
    }

    init {
        try {
            java.lang.System.loadLibrary("jni")
        } catch (e: Exception) {
            Log.e(TAG, "load libjni failed with error $e")
        }
    }

    external fun test(input: Int): Int

}