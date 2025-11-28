#pragma once
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

#define DR_WAV_IMPLEMENTATION
#include "../thirdparty/dr_wav.h"

namespace Breakout {

struct ImageData {
    int width{};
    int height{};
    int channels{};
    std::vector<unsigned char> pixels;
};

class AssetLoader {
public:
    static std::vector<uint8_t> loadBuffer(AAssetManager* mgr, const char* filename) {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
        if (!asset) {
            return {}; // return empty vector if asset not found
        }

        const size_t length = AAsset_getLength(asset);
        std::vector<uint8_t> buffer(length);
        AAsset_read(asset, buffer.data(), length);

        AAsset_close(asset);

        return buffer;
    }

    static ImageData loadImage(AAssetManager* mgr, const char* filename) {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
        if (!asset) return {};

        size_t length = AAsset_getLength(asset);
        const void* buffer = AAsset_getBuffer(asset);

        int width, height, channels;
        unsigned char* rawPixels = stbi_load_from_memory(
                reinterpret_cast<const unsigned char*>(buffer),
                static_cast<int>(length),
                &width, &height, &channels,
                STBI_rgb_alpha);

        AAsset_close(asset);

        ImageData img;
        img.width    = width;
        img.height   = height;
        img.channels = STBI_rgb_alpha;
        img.pixels.assign(rawPixels, rawPixels + (width * height * 4));

        stbi_image_free(rawPixels);
        return img;
    }

    static std::vector<float> loadSound(AAssetManager *mgr, const char *filename, uint32_t &sampleRate, uint16_t &channels) {
        std::vector<uint8_t> buffer = loadBuffer(mgr, filename);
        // Load raw asset bytes
        if (buffer.empty()) {
            return {};
        }

        // Initialize dr_wav from memory
        drwav wav;
        if (!drwav_init_memory(&wav, buffer.data(), buffer.size(), nullptr)) {
            return {};
        }

        // Capture format info
        sampleRate = wav.sampleRate;
        channels = wav.channels;

        // Read all samples as float32
        size_t totalSamples = wav.totalPCMFrameCount * wav.channels;
        std::vector<float> samples(totalSamples);

        drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, samples.data());

        // Clean up
        drwav_uninit(&wav);

        return samples;
    }

    static std::string loadText(AAssetManager *mgr, const char *filename) {
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