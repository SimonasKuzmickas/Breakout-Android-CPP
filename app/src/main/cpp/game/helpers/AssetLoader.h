#pragma once
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

namespace Breakout {

class AssetLoader {
public:
    static std::string load(AAssetManager *mgr, const char *filename) {
        AAsset *asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
        if (!asset) return {};

        size_t size = AAsset_getLength(asset);
        std::string buffer(size, '\0');
        AAsset_read(asset, buffer.data(), size);
        AAsset_close(asset);
        return buffer;
    }
};

}