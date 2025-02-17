package com.example.project1_stage2.adapter

import android.content.Context
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.SeekBar
import android.widget.TextView
import android.widget.Toast
import androidx.recyclerview.widget.RecyclerView
import com.example.project1_stage2.DataViewModel
import com.example.project1_stage2.R
import com.example.project1_stage2.data.LedData
import com.example.project1_stage2.databinding.LedRecycleBinding
import com.google.android.material.dialog.MaterialAlertDialogBuilder

class LedAdapter(private val context: Context, private val dataset: List<LedData>, private val viewModel: DataViewModel)
    : RecyclerView.Adapter<LedAdapter.LedViewHolder>(){

    class LedViewHolder(view: View): RecyclerView.ViewHolder(view){
        val textView: TextView = view.findViewById(R.id.led_description)

    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): LedViewHolder {
        val binding = LedRecycleBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return LedViewHolder(binding.root)
    }

    override fun getItemCount(): Int {
            return DataViewModel.led_data.size
    }

    override fun onBindViewHolder(holder: LedViewHolder, position: Int) {
        val item = dataset[position]

        val image: ImageView = holder.itemView.findViewById<ImageView>(R.id.led)
        val bar: SeekBar = holder.itemView.findViewById(R.id.control_led)
        val delete: ImageView = holder.itemView.findViewById(R.id.delete_led)

        holder.textView.text = item.description
        if(item.status) {
            image.setImageResource(R.drawable.led_off)
        }else{
            image.setImageResource(R.drawable.led_on)
        }

        bar.progress = item.progress

        image.setOnClickListener {
            if (item.status) {
                viewModel.publishMessage("esp/led", "${item.id}+ON")
                image.setImageResource(R.drawable.led_on)
                bar.progress = 50
                item.progress = 50
                item.status = false
            } else {
                viewModel.publishMessage("esp/led", "${item.id}+OFF")
                image.setImageResource(R.drawable.led_off)
                bar.progress = 0
                item.progress = 0
                item.status = true
            }
        }

        bar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                if (fromUser) {
                    handleSeekBarProgress(progress, item, image)
                }
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
                Toast.makeText(context, "Start Tracking", Toast.LENGTH_SHORT).show()
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                Toast.makeText(context, "Stop Tracking", Toast.LENGTH_SHORT).show()
            }

        })

        delete.setOnClickListener {
            MaterialAlertDialogBuilder(context)
                .setTitle("Delete led")
                .setMessage("Do you want to delete this led?")
                .setCancelable(false)
                .setNegativeButton("NO") { _, _ ->

                }
                .setPositiveButton("YES") { _, _ ->
                    DataViewModel.led_data.remove(item)
                    this.notifyItemRemoved(position)
                    viewModel.publishMessage("esp/delete", "LED+${item.id}")
                    Toast.makeText(context, "Delete successfully!", Toast.LENGTH_SHORT).show()
                }
                .show()
        }
    }

    private fun handleSeekBarProgress(progress: Int, item: LedData, image: ImageView) {
        if (progress == 0) {
            image.setImageResource(R.drawable.led_off)
            viewModel.publishMessage("esp/led", "${item.id}+OFF")
            item.status = true
            Handler(Looper.getMainLooper()).postDelayed({
                Log.d("Delay", "Delay 1ms")
            }, 10)
        } else {
            image.setImageResource(R.drawable.led_on)
            item.status = false
            viewModel.publishMessage("esp/led/brightness", "${item.id}+$progress")
        }
        item.progress = progress
        Toast.makeText(context, "Tracking: $progress", Toast.LENGTH_SHORT).show()
    }

}