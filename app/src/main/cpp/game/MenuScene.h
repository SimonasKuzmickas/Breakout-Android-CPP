#pragma once

#include "scene/IScene.h"
#include "GameScene.h"
#include "ui/UIInputHandler.h"
#include "ui/UIMenuLayout.h"
#include "ui/UIAutoPlayer.h"

class MenuScene : public IScene {
public:
    explicit MenuScene(AppContext* context) {
        appContext = context;
    }

    void onStart() override {
        addComponent(std::make_shared<UIInputHandler>(appContext));

        addComponent(std::make_shared<UIAutoPlayer>());
        addComponent(std::make_shared<LevelSystem>());
        addComponent(std::make_shared<PowerUpSystem>());
        addComponent(std::make_shared<BallSystem>());
        addComponent(std::make_shared<Paddle>());
        addComponent(std::make_shared<LaserShooter>());

        auto menuLayout = std::make_shared<UIMenuLayout>(appContext);
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