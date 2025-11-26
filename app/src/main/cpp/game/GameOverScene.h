#pragma once

#include "scene/IScene.h"
#include "GraphicsManager.h"
#include "ui/UIInputHandler.h"
#include "ui/UIMenuLayout.h"
#include "ui/UIGameOverLayout.h"

class GameOverScene : public IScene {
public:
    Event<> onPressedMenu;
    Event<> onPressedRetry;

    explicit GameOverScene(AppContext* context)
    {
        appContext = context;
    }

    void onStart() override {
        addComponent(std::make_shared<UIInputHandler>(appContext));

        auto layout = std::make_shared<UIGameOverLayout>(appContext);
        addComponent(layout);
    }

    void onUpdate() override {

    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};