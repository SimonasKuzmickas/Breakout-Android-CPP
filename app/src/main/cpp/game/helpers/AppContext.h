#pragma once
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <thread>

struct AppContext {
    ANativeWindow* window = nullptr;
    std::thread thread;
    std::atomic<bool> running{false};
    AAssetManager* assetManager = nullptr;
};
