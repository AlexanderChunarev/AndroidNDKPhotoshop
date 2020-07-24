package com.example.ndkeducation

import android.app.Activity
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.example.ndkeducation.adapters.BaseAdapter
import com.example.ndkeducation.adapters.PreviewFiltersAdapter
import com.example.ndkeducation.filters.*
import com.example.ndkeducation.utils.BitmapUtils
import com.example.ndkeducation.viewmodels.ImageProcessingViewModel
import kotlinx.android.synthetic.main.activity_main.*
import java.nio.ByteBuffer


class MainActivity : AppCompatActivity(), BaseAdapter.AdapterCallback<BaseFilter> {
    private val imageProcessingViewModel by lazy {
        ViewModelProvider(this).get(
            ImageProcessingViewModel::class.java
        )
    }
    private val adapter by lazy { PreviewFiltersAdapter(arrayListOf(), this) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val photoPickerIntent = Intent(Intent.ACTION_PICK)
        photoPickerIntent.type = "image/*"
        startActivityForResult(photoPickerIntent, 1)

        filters_recyclerView.apply {
            adapter = this@MainActivity.adapter
        }

        imageProcessingViewModel.imageLiveData.observe(this, Observer {
            selected_imageView.setImageBitmap(it)
        })
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode == Activity.RESULT_OK) {
            val imageStream = contentResolver.openInputStream(Uri.parse(data!!.data.toString()))
            val selectedBitmap = BitmapFactory.decodeStream(imageStream)
            BitmapUtils.getInstance().setBitmap(selectedBitmap)

            val list = arrayListOf<BaseFilter>()
            list.add(InvertFilter.getInstance())
            list.add(SepiaFilter.getInstance())
            list.add(BlurFilter.getInstance())
            list.add(GrayScaleFilter.getInstance())
            list.add(SharpnessFilter.getInstance())
            adapter.addItems(list)
        }
    }

    override fun onItemClickListener(any: BaseFilter) {
        when (any) {
            is InvertFilter -> imageProcessingViewModel.performBitmapInvert()
            is SepiaFilter -> imageProcessingViewModel.performBitmapSepia()
            is BlurFilter -> imageProcessingViewModel.performBitmapBlur()
            is GrayScaleFilter -> imageProcessingViewModel.performBitmapGrayScale()
            is SharpnessFilter -> imageProcessingViewModel.performBitmapSharpness()
        }
    }
}
