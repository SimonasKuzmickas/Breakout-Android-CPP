#pragma once

#define DR_WAV_IMPLEMENTATION
#include <android/log.h>
#include <oboe/Oboe.h>

#include "Brick.h"
#include "Math.h"
#include "dr_wav.h"
#include "scene/ISceneComponent.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class SoundsManager : public ISceneComponent, public oboe::AudioStreamCallback {
public:
    SoundsManager(AppContext* context) { appContext = context; }

    void onAwake() override {
        // Preload WAV
        uint32_t sampleRate;
        uint16_t channels;
        samples = loadWavFromAssets(appContext->assetManager, "paddlehit.wav",
                                    sampleRate, channels);

        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Output)
                ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                ->setFormat(oboe::AudioFormat::Float)
                ->setChannelCount(channels)
                ->setSampleRate(sampleRate)
                ->setCallback(this);

        oboe::Result result = builder.openStream(mStream);
        //  builder.setSampleRate(mStream->getSampleRate());
        if (result == oboe::Result::OK) {
            mStream->requestStart();
            LOGI("Audio stream started");
        } else {
            LOGE("Failed to open Oboe stream: %s", oboe::convertToText(result));
        }
        play();
    }

    void onDestroy() override {
        if (mStream) {
            mStream->stop();
            mStream->close();
            mStream.reset();
            LOGI("Audio stream stopped");
        }
    }

    // Trigger playback once
    void play() {
        readIndex = 0;
        playing = true;
    }

    std::vector<float> loadWavFromAssets(AAssetManager* mgr, const char* filename,
                                         uint32_t& sampleRate,
                                         uint16_t& channels) {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
        if (!asset) return {};

        size_t length = AAsset_getLength(asset);
        std::vector<uint8_t> buffer(length);
        AAsset_read(asset, buffer.data(), length);
        AAsset_close(asset);

        drwav wav;
        if (!drwav_init_memory(&wav, buffer.data(), buffer.size(), nullptr)) {
            return {};
        }

        sampleRate = wav.sampleRate;
        channels = wav.channels;

        size_t totalSamples = wav.totalPCMFrameCount * wav.channels;
        std::vector<float> samples(totalSamples);
        drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, samples.data());
        drwav_uninit(&wav);
        return samples;
    }

    // Oboe callback: fill audio buffer
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream* oboeStream,
                                          void* audioData,
                                          int32_t numFrames) override {
        float* out = static_cast<float*>(audioData);
        int32_t channels = oboeStream->getChannelCount();
        int32_t samplesNeeded = numFrames * channels;

        for (int i = 0; i < samplesNeeded; i++) {
            if (playing && readIndex < samples.size()) {
                out[i] = samples[readIndex++];
            } else {
                out[i] = 0.0f;  // silence
            }
        }

        // Stop playing once buffer ends
        if (readIndex >= samples.size()) {
            playing = false;
        }

        return oboe::DataCallbackResult::Continue;
    }

private:
    std::shared_ptr<oboe::AudioStream> mStream;
    AppContext* appContext;

    std::vector<float> samples;
    size_t readIndex = 0;
    bool playing = false;
};