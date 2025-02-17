package com.example.project1_stage2.fragment

import android.annotation.SuppressLint
import android.graphics.Color
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
import com.example.project1_stage2.databinding.FragmentWelcomeBinding
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.MqttMessage

class WelcomeFragment : Fragment() {
    private lateinit var bindingWel: FragmentWelcomeBinding
    private val viewModel: DataViewModel by activityViewModels()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        // Inflate the layout for this fragment
        //binding = DataBindingUtil.inflate(inflater, R.layout.fragment_welcome, container, false)
        bindingWel = FragmentWelcomeBinding.inflate(inflater, container, false)
        Log.d("my_log", "on create view")
        return bindingWel.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        Log.d("my_log", "on viewCreated")
        viewModel.tempData.color = Color.RED
        viewModel.humData.color = Color.BLUE

        bindingWel.temperature.progress = viewModel.tempVal.toInt()
        bindingWel.humidity.setStartColor(Color.CYAN)
        bindingWel.humidity.setEndColor(Color.BLUE)
        bindingWel.humidity.progress = viewModel.humVal.toInt()

        setupGraphs()

        bindingWel.humiSwitch.isChecked = viewModel.is_hum_check
        bindingWel.tempSwitch.isChecked = viewModel.is_temp_check

        val navHost = view.findNavController()

        bindingWel.ledView.setOnClickListener{
            navHost.navigate(R.id.action_welcomeFragment_to_ledFragment)
        }
        bindingWel.doorView.setOnClickListener {
            navHost.navigate(R.id.action_welcomeFragment_to_doorFragment)
        }
        bindingWel.fanView.setOnClickListener {
            navHost.navigate(R.id.action_welcomeFragment_to_fanFragment)
        }

        viewModel.client?.setCallback(object : MqttCallback {
            override fun connectionLost(cause: Throwable?) {
                Log.d("my_log", "connection lost")
            }

            @SuppressLint("DefaultLocale")
            override fun messageArrived(topic: String, message: MqttMessage) {
                //processMessage(topic, message)
                viewModel.processMessage(topic, message)
                bindingWel.temperatureText.text = String.format("%s°C", viewModel.tempVal)
                bindingWel.humidityText.text = String.format("%s%%", viewModel.humVal)
                bindingWel.humidity.progress = viewModel.humVal.toInt()
                bindingWel.temperature.progress = (viewModel.tempVal.toInt() * 2)
            }

            override fun deliveryComplete(token: IMqttDeliveryToken?) {
                Log.d("my_log", "delivery complete")
            }
        })

        bindingWel.tempSwitch.setOnClickListener {
            bindingWel.temperatureGraph.visibility = if (bindingWel.tempSwitch.isChecked) View.VISIBLE else View.INVISIBLE
            bindingWel.temperature.visibility = if(bindingWel.tempSwitch.isChecked) View.INVISIBLE else View.VISIBLE
            bindingWel.temperatureText.visibility = if(bindingWel.tempSwitch.isChecked) View.INVISIBLE else View.VISIBLE

            viewModel.is_temp_check = bindingWel.tempSwitch.isChecked
        }

        bindingWel.humiSwitch.setOnClickListener {
            bindingWel.humidityGraph.visibility = if (bindingWel.humiSwitch.isChecked) View.VISIBLE else View.INVISIBLE
            bindingWel.humidity.visibility = if (bindingWel.humiSwitch.isChecked) View.INVISIBLE else View.VISIBLE
            bindingWel.humidityText.visibility = if (bindingWel.humiSwitch.isChecked) View.INVISIBLE else View.VISIBLE

            viewModel.is_hum_check = bindingWel.humiSwitch.isChecked
        }

        bindingWel.temperatureGraph.visibility = if (bindingWel.tempSwitch.isChecked) View.VISIBLE else View.INVISIBLE
        bindingWel.temperature.visibility = if(bindingWel.tempSwitch.isChecked) View.INVISIBLE else View.VISIBLE
        bindingWel.temperatureText.visibility = if(bindingWel.tempSwitch.isChecked) View.INVISIBLE else View.VISIBLE

        bindingWel.humidityGraph.visibility = if (bindingWel.humiSwitch.isChecked) View.VISIBLE else View.INVISIBLE
        bindingWel.humidity.visibility = if (bindingWel.humiSwitch.isChecked) View.INVISIBLE else View.VISIBLE
        bindingWel.humidityText.visibility = if (bindingWel.humiSwitch.isChecked) View.INVISIBLE else View.VISIBLE

        Log.d("my_log", "${bindingWel.tempSwitch.isChecked}")
    }

    private fun setupGraphs() {
        bindingWel.temperatureGraph.viewport.apply {
            isYAxisBoundsManual = true
            setMinY(0.0)
            setMaxY(50.0)
        }
        bindingWel.temperatureGraph.gridLabelRenderer.isHorizontalLabelsVisible = false
        bindingWel.temperatureGraph.addSeries(viewModel.tempData)

        bindingWel.humidityGraph.viewport.apply {
            isYAxisBoundsManual = true
            setMinY(0.0)
            setMaxY(100.0)
        }
        bindingWel.humidityGraph.gridLabelRenderer.isHorizontalLabelsVisible = false
        bindingWel.humidityGraph.addSeries(viewModel.humData)
    }
}