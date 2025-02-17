package com.example.project1_stage2.data

data class LedData (
    val id: Int,
    val description: String,
    val gpioPin: String,
    var progress: Int,
    var status: Boolean
    )