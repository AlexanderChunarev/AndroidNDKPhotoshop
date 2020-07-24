package com.example.ndkeducation.filters

import java.nio.ByteBuffer

class SharpnessFilter : BaseFilter() {
    override val name: String
        get() = "Sharpness"

    override fun processImage() {
        sharpness(byteBuffer)
    }

    private external fun sharpness(byteBuffer: ByteBuffer)

    companion object {
        @Volatile
        private var instance: SharpnessFilter? = null

        fun getInstance(): SharpnessFilter {
            return instance
                ?: synchronized(this) {
                    instance
                        ?: SharpnessFilter().also {
                            instance = it
                        }
                }
        }
    }
}