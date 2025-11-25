package com.nordcurrent.breakout

import android.content.Context
import android.content.res.AssetManager
import android.view.MotionEvent
import android.view.Surface
import android.view.SurfaceView
import android.view.SurfaceHolder

class GameView(context: Context) : SurfaceView(context), SurfaceHolder.Callback {

    init { holder.addCallback(this) }

    private var nativeHandle: Long = 0

    external fun nativeStart(surface: Surface, mgr: AssetManager?): Long
    external fun nativeStop(handle: Long)

    private external fun nativeOnTouch(pointerId: Int, x: Float, y: Float, action: Int)

    override fun surfaceCreated(holder: SurfaceHolder) {

        nativeHandle = nativeStart(holder.surface, context.resources.assets)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        nativeStop(nativeHandle)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val pointerIndex = event.actionIndex
        val pointerId = event.getPointerId(pointerIndex)
        val x = event.getX(pointerIndex)
        val y = event.getY(pointerIndex)
        val action = event.actionMasked

        nativeOnTouch(pointerId, x, y, action)
        return true
    }

    override fun surfaceChanged(h: SurfaceHolder, f: Int, w: Int, h2: Int) {}
}