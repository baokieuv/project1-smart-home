package com.example.project1_stage2.adapter

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import android.widget.ToggleButton
import androidx.recyclerview.widget.RecyclerView
import com.example.project1_stage2.DataViewModel
import com.example.project1_stage2.R
import com.example.project1_stage2.data.DoorData
import com.example.project1_stage2.databinding.DoorRecycleBinding
import com.google.android.material.dialog.MaterialAlertDialogBuilder

class DoorAdapter(private val context: Context, private val dataset: List<DoorData>, private val viewModel: DataViewModel)
    : RecyclerView.Adapter<DoorAdapter.DoorViewHolder>() {

    class DoorViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val textView: TextView = view.findViewById(R.id.door_description)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DoorViewHolder {
        val binding = DoorRecycleBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return DoorViewHolder(binding.root)
    }

    override fun getItemCount(): Int {
        return DataViewModel.door_data.size
    }

    override fun onBindViewHolder(holder: DoorViewHolder, position: Int) {
        val item = dataset[position]
        holder.textView.text = item.description

        val image: ImageView = holder.itemView.findViewById(R.id.door)
        val mode: ToggleButton = holder.itemView.findViewById(R.id.door_mode)
        val delete: ImageView = holder.itemView.findViewById(R.id.delete_door)
        val imageMain: ImageView = holder.itemView.findViewById(R.id.image_main)

        if (item.status) {
            image.setImageResource(R.drawable.opened_door)
        } else {
            image.setImageResource(R.drawable.closed_door)
        }

        if(item.primary){
            imageMain.visibility = View.VISIBLE
        }else {
            imageMain.visibility = View.INVISIBLE
        }

        mode.isChecked = item.mode

       image.setOnClickListener {
            if (item.status) {
                viewModel.publishMessage("esp/door", "${item.id}+CLOSE")
                image.setImageResource(R.drawable.closed_door)
                item.status = false
            } else {
                viewModel.publishMessage("esp/door", "${item.id}+OPEN")
                image.setImageResource(R.drawable.opened_door)
                item.status = true
            }
        }

        mode.setOnClickListener {
            if (mode.isChecked) {
                viewModel.publishMessage("esp/door/mode", "${item.id}AUTO")
                item.mode = true
            } else {
                viewModel.publishMessage("esp/door/mode", "${item.id}+NORMAL")
                item.mode = false
            }
        }

        delete.setOnClickListener {
            MaterialAlertDialogBuilder(context)
                .setTitle("Delete door")
                .setMessage("Do you want to delete this door?")
                .setCancelable(false)
                .setNegativeButton("NO") { _, _ ->

                }
                .setPositiveButton("YES") { _, _ ->
                    DataViewModel.door_data.remove(item)
                    this.notifyItemRemoved(position)
                    viewModel.publishMessage("esp/delete", "DOOR+${item.id}")
                    Toast.makeText(context, "Delete successfully!", Toast.LENGTH_SHORT).show()
                }
                .show()
        }
    }
}