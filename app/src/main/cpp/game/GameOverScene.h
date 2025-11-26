#pragma once

#include "scene/IScene.h"
#include "GraphicsManager.h"
#include "ui/UILayoutHandler.h"
#include "ui/UIMenuLayout.h"
#include "ui/UIGameOverLayout.h"
//#include "MenuScene.h"

class GameOverScene : public IScene {
public:
    Event<> onPressedMenu;
    Event<> onPressedRetry;

    explicit GameOverScene(AppContext* context)
    {
        appContext = context;
    }

    void onStart() override {
        auto layout = std::make_shared<UIGameOverLayout>(appContext);
        addComponent(layout);

        layout->onMenuPressed.subscribe([this]() {
           // sceneManager->requestChange(std::make_unique<MenuScene>(appContext));
        });
    }

    void onUpdate() override {

    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};