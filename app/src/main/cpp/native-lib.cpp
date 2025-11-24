#include <jni.h>
#include <thread>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "game/scene/Scene.h"
#include "game/Graphics.h"
#include "game/LevelManager.h"
#include "game/Paddle.h"
#include "game/PlayerInput.h"
#include "game/BallSystem.h"

#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

void gameLoop(AppContext* appContext) {
    const double deltaTime = 1.0 / 60.0;
    auto lastTick = std::chrono::steady_clock::now();

    Scene scene;
    scene.addComponent(std::make_shared<Graphics>(appContext));
    scene.addComponent(std::make_shared<LevelManager>());
    scene.addComponent(std::make_shared<Paddle>());
    scene.addComponent(std::make_shared<BallSystem>());
    scene.addComponent(std::make_shared<PlayerInput>(appContext));
    scene.start();

    while (appContext->running) {
        auto now = std::chrono::steady_clock::now();
        auto next = lastTick + std::chrono::duration<double>(deltaTime);

        if (now >= next) {
            lastTick = now;
            scene.update();
            scene.render();
        }

        std::this_thread::sleep_until(next);
    }

    scene.destroy();
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

extern "C"
JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeOnTouch(JNIEnv* env, jobject thiz,
                                                     jint pointerId, jfloat x, jfloat y, jint action) {
    if (g_playerInput) {
        g_playerInput->onTouch(pointerId, x, y, action);
    }
}