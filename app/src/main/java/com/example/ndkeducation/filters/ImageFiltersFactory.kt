package com.example.ndkeducation.filters

import com.example.ndkeducation.models.FiltersFactory

class ImageFiltersFactory : FiltersFactory {
    override fun invertFilter() = InvertFilter.getInstance()

    override fun sepiaFilter() = SepiaFilter.getInstance()

    override fun grayScaleFilter() = GrayScaleFilter.getInstance()

    override fun blurFilter() = BlurFilter.getInstance()

    override fun sharpnessFilter() = SharpnessFilter.getInstance()
}