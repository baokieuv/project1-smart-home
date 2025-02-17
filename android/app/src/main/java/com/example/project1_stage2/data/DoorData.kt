package com.example.project1_stage2.data

data class DoorData (
    val id: Int,
    val description: String,
    val gpioPin: String,
    var mode: Boolean,
    var status: Boolean,
    var primary: Boolean
    )