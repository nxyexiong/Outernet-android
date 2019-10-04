package com.nxyexiong.outernet

import android.app.Activity
import android.content.Intent
import android.content.SharedPreferences
import android.net.VpnService
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.core.content.edit

class MainActivity : AppCompatActivity() {

    companion object {
        const val TAG = "MainActivity"
        const val prefsName = "outernet_prefs"
        const val prefsAddr = "addr"
        const val prefsPort = "port"
        const val prefsUser = "user"
        const val prefsSecret = "secret"

        const val CODE_CONNECT = 1001
        const val CODE_DISCONNECT = 1002
    }

    private lateinit var addrEdit: EditText
    private lateinit var portEdit: EditText
    private lateinit var userEdit: EditText
    private lateinit var secretEdit: EditText
    private lateinit var connectBtn: Button
    private lateinit var disconnectBtn: Button

    private lateinit var prefs: SharedPreferences

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        addrEdit = findViewById(R.id.addr_edit)
        portEdit = findViewById(R.id.port_edit)
        userEdit = findViewById(R.id.user_edit)
        secretEdit = findViewById(R.id.secret_edit)
        connectBtn = findViewById(R.id.connect_btn)
        disconnectBtn = findViewById(R.id.disconnect_btn)

        prefs = getSharedPreferences(prefsName, MODE_PRIVATE)

        addrEdit.setText(prefs.getString(prefsAddr, ""))
        portEdit.setText(prefs.getString(prefsPort, ""))
        userEdit.setText(prefs.getString(prefsUser, ""))
        secretEdit.setText(prefs.getString(prefsSecret, ""))

        connectBtn.setOnClickListener {
            handleConnectBtn()
        }
        disconnectBtn.setOnClickListener {
            handleDisconnectBtn()
        }
    }

    private fun handleConnectBtn() {
        Log.i(TAG, "connect button clicked")

        val addr = addrEdit.text.toString()
        val port = portEdit.text.toString()
        val user = userEdit.text.toString()
        val secret = secretEdit.text.toString()

        if (!android.util.Patterns.IP_ADDRESS.matcher(addr).matches()) {
            Toast.makeText(this, "server ip is incorrect", Toast.LENGTH_SHORT).show()
            return
        }
        if (port.toIntOrNull() == null || port.toInt() > 65536 || port.toInt() <= 0) {
            Toast.makeText(this, "server port is incorrect", Toast.LENGTH_SHORT).show()
            return
        }

        prefs.edit()
            .putString(prefsAddr, addr)
            .putString(prefsPort, port)
            .putString(prefsUser, user)
            .putString(prefsSecret, secret)
            .commit()

        val intent = VpnService.prepare(this)
        if (intent != null) {
            startActivityForResult(intent, CODE_CONNECT)
        } else {
            onActivityResult(CODE_CONNECT, Activity.RESULT_OK, null)
        }
    }

    private fun handleDisconnectBtn() {
        Log.i(TAG, "disconnect button clicked")

        onActivityResult(CODE_DISCONNECT, Activity.RESULT_OK, null)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == CODE_CONNECT && resultCode == Activity.RESULT_OK) {
            val intent = Intent(this, OuternetVpnService::class.java)
            startService(intent.setAction(OuternetVpnService.ACTION_CONNECT))
        } else if (requestCode == CODE_DISCONNECT && resultCode == Activity.RESULT_OK) {
            val intent = Intent(this, OuternetVpnService::class.java)
            startService(intent.setAction(OuternetVpnService.ACTION_DISCONNECT))
        }
    }

}
