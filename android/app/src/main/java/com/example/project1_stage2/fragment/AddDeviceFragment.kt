package com.example.project1_stage2.fragment

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.fragment.app.activityViewModels
import androidx.navigation.NavController
import androidx.navigation.NavHostController
import androidx.navigation.findNavController
import com.example.project1_stage2.DataViewModel
import com.example.project1_stage2.R
import com.example.project1_stage2.data.DoorData
import com.example.project1_stage2.data.FanData
import com.example.project1_stage2.data.LedData
import com.example.project1_stage2.databinding.FragmentAddDeviceBinding


class AddDeviceFragment : Fragment() {

    private val viewModel: DataViewModel by activityViewModels()
    private lateinit var binding: FragmentAddDeviceBinding
    private lateinit var navHost: NavController

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        // Inflate the layout for this fragment
        binding = FragmentAddDeviceBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val devices = resources.getStringArray(R.array.Devices)
        val pinNumbers = resources.getStringArray(R.array.PinNumbers)
        navHost = view.findNavController()

        val adapter = context?.let { ArrayAdapter(it, R.layout.spinner_item, devices)}
        binding.device.adapter = adapter
        binding.pinNumber.adapter = context?.let { ArrayAdapter(it, R.layout.spinner_item, pinNumbers) }

        binding.device.onItemSelectedListener = object : AdapterView.OnItemSelectedListener{
            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                Toast.makeText(context, devices[position], Toast.LENGTH_SHORT).show()
                when(devices[position]){
                    "Led" -> {
                        binding.imageReview.setImageResource(R.drawable.led_off)
                        binding.mainDoor.visibility = View.INVISIBLE
                    }
                    "Fan" -> {
                        binding.imageReview.setImageResource(R.drawable.fan_off)
                        binding.mainDoor.visibility = View.INVISIBLE
                    }
                    "Door" -> {
                        binding.imageReview.setImageResource(R.drawable.closed_door)
                        binding.mainDoor.visibility = View.VISIBLE
                    }
                }
            }

            override fun onNothingSelected(parent: AdapterView<*>?) {

            }

        }
        binding.pinNumber.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                Toast.makeText(context, pinNumbers[position], Toast.LENGTH_SHORT).show()
            }

            override fun onNothingSelected(parent: AdapterView<*>?) {
                TODO("Not yet implemented")
            }

        }

        binding.close.setOnClickListener {
            navHost.popBackStack()
        }

        binding.check.setOnClickListener {
            when(binding.device.selectedItem.toString()){
                "Led" -> {
                    val item = LedData(5, binding.descriptionText.text.toString() , binding.pinNumber.selectedItem.toString(), 0, true)
                    DataViewModel.led_data.add(item)
                }
                "Fan" -> {
                    val item = FanData(5, binding.descriptionText.text.toString() , binding.pinNumber.selectedItem.toString(), 0, true)
                    DataViewModel.fan_data.add(item)
                }
                "Door" -> {
                    val item = DoorData(5, binding.descriptionText.text.toString() , binding.pinNumber.selectedItem.toString(), false, false, binding.mainDoor.isChecked)
                    DataViewModel.door_data.add(item)
                }
            }
            showSuccessDialog()
        }
    }

    private fun showSuccessDialog(){
        val builder = context?.let { AlertDialog.Builder(it) }
        builder?.setTitle("Notice")
        builder?.setMessage("Add device successfully!")
        builder?.setPositiveButton("OK") { dialog, _ ->
            dialog.dismiss()
        }
        builder?.setOnDismissListener {
            navHost.popBackStack()
        }
        builder?.create()?.show()
    }
}