package com.nxyexiong.outernet

import android.app.Service
import android.content.Intent
import android.net.VpnService
import android.os.ParcelFileDescriptor
import android.util.Log

class OuternetVpnService: VpnService() {
    companion object {
        const val TAG = "OuternetVpnService"
        const val ACTION_CONNECT = "action_connect"
        const val ACTION_DISCONNECT = "action_disconnect"
    }

    private var _vpnInterface: ParcelFileDescriptor? = null

    override fun onCreate() {
        super.onCreate()
        Log.i(TAG, "vpn onCreate")
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        if (intent != null && intent.action == ACTION_CONNECT) {
            connect()
            return Service.START_STICKY
        } else if (intent != null && intent.action == ACTION_DISCONNECT) {
            disconnect()
            return Service.START_NOT_STICKY
        }
        return Service.START_NOT_STICKY
    }

    private fun connect() {
        val prefs = getSharedPreferences(MainActivity.prefsName, MODE_PRIVATE)
        val addr = prefs.getString(MainActivity.prefsAddr, "")
        val port = prefs.getString(MainActivity.prefsPort, "0")!!.toInt()
        val user = prefs.getString(MainActivity.prefsUser, "")
        val secret = prefs.getString(MainActivity.prefsSecret, "")

        // TODO: do connect things
    }

    private fun disconnect() {
        // TODO: disconnect
    }

    private fun establish(virtualIp: String): Boolean {
        val builder = this.Builder()
        builder.setSession("Outernet")
        builder.setMtu(1500)
        builder.addAddress(virtualIp, 24)
        builder.addRoute("0.0.0.0", 0)
        builder.addDnsServer("8.8.8.8")
        builder.addRoute("8.8.8.8", 32)

        Log.i(TAG, "vpn start establish")

        _vpnInterface = builder.establish()
        if (_vpnInterface == null) {
            Log.e(TAG, "vpn establish failed")
            return false
        }

        val fd = _vpnInterface!!.fd
        if (fd <= 0) {
            Log.e(TAG, "vpn establish fd <= 0")
            return false
        }

        // TODO: send fd to vpn client

        return true
    }

}