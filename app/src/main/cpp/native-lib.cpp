#include <jni.h>
#include <thread>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "game/scene/SceneManager.h"
#include "game/MenuScene.h"

void gameLoop(AppContext* appContext) {
    const double deltaTime = 1.0 / 60.0;
    auto lastTick = std::chrono::steady_clock::now();

    auto sceneManager = SceneManager();
    sceneManager.requestChange(std::make_unique<MenuScene>(appContext));

    while (appContext->running) {
        auto now = std::chrono::steady_clock::now();
        auto next = lastTick + std::chrono::duration<double>(deltaTime);

        if (now >= next) {
            lastTick = now;
            sceneManager.update();
        }

        std::this_thread::sleep_until(next);
    }

    sceneManager.destroy();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStart(JNIEnv* env, jobject, jobject surface, jobject assetManager) {
    auto* context = new AppContext();
    context->window = ANativeWindow_fromSurface(env, surface);
    context->running = true;
    context->thread = std::thread(gameLoop, context);
    context->assetManager = AAssetManager_fromJava(env, assetManager);
    return reinterpret_cast<jlong>(context);
}

extern "C" JNIEXPORT void JNICALL
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

extern "C" JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeOnTouch(JNIEnv* env, jobject thiz,
                                                     jint pointerId, jfloat x, jfloat y, jint action) {
    if (g_playerInput) {
        g_playerInput->onTouch(pointerId, x, y, action);
    }

    if (g_inputHandler) {
        g_inputHandler->onTouch(x, y, action);
    }
}