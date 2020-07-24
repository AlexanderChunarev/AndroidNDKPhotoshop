package com.example.ndkeducation.adapters

import android.view.View
import android.widget.ImageView
import android.widget.TextView
import com.example.ndkeducation.R
import com.example.ndkeducation.filters.BaseFilter
import com.example.ndkeducation.utils.BitmapUtils

class PreviewFiltersAdapter(items: ArrayList<BaseFilter>, callback: AdapterCallback<BaseFilter>) :
    BaseAdapter<BaseFilter>(items, callback) {

    interface ClickCallback {

    }

    init {
        setItemLayout(R.layout.filter_item)
    }

    override fun setResources(view: View, obj: BaseFilter, position: Int) {
        val previewImageView = view.findViewById<ImageView>(R.id.filter_preview_imageView)
        val previewFilterName = view.findViewById<TextView>(R.id.filter_name_text)
        previewImageView.setImageBitmap(BitmapUtils.getInstance().getBitmap())
        previewFilterName.text = obj.name
    }
}