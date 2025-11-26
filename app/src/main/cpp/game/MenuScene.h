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
 
        auto menuLayout = std::make_shared<UIMenuLayout>();
        addComponent(menuLayout);

        if (menuLayout) {
            menuLayout->onStartPressed.subscribe([this]() {
                sceneManager->requestChange(std::make_unique<GameScene>(appContext));
            });
        }
    }

    void onUpdate() override {

    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};