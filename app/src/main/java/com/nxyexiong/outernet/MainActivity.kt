package com.nxyexiong.outernet

import android.content.SharedPreferences
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import androidx.core.content.edit

class MainActivity : AppCompatActivity() {

    companion object {
        const val TAG = "MainActivity"
        const val prefsName = "outernet_prefs"
        const val prefsAddr = "addr"
        const val prefsPort = "port"
        const val prefsUser = "user"
        const val prefsSecret = "secret"
    }

    private lateinit var addrEdit: EditText
    private lateinit var portEdit: EditText
    private lateinit var userEdit: EditText
    private lateinit var secretEdit: EditText
    private lateinit var connectBtn: Button

    private lateinit var prefs: SharedPreferences

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        addrEdit = findViewById(R.id.addr_edit)
        portEdit = findViewById(R.id.port_edit)
        userEdit = findViewById(R.id.user_edit)
        secretEdit = findViewById(R.id.secret_edit)
        connectBtn = findViewById(R.id.connect_btn)

        prefs = getSharedPreferences(prefsName, MODE_PRIVATE)

        addrEdit.setText(prefs.getString(prefsAddr, ""))
        portEdit.setText(prefs.getString(prefsPort, ""))
        userEdit.setText(prefs.getString(prefsUser, ""))
        secretEdit.setText(prefs.getString(prefsSecret, ""))

        connectBtn.setOnClickListener {
            handleConnectBtn()
        }

    }

    private fun handleConnectBtn() {
        Log.i(TAG, "connect button clicked")
        val native = NativeHelper()
        val output = native.test(5)
        Log.i(TAG, "from jni: $output")

        val addr = addrEdit.text.toString()
        val port = portEdit.text.toString()
        val user = userEdit.text.toString()
        val secret = secretEdit.text.toString()

        // TODO: check parameters

        prefs.edit()
            .putString(prefsAddr, addr)
            .putString(prefsPort, port)
            .putString(prefsUser, user)
            .putString(prefsSecret, secret)
            .commit()

        // TODO: start vpn service
    }

}
