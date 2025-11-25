#pragma once
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <functional>
#include <vector>
#include <thread>

struct AppContext {
    ANativeWindow* window = nullptr;
    std::thread thread;
    std::atomic<bool> running{false};
    AAssetManager* assetManager = nullptr;
};
