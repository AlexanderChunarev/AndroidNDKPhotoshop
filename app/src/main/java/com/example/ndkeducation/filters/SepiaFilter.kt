package com.example.ndkeducation.filters

import java.nio.ByteBuffer

class SepiaFilter : BaseFilter() {

    private external fun sepia(byteBuffer: ByteBuffer)

    override val name: String
        get() = "Sepia"

    override fun processImage() {
        sepia(byteBuffer)
    }

    companion object {
        @Volatile
        private var instance: SepiaFilter? = null

        fun getInstance(): SepiaFilter {
            return instance
                ?: synchronized(this) {
                    instance
                        ?: SepiaFilter().also {
                            instance = it
                        }
                }
        }
    }

}