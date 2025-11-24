package com.nordcurrent.breakout

import android.content.Context
import android.view.Surface
import android.view.SurfaceView
import android.view.SurfaceHolder

class GameView(context: Context) : SurfaceView(context), SurfaceHolder.Callback {

    init { holder.addCallback(this) }

    private var nativeHandle: Long = 0

    external fun nativeStart(surface: Surface): Long
    external fun nativeStop(handle: Long)

    override fun surfaceCreated(holder: SurfaceHolder) {
        nativeHandle = nativeStart(holder.surface)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        nativeStop(nativeHandle)
    }

    override fun surfaceChanged(h: SurfaceHolder, f: Int, w: Int, h2: Int) {}
}