#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

struct AppContext {
    ANativeWindow* window = nullptr;
    std::thread thread;
    std::atomic<bool> running{false};
};


//
//// Global EGL state
//
//
//// Global GL objects

//
//// Initialization
//bool initGraphics(AppContext* window);
//
//// Rendering helpers
//void renderRedScreen();
//void flip();
//void initSquare();
//void renderSquare();

#endif // GRAPHICS_UTILS_H