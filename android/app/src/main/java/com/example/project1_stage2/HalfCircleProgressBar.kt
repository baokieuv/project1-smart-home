package com.example.project1_stage2

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Shader
import android.graphics.SweepGradient
import android.util.AttributeSet
import android.view.View
import kotlin.math.min

class HalfCircleProgressBar @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null
) : View(context, attrs) {

    private val strokePaint = Paint().apply {
        color = 0xFF00FF00.toInt() // Màu đen
        style = Paint.Style.STROKE // Chỉ vẽ đường viền
        strokeWidth = 45f // Độ dày của cung tròn
        isAntiAlias = true // Khử răng cưa
    }

    var progress: Int = 100
        set(value) {
            field = value
            invalidate()
        }
    private var startColor: Int = Color.GREEN // Màu nhạt (xanh nhạt)
    private var endColor: Int = Color.RED   // Màu đậm (xanh đậm)

    @SuppressLint("DrawAllocation")
    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        val width = width
        val height = height
        val radiusOuter = min(width, height) / 2 - 20 // Bán kính của cung tròn

        // Xác định tâm hình
        val centerX = width / 2
        val centerY = height / 2

        val shader: Shader = SweepGradient(
            width / 2f, height.toFloat(), intArrayOf(startColor, endColor), floatArrayOf(0.5f, 1f)
        )
        strokePaint.shader = shader

        // Vẽ cung tròn bên ngoài
        canvas.drawArc(
            (centerX - radiusOuter).toFloat(),
            (centerY - radiusOuter).toFloat(),
            (centerX + radiusOuter).toFloat(),
            (centerY + radiusOuter).toFloat(),
            180f, progress * 1.8f,
            false, strokePaint
        )
    }

//    fun setProgress(progress: Int) {
//        this.progress = progress.coerceIn(0, 100)
//        invalidate()
//    }

    fun setStartColor(startColor: Int) {
        this.startColor = startColor
    }

    fun setEndColor(endColor: Int) {
        this.endColor = endColor
    }
}
