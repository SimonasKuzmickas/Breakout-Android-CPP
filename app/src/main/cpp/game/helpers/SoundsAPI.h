#pragma once

#define DR_WAV_IMPLEMENTATION
#include "../thirdparty/dr_wav.h"
#include <oboe/Oboe.h>
#include "AppContext.h"

namespace Breakout {

struct Sound {
    std::vector<float> samples;
    size_t readIndex = 0;
    bool playing = false;
    float gain = 1.0f;

    void play(float g = 1.0f) {
        readIndex = 0;
        playing = true;
        gain = g;
    }
};

class SoundsAPI : public oboe::AudioStreamCallback {
public:
    SoundsAPI(AppContext *context) {
        appContext = context;
    }

    virtual oboe::DataCallbackResult onAudioReady(oboe::AudioStream *stream,
                                                  void *audioData,
                                                  int32_t numFrames) override {
        float *out = static_cast<float *>(audioData);
        int channels = stream->getChannelCount();
        int samplesNeeded = numFrames * channels;

        for (int i = 0; i < samplesNeeded; i++) out[i] = 0.0f;

        for (Sound *s: sounds) {
            if (!s || !s->playing) continue;

            for (int i = 0; i < samplesNeeded; i++) {
                if (s->readIndex < s->samples.size()) {
                    out[i] += s->samples[s->readIndex++] * s->gain;
                } else {
                    s->playing = false; // finished
                    break;
                }
            }
        }

        return oboe::DataCallbackResult::Continue;
    }

    Sound *loadSound(const std::string &name) {
        uint32_t sr = 0;
        uint16_t ch = 0;

        std::string fullName = name + ".wav";
        auto data = loadWavFromAssets(appContext->assetManager, fullName.c_str(), sr, ch);

        Sound *s = new Sound{std::move(data)};
        sounds.emplace_back(s);
        return sounds.back();
    }

    void initialize() {
        uint32_t sr;
        uint16_t ch;

        loadWavFromAssets(appContext->assetManager, "sfx_sample.wav", sr, ch);

        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Output)
                ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                ->setSharingMode(oboe::SharingMode::Exclusive)
                ->setFormat(oboe::AudioFormat::Float)
                ->setChannelCount(ch)
                ->setSampleRate(sr)
                ->setCallback(this);

        builder.openStream(mStream);
        mStream->requestStart();
    }

    void destroy() {
        if (mStream) {
            mStream->stop();
            mStream->close();
            mStream.reset();
        }

        sounds.clear();
    }

private:
    std::vector<Sound *> sounds;
    std::shared_ptr<oboe::AudioStream> mStream;
    AppContext *appContext;

    std::vector<float> loadWavFromAssets(AAssetManager *mgr, const char *filename,
                                         uint32_t &sampleRate,
                                         uint16_t &channels) {
        AAsset *asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
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
};

}