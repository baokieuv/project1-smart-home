package com.example.project1_stage2

import android.os.Bundle
import android.widget.Toast
import androidx.activity.viewModels

import androidx.appcompat.app.AppCompatActivity
import com.example.project1_stage2.databinding.ActivityMainBinding
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.IMqttActionListener
import org.eclipse.paho.client.mqttv3.IMqttToken
import org.eclipse.paho.client.mqttv3.MqttClient
import org.eclipse.paho.client.mqttv3.MqttConnectOptions
import org.eclipse.paho.client.mqttv3.MqttException


class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    private val viewModel: DataViewModel by viewModels()

    companion object{
        const val MQTTHOST = "tcp://broker.hivemq.com:1883"
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        if(viewModel.client == null){
            setupMqttClient()
        }

    }
    private fun setupMqttClient() {
        val clientID = MqttClient.generateClientId()
        viewModel.client = MqttAndroidClient(applicationContext, MQTTHOST, clientID)
        viewModel.options = MqttConnectOptions()

        try {
            val token = viewModel.client?.connect(viewModel.options)
            token?.actionCallback = object : IMqttActionListener {
                override fun onSuccess(asyncActionToken: IMqttToken?) {
                    Toast.makeText(applicationContext, "Connected", Toast.LENGTH_SHORT).show()
                    viewModel.subscribeToTopic("esp/door/status")
                    viewModel.subscribeToTopic("esp/dht11")
                }

                override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                    Toast.makeText(applicationContext, "Connection failed", Toast.LENGTH_SHORT).show()
                }
            }
        } catch (e: MqttException) {
            throw RuntimeException(e)
        }
    }
}