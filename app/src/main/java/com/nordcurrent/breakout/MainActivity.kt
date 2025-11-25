package com.nordcurrent.breakout

import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.nordcurrent.breakout.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private external fun nativeInit()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

//        binding = ActivityMainBinding.inflate(layoutInflater)
//        setContentView(binding.root)
//
//        // Example of a call to a native method
//        binding.sampleText.text = stringFromJNI()



        // deprecated: if it's not broken - don't fix it
        val decorView = getWindow().getDecorView()
        decorView.setSystemUiVisibility(
            (View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                    or View.SYSTEM_UI_FLAG_FULLSCREEN
                    or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION)
        )

        setContentView(GameView(this))
    }

    companion object {
        // Used to load the 'breakout' library on application startup.
        init {
            System.loadLibrary("breakout")
        }
    }
}