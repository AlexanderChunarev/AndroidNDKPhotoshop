package com.example.ndkeducation.models

import com.example.ndkeducation.filters.*

interface FiltersFactory {
    fun invertFilter(): InvertFilter

    fun sepiaFilter(): SepiaFilter

    fun grayScaleFilter(): GrayScaleFilter

    fun blurFilter(): BlurFilter

    fun sharpnessFilter(): SharpnessFilter
}