#pragma once

#include "scene/IScene.h"
#include "GameScene.h"
#include "ui/UIInputHandler.h"
#include "ui/UIRenderer.h"
#include "ui/UIMenuLayout.h"

class MenuScene : public IScene {
public:
    explicit MenuScene(AppContext* context) {
        appContext = context;
    }

    void onStart() override {
        addComponent(std::make_shared<UIInputHandler>(appContext));
        addComponent(std::make_shared<UIRenderer>(appContext));
        addComponent(std::make_shared<UIMenuLayout>());
    }

    void onUpdate() override {
       sceneManager->requestChange(std::make_unique<GameScene>(appContext));
    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};