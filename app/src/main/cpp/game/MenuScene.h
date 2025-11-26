#pragma once

#include "scene/IScene.h"
#include "GameScene.h"

class MenuScene : public IScene {
public:
    explicit MenuScene(AppContext* context)
    {
        appContext = context;
    }

    void onStart() override {

    }

    void onUpdate() override {
        sceneManager->requestChange(std::make_unique<GameScene>(appContext));
    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};