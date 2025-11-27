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
        // Core game systems dependencies
        auto paddle       = std::make_shared<Paddle>();
        auto levelSystem  = std::make_shared<LevelSystem>(appContext);
        auto ballSystem   = std::make_shared<BallSystem>(paddle, levelSystem);
        auto playerState  = std::make_shared<PlayerState>(levelSystem, ballSystem);
        auto laserShooter = std::make_shared<LaserShooter>(paddle, levelSystem, ballSystem);
        auto powerUpSystem= std::make_shared<PowerUpSystem>(levelSystem, paddle, laserShooter, ballSystem, playerState);

        addComponent(paddle);
        addComponent(levelSystem);
        addComponent(ballSystem);
        addComponent(playerState);
        addComponent(laserShooter);
        addComponent(powerUpSystem);

        // UI and audio systems
        addComponent(std::make_shared<UIAutoPlayer>());
        addComponent(std::make_shared<SoundsManager>(appContext));

        // Menu layout
        auto layout = std::make_shared<UILayoutMenu>(appContext);
        addComponent(layout);

        // Scene transition
        layout->onStartPressed.addListener([this]() {
            sceneManager->requestChange(SceneId::Game);
        });
    }

private:
    AppContext *appContext;
};

}