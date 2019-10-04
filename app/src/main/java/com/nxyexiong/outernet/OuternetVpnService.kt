package com.nxyexiong.outernet

import android.app.Service
import android.content.Intent
import android.net.VpnService
import android.os.Binder
import android.os.ParcelFileDescriptor
import android.util.Log
import kotlin.system.exitProcess

class OuternetVpnService: VpnService() {
    companion object {
        const val TAG = "OuternetVpnService"
        const val ACTION_CONNECT = "action_connect"
        const val ACTION_DISCONNECT = "action_disconnect"
        const val CONNECT_TIMEOUT = 10
    }

    private var _vpnInterface: ParcelFileDescriptor? = null
    private var _inited = false
    private var _connected = false

    override fun onCreate() {
        super.onCreate()
        Log.i(TAG, "vpn onCreate")
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        if (intent != null && intent.action == ACTION_CONNECT) {
            if (_inited) {
                return Service.START_STICKY
            }
            connect()
            if (_inited) {
                return Service.START_STICKY
            } else {
                return Service.START_NOT_STICKY
            }
        } else if (intent != null && intent.action == ACTION_DISCONNECT) {
            disconnect()
            return Service.START_NOT_STICKY
        }
        return Service.START_NOT_STICKY
    }

    private fun connect() {
        _inited = true
        val prefs = getSharedPreferences(MainActivity.prefsName, MODE_PRIVATE)
        val addr = prefs.getString(MainActivity.prefsAddr, "")
        val port = prefs.getString(MainActivity.prefsPort, "0")!!.toInt()
        val user = prefs.getString(MainActivity.prefsUser, "")
        val secret = prefs.getString(MainActivity.prefsSecret, "")

        NativeHelper.newTunnelCallback = { tunIp, dstIp ->
            _connected = establish(tunIp, dstIp)
        }

        NativeHelper.initClient(addr!!, port, user!!, secret!!)
        NativeHelper.runClient()

        var sec = 0
        while (sec < CONNECT_TIMEOUT && !_connected) {
            NativeHelper.checkHandshake()
        }

        if (!_connected) {
            disconnect()
        }
    }

    private fun disconnect() {
        NativeHelper.stopClient()
        NativeHelper.uninitClient()
        _connected = false
        _inited = false
        _vpnInterface?.close()
        _vpnInterface = null
        stopSelf()
        exitProcess(0)
    }

    private fun establish(tunIp: String, dstIp: String): Boolean {
        val builder = this.Builder()
        builder.setSession("Outernet")
        builder.setMtu(1300)
        builder.addAddress(dstIp, 24)
        builder.addRoute("0.0.0.0", 0)
        builder.addDnsServer("8.8.8.8")
        builder.addRoute("8.8.8.8", 32)
        builder.addDisallowedApplication(applicationContext.packageName)

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

        NativeHelper.establishClient(fd)

        return true
    }

}