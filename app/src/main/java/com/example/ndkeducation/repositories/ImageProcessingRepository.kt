package com.example.ndkeducation.repositories

import com.example.ndkeducation.filters.ImageFiltersFactory
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

class ImageProcessingRepository {
    private val imageFiltersFactory by lazy { ImageFiltersFactory() }

    suspend fun applyInvertFiler() = withContext(Dispatchers.Default) {
        imageFiltersFactory.invertFilter().apply()
    }

    suspend fun applySepiaFiler() = withContext(Dispatchers.Default) {
        imageFiltersFactory.sepiaFilter().apply()
    }

    suspend fun applyGrayScaleFiler() = withContext(Dispatchers.Default) {
        imageFiltersFactory.grayScaleFilter().apply()
    }

    suspend fun applyBlurFilter() = withContext(Dispatchers.Default) {
        imageFiltersFactory.blurFilter().apply()
    }

    suspend fun applySharpnessFilter() = withContext(Dispatchers.Default) {
        imageFiltersFactory.sharpnessFilter().apply()
    }
}