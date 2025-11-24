package com.nordcurrent.breakout

import android.content.Context
import android.view.Surface
import android.view.SurfaceView
import android.view.SurfaceHolder

class GameView(context: Context) : SurfaceView(context), SurfaceHolder.Callback {

    init { holder.addCallback(this) }

    external fun nativeStart(surface: Surface)
    external fun nativeStop()

    override fun surfaceCreated(holder: SurfaceHolder) {
        nativeStart(holder.surface)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        nativeStop()
    }

    override fun surfaceChanged(h: SurfaceHolder, f: Int, w: Int, h2: Int) {}
}