package com.example.ndkeducation.utils

import android.graphics.Bitmap

class BitmapUtils {
    private lateinit var bitmap: Bitmap
    private var height: Int = 0
    private var width: Int = 0

    fun getBitmap() = bitmap

    fun setBitmap(bitmap: Bitmap) {
        this.bitmap = bitmap
        bitmap.apply {
            this@BitmapUtils.height = height
            this@BitmapUtils.width = width
        }
    }

    fun createBitmap(pixels: IntArray): Bitmap =
        Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888).apply {
            setPixels(pixels, 0, width, 0, 0, width, height)
        }

    fun getPixels(): IntArray {
        val pixels = IntArray(width * height)
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height)
        return pixels
    }

    companion object {
        @Volatile
        private var instance: BitmapUtils? = null

        fun getInstance(): BitmapUtils {
            return instance
                ?: synchronized(this) {
                instance
                    ?: BitmapUtils().also {
                        instance = it
                    }
            }
        }
    }
}