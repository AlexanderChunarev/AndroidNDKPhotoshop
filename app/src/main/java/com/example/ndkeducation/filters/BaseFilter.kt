package com.example.ndkeducation.filters

import android.graphics.Bitmap
import com.example.ndkeducation.utils.BitmapUtils
import java.nio.ByteBuffer

abstract class BaseFilter {
    private val bitmapHolder = BitmapUtils.getInstance()
    protected val byteBuffer by lazy { jniStoreBitmapData(bitmapHolder.getBitmap()) }

    abstract val name: String

    abstract fun processImage()

    fun apply(): Bitmap {
        processImage()
        return jniGetBitmap(byteBuffer)
    }

    private external fun jniStoreBitmapData(bitmap: Bitmap): ByteBuffer

    private external fun jniGetBitmap(byteBuffer: ByteBuffer): Bitmap

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}