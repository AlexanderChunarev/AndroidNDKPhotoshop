package com.example.ndkeducation.viewmodels

import android.graphics.Bitmap
import androidx.lifecycle.ViewModel
import com.example.ndkeducation.repositories.ImageProcessingRepository
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class ImageProcessingViewModel : ViewModel() {
    private val imageProcessingRepository by lazy { ImageProcessingRepository() }
    val imageLiveData by lazy { SingleLiveEvent<Bitmap>() }

    fun performBitmapInvert() = GlobalScope.launch {
        val bitmap = imageProcessingRepository.applyInvertFiler()
        withContext(Dispatchers.Main) {
            imageLiveData.value = bitmap
        }
    }

    fun performBitmapSepia() = GlobalScope.launch {
        val bitmap = imageProcessingRepository.applySepiaFiler()
        withContext(Dispatchers.Main) {
            imageLiveData.value = bitmap
        }
    }

    fun performBitmapGrayScale() = GlobalScope.launch {
        val bitmap = imageProcessingRepository.applyGrayScaleFiler()
        withContext(Dispatchers.Main) {
            imageLiveData.value = bitmap
        }
    }

    fun performBitmapBlur() = GlobalScope.launch {
        val bitmap = imageProcessingRepository.applyBlurFilter()
        withContext(Dispatchers.Main) {
            imageLiveData.value = bitmap
        }
    }

    fun performBitmapSharpness() = GlobalScope.launch {
        val bitmap = imageProcessingRepository.applySharpnessFilter()
        withContext(Dispatchers.Main) {
            imageLiveData.value = bitmap
        }
    }
}