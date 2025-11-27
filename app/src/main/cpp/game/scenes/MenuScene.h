#pragma once

#include "../scene/IScene.h"
#include "GameScene.h"
#include "../ui/UILayoutHandler.h"
#include "../ui/UILayoutMenu.h"
#include "../ui/UIAutoPlayer.h"
#include "SceneManager.h"
#include "SceneId.h"

namespace Breakout::Scenes {

// MenuScene: Main menu scene that initializes UI
// transitions to GameScene when Start is pressed
class MenuScene : public IScene {
public:
    explicit MenuScene(AppContext *context)
            : appContext(context) {}

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

        layout->onStartPressed.addListener([this]() {
            sceneManager->requestChange(SceneId::Game);
        });
    }

private:
    AppContext *appContext;
};

}