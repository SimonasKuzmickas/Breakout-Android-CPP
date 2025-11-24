#include <jni.h>
#include <string>

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <thread>
#include <atomic>

#include "graphics.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

ANativeWindow* g_window = nullptr;
std::thread g_thread;
std::atomic<bool> running = false;

extern "C" JNIEXPORT jstring JNICALL
Java_com_nordcurrent_breakout_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void gameLoop() {
    initGraphics(g_window);
    initSquare();
    const double dt = 1.0 / 60.0;
    auto last = std::chrono::steady_clock::now();

    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration<double>(now - last);

        if (elapsed.count() >= dt) {
            last = now;

        }

        renderRedScreen();
        renderSquare();
        flip();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStart(JNIEnv* env, jobject, jobject surface) {
    g_window = ANativeWindow_fromSurface(env, surface);
    running = true;
    g_thread = std::thread(gameLoop);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStop(JNIEnv*, jobject) {
    running = false;
    if (g_thread.joinable()) g_thread.join();
}

