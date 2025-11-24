#include <jni.h>
#include <thread>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "Session.h"
#include "GraphicsModule.h"
#include "GameWorld.h"

void gameLoop(AppContext* appContext) {
    const double deltaTime = 1.0 / 60.0;
    auto lastTick = std::chrono::steady_clock::now();

    Session session;
    session.addModule(std::make_shared<GraphicsModule>(appContext));
    session.addModule(std::make_shared<GameWorld>(appContext));

    session.start();

    while (appContext->running) {
        auto now = std::chrono::steady_clock::now();
        auto next = lastTick + std::chrono::duration<double>(deltaTime);

        if (now >= next) {
            lastTick = now;
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
    auto* context = new AppContext();
    context->window = ANativeWindow_fromSurface(env, surface);
    context->running = true;
    context->thread = std::thread(gameLoop, context);
    return reinterpret_cast<jlong>(context);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStop(JNIEnv*, jobject, jlong handle) {
    auto* context = reinterpret_cast<AppContext*>(handle);
    if (!context) return;

    context->running = false;
    if (context->thread.joinable()) context->thread.join();

    if (context->window) {
        ANativeWindow_release(context->window);
        context->window = nullptr;
    }

    delete context;
}