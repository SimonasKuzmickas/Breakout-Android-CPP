#pragma once

#include "../scene/IScene.h"
#include "GameScene.h"
#include "../ui/UILayoutHandler.h"
#include "../ui/UILayoutMenu.h"
#include "../ui/UIAutoPlayer.h"
#include "SceneManager.h"
#include "SceneId.h"

class MenuScene : public IScene {
public:
    explicit MenuScene(AppContext* context) {
        appContext = context;
    }

    void onStart() override {
        addComponent(std::make_shared<UIAutoPlayer>());
        addComponent(std::make_shared<LevelSystem>());
        addComponent(std::make_shared<PowerUpSystem>());
        addComponent(std::make_shared<BallSystem>());
        addComponent(std::make_shared<Paddle>());
        addComponent(std::make_shared<LaserShooter>());
        addComponent(std::make_shared<SoundsManager>(appContext));
        addComponent(std::make_shared<PlayerState>());

        auto layout = std::make_shared<UILayoutMenu>(appContext);
        addComponent(layout);

        if (layout) {
            layout->onStartPressed.subscribe([this]() {
                sceneManager->requestChange(SceneId::Game);
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