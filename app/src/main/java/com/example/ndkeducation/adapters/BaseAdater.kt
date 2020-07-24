package com.example.ndkeducation.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView

abstract class BaseAdapter<T>(
    private val items: ArrayList<T>,
    private val callback: AdapterCallback<T>?
) : RecyclerView.Adapter<BaseAdapter<T>.ViewHolder>() {
    private var layout: Int? = null

    abstract fun setResources(view: View, obj: T, position: Int)

    fun setItemLayout(layout: Int) {
        this.layout = layout
    }

    fun addItems(items: ArrayList<T>) {
        this.items.apply {
            clear()
            addAll(items)
        }
        notifyDataSetChanged()
    }

    interface AdapterCallback<T> {
        fun onItemClickListener(any: T)
    }

    inner class ViewHolder(view: View) : RecyclerView.ViewHolder(view), View.OnClickListener {
        private var obj: T? = null
        private var clickView: View? = null
        private val rootView = view

        init {
            view.setOnClickListener(this)
        }

        fun onBind(current: T) {
            this.obj = current
            setResources(rootView, current, adapterPosition)
        }

        override fun onClick(v: View?) {
            callback?.let {
                callback.onItemClickListener(obj!!)
            }
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view =
            LayoutInflater.from(parent.context).inflate(layout!!, parent, false)
        return ViewHolder(view)
    }

    override fun getItemCount(): Int {
        return items.size
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.onBind(items[position])
    }
}