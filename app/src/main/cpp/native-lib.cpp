#include <jni.h>
#include <android/log.h>
#include <thread>

#include "Session.h"
#include "EmptyModule.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

void mainLoop(AppContext* ctx) {

    const double delta = 1.0 / 60.0;
    auto last = std::chrono::steady_clock::now();

    Session session;
    session.addModule(std::make_shared<EmptyModule>(ctx));
    session.start();

    while (ctx->running) {
        auto now = std::chrono::steady_clock::now();
        auto next = last + std::chrono::duration<double>(delta);

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
JNIEXPORT jlong JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStart(JNIEnv* env, jobject, jobject surface) {
    auto* ctx = new AppContext();

    ctx->window = ANativeWindow_fromSurface(env, surface);
    ctx->running = true;
    ctx->thread = std::thread(mainLoop, ctx);

    return reinterpret_cast<jlong>(ctx);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStop(JNIEnv*, jobject, jlong handle) {
    auto* ctx = reinterpret_cast<AppContext*>(handle);
    if (!ctx) return;

    ctx->running = false;
    if (ctx->thread.joinable()) ctx->thread.join();

    if (ctx->window) {
        ANativeWindow_release(ctx->window);
        ctx->window = nullptr;
    }

    delete ctx;
}