#include <jni.h>
#include <thread>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "game/helpers/GameTime.h"
#include "game/scenes/SceneManager.h"
#include "game/scenes/MenuScene.h"
#include "game/scenes/SceneId.h"

namespace Breakout {
    void gameLoop(AppContext* appContext) {
        int targetFrameRate = 60;
        const double deltaTimeTarget = 1.0 / targetFrameRate;
        auto lastTick = std::chrono::steady_clock::now();

        auto sceneManager = Scenes::SceneManager(appContext);
        sceneManager.requestChange((int)Scenes::SceneId::Menu);

        while (appContext->running) {


            auto now = std::chrono::steady_clock::now();
            auto next = lastTick + std::chrono::duration<double>(deltaTimeTarget);

            if (now >= next) {
                lastTick = now;

                GameTime::update();
                sceneManager.update();
            }

            std::this_thread::sleep_until(next);
        }

       sceneManager.destroy();
    }
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStart(JNIEnv* env, jobject, jobject surface, jobject assetManager) {
    auto* context = new Breakout::AppContext();
    context->window = ANativeWindow_fromSurface(env, surface);
    context->running = true;
    context->thread = std::thread(Breakout::gameLoop, context);
    context->assetManager = AAssetManager_fromJava(env, assetManager);
    return reinterpret_cast<jlong>(context);
}

extern "C" JNIEXPORT void JNICALL
Java_com_nordcurrent_breakout_GameView_nativeStop(JNIEnv*, jobject, jlong handle) {
    auto* context = reinterpret_cast<Breakout::AppContext*>(handle);
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
    if (Breakout::g_playerController) {
        Breakout::g_playerController->onTouch(pointerId, x, y, action);
    }

    if (Breakout::g_layoutHandler) {
        Breakout::g_layoutHandler->onTouch(x, y, action);
    }
}