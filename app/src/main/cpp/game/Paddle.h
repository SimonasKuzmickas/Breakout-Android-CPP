#pragma once
#include "IModule.h"
#include "Math.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class Paddle : public IModule {
public:
    explicit Paddle()
    {
        

    }

    void onStart() override   {

    }

    void onShutdown() override {

    }

private:
    Rect worldBounds;
};