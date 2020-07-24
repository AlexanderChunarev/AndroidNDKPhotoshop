package com.example.ndkeducation.filters

import android.graphics.Bitmap
import java.nio.ByteBuffer

class GrayScaleFilter : BaseFilter() {

    private external fun grayScale(byteBuffer: ByteBuffer)

    override val name: String
        get() = "GrayScale"

    override fun processImage() {
        grayScale(byteBuffer)
    }

    companion object {
        @Volatile
        private var instance: GrayScaleFilter? = null

        fun getInstance(): GrayScaleFilter {
            return instance
                ?: synchronized(this) {
                    instance
                        ?: GrayScaleFilter().also {
                            instance = it
                        }
                }
        }
    }
}