#include <jni.h>
#include <string>

#include <jni.h>

#include <android/log.h>
#include <thread>

#include "Session.h"
#include "EmptyModule.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

void mainLoop() {

    const double dt = 1.0 / 60.0;
    auto last = std::chrono::steady_clock::now();

    Session session;
    session.addModule(std::make_shared<EmptyModule>());
    session.start();

    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto next = last + std::chrono::duration<double>(dt);

        if (now >= next) {
            last = now;
            session.update();
            session.render();
        }

        std::this_thread::sleep_until(next);
    }

    session.shutdown();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStart(JNIEnv* env, jobject, jobject surface) {
    g_window = ANativeWindow_fromSurface(env, surface);
    running = true;
    g_thread = std::thread(mainLoop);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStop(JNIEnv*, jobject) {
    running = false;
    if (g_thread.joinable()) g_thread.join();
}

