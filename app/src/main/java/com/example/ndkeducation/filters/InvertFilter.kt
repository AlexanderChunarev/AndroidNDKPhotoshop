package com.example.ndkeducation.filters

import java.nio.ByteBuffer

class InvertFilter : BaseFilter() {

    private external fun invert(byteBuffer: ByteBuffer)

    override val name: String
        get() = "Invert"

    override fun processImage() {
        invert(byteBuffer)
    }

    companion object {
        @Volatile
        private var instance: InvertFilter? = null

        fun getInstance(): InvertFilter {
            return instance
                ?: synchronized(this) {
                    instance
                        ?: InvertFilter().also {
                            instance = it
                        }
                }
        }
    }
}