#pragma once

#include "../scene/IScene.h"
#include "../ui/UILayoutHandler.h"
#include "../ui/UILayoutGameOver.h"
#include "SceneId.h"

class GameOverScene : public IScene {
public:
    Event<> onPressedMenu;
    Event<> onPressedRetry;

    explicit GameOverScene(AppContext* context) {
        appContext = context;
    }

    void onStart() override {
        auto layout = std::make_shared<UILayoutGameOver>(appContext);
        addComponent(layout);

        layout->onMenuPressed.subscribe([this]() {
            sceneManager->requestChange((int)SceneId::Menu);
        });

        layout->onRetryPressed.subscribe([this]() {
            sceneManager->requestChange((int)SceneId::GameOver);
        });
    }

    void onUpdate() override {

    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};