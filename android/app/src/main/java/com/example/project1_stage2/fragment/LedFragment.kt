package com.example.project1_stage2.fragment

import android.annotation.SuppressLint
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.navigation.findNavController
import com.example.project1_stage2.DataViewModel
import com.example.project1_stage2.R
import com.example.project1_stage2.adapter.LedAdapter
import com.example.project1_stage2.databinding.FragmentLedBinding
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.MqttMessage

class LedFragment : Fragment() {

    private val viewModel: DataViewModel by activityViewModels()

    private lateinit var bindingLed: FragmentLedBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        // Inflate the layout for this fragment
        bindingLed = FragmentLedBinding.inflate(inflater, container, false)

        bindingLed.ledRecyclerView.adapter =
            context?.let { LedAdapter(it, DataViewModel.led_data, viewModel) }

        return bindingLed.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        bindingLed.ledRecyclerView.setHasFixedSize(true)

        bindingLed.addButton.setOnClickListener {
            val navHost = view.findNavController()
            navHost.navigate(R.id.action_ledFragment_to_addDeviceFragment)
        }

        viewModel.client?.setCallback(object : MqttCallback {
            override fun connectionLost(cause: Throwable?) {
                Log.d("my_log", "connection lost")
            }

            @SuppressLint("DefaultLocale")
            override fun messageArrived(topic: String, message: MqttMessage) {
                viewModel.processMessage(topic, message)
            }

            override fun deliveryComplete(token: IMqttDeliveryToken?) {}
        })
    }
}