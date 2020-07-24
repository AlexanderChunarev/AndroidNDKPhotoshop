package com.example.ndkeducation.filters

import java.nio.ByteBuffer

class BlurFilter : BaseFilter() {
    override val name: String
        get() = "Blur"

    override fun processImage() {
        jniBlur(byteBuffer, 5)
    }

    private external fun jniBlur(byteBuffer: ByteBuffer, radius: Int)

    companion object {
        @Volatile
        private var instance: BlurFilter? = null

        fun getInstance(): BlurFilter {
            return instance
                ?: synchronized(this) {
                    instance
                        ?: BlurFilter().also {
                            instance = it
                        }
                }
        }
    }
}