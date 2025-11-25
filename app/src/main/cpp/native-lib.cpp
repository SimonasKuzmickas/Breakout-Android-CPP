#include <jni.h>
#include <thread>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "game/scene/Scene.h"
#include "game/GraphicsManager.h"
#include "game/SoundsManager.h"
#include "game/PlayerInput.h"
#include "game/LevelSystem.h"
#include "game/PowerUpSystem.h"
#include "game/BallSystem.h"
#include "game/Paddle.h"

#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

void gameLoop(AppContext* appContext) {
    const double deltaTime = 1.0 / 60.0;
    auto lastTick = std::chrono::steady_clock::now();

    Scene scene;
    scene.addComponent(std::make_shared<GraphicsManager>(appContext));
    scene.addComponent(std::make_shared<SoundsManager>(appContext));
    scene.addComponent(std::make_shared<PlayerInput>(appContext));
    scene.addComponent(std::make_shared<LevelSystem>());
    scene.addComponent(std::make_shared<PowerUpSystem>());
    scene.addComponent(std::make_shared<BallSystem>());
    scene.addComponent(std::make_shared<Paddle>());

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
}