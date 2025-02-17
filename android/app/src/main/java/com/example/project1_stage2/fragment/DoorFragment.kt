package com.example.project1_stage2.fragment

import android.annotation.SuppressLint
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.navigation.findNavController
import com.example.project1_stage2.DataViewModel
import com.example.project1_stage2.R
import com.example.project1_stage2.adapter.DoorAdapter
import com.example.project1_stage2.databinding.FragmentDoorBinding
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.MqttMessage


class DoorFragment : Fragment() {

    private val viewModel: DataViewModel by activityViewModels()
    private lateinit var bindingDoor: FragmentDoorBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        // Inflate the layout for this fragment
        bindingDoor = FragmentDoorBinding.inflate(inflater, container, false)
        bindingDoor.doorRecyclerView.adapter =
            context?.let { DoorAdapter(it, DataViewModel.door_data, viewModel) }

        return bindingDoor.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        //while(viewModel.client == null);

        bindingDoor.addButton.setOnClickListener {
            val navHost = view.findNavController()
            navHost.navigate(R.id.action_doorFragment_to_addDeviceFragment)
        }

        viewModel.client?.setCallback(object : MqttCallback {
            override fun connectionLost(cause: Throwable?) {}

            @SuppressLint("DefaultLocale")
            override fun messageArrived(topic: String, message: MqttMessage) {
                val myData = message.toString()
                viewModel.processMessage(topic, message)
                if(topic == "esp/door/status"){
                    val result = myData.split("+")
                    val index = DataViewModel.door_data.indexOfFirst { it.id == result[0].toInt() }
                    bindingDoor.doorRecyclerView.adapter?.notifyItemChanged(index)
                }
            }

            override fun deliveryComplete(token: IMqttDeliveryToken?) {}
        })
    }

}