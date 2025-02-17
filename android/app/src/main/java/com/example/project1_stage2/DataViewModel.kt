package com.example.project1_stage2

import android.util.Log
import androidx.lifecycle.ViewModel
import com.example.project1_stage2.data.DoorData
import com.example.project1_stage2.data.FanData
import com.example.project1_stage2.data.LedData
import com.jjoe64.graphview.series.DataPoint
import com.jjoe64.graphview.series.LineGraphSeries
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.IMqttActionListener
import org.eclipse.paho.client.mqttv3.IMqttToken
import org.eclipse.paho.client.mqttv3.MqttConnectOptions
import org.eclipse.paho.client.mqttv3.MqttException
import org.eclipse.paho.client.mqttv3.MqttMessage
import java.nio.charset.StandardCharsets

class DataViewModel: ViewModel() {
    var client: MqttAndroidClient? = null
    var options: MqttConnectOptions? = null
    val tempData = LineGraphSeries<DataPoint>()
    val humData = LineGraphSeries<DataPoint>()
    var tempVal: String = "20"
    var humVal: String = "80"
    private var time: Long = 0
    var is_temp_check = false
    var is_hum_check = false

    companion object{
        val led_data: MutableList<LedData> = mutableListOf(
            LedData(1, "home", "GPIOA_PIN_10", 0, true),
            LedData(2, "kitchen", "GPIOA_PIN_12", 0, true)
        )
        val door_data: MutableList<DoorData> = mutableListOf(
            DoorData(1, "home", "GPIOA_PIN_11", false, false, true),
            DoorData(2, "kitchen", "GPIOA_PIN_12", false, false, false)
        )
        val fan_data: MutableList<FanData> = mutableListOf(
            FanData(1, "home", "GPIOA_PIN_10", 0, true),
            FanData(2, "kitchen", "GPIOA_PIN_12", 0, true)
        )
    }
    fun subscribeToTopic(topic: String) {
        val qos = 1
        try {
            val subToken = client?.subscribe(topic, qos)
            subToken?.actionCallback = object : IMqttActionListener {
                override fun onSuccess(asyncActionToken: IMqttToken?) {
                    Log.d("my_log", "Subscription successful $topic")
                }

                override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                    Log.d("my_log", "Subscription failed")
                }
            }
        } catch (e: MqttException) {
            e.printStackTrace()
        }
    }

    fun publishMessage(topic: String, payload: String) {
        val encodedPayload = payload.toByteArray(StandardCharsets.UTF_8)
        val message = MqttMessage(encodedPayload)
        try {
            if(client == null) Log.d("my_log", "client is null")
            client?.publish(topic, message)
            Log.d("my_log", "publish successful $topic $payload")
        } catch (e: MqttException) {
            throw RuntimeException(e)
        }
    }

    fun processMessage(topic: String, message: MqttMessage) {
        val myData = message.toString()
        Log.d("my_log", myData)
        if(topic == "esp/dht11"){
            val result = myData.split(" ")
            tempVal = result[0]
            humVal = result[1]

            tempData.appendData(DataPoint(time.toDouble(), tempVal.toDouble()), false, 10)
            humData.appendData(DataPoint(time.toDouble(), humVal.toDouble()), false, 10)
            time++
        }
        else if(topic == "esp/door/status"){
            val result = myData.split("+")
            val data = door_data.find {
                it.id == result[0].toInt()
            }
            data?.status = result[1] == "OPEN"
        }
    }
}