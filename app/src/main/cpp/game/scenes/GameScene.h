#pragma once

#include "../scene/IScene.h"
#include "../GraphicsManager.h"
#include "../SoundsManager.h"
#include "../components/PlayerController.h"
#include "../components/PlayerState.h"
#include "../components/LevelSystem.h"
#include "../components/PowerUpSystem.h"
#include "../components/BallSystem.h"
#include "../components/Paddle.h"
#include "../components/LaserShooter.h"
#include "GameOverScene.h"
#include "SceneId.h"

namespace Breakout::Scenes {

// GameScene: Core gameplay scene
// transitions to GameOverScene when the player dies
class GameScene : public IScene {
public:
    explicit GameScene(AppContext *context)
            : appContext(context) {}

    void onStart() override {
        // Core game systems dependencies
        auto levelSystem   = std::make_shared<LevelSystem>(appContext);
        auto paddle        = std::make_shared<Paddle>();
        auto playerController = std::make_shared<PlayerController>(appContext, paddle);
        auto ballSystem    = std::make_shared<BallSystem>(paddle, levelSystem);
        auto playerState   = std::make_shared<PlayerState>(levelSystem, ballSystem);
        auto laserShooter  = std::make_shared<LaserShooter>(paddle, levelSystem, ballSystem);
        auto powerUpSystem = std::make_shared<PowerUpSystem>(levelSystem, paddle, laserShooter, ballSystem, playerState);

        addComponent(playerController);
        addComponent(levelSystem);
        addComponent(paddle);
        addComponent(ballSystem);
        addComponent(playerState);
        addComponent(laserShooter);
        addComponent(powerUpSystem);


        // fx and audio systems
        addComponent(std::make_shared<SoundsManager>(appContext));
        addComponent(std::make_shared<GraphicsManager>(appContext));

        // Scene transition
        playerState->onDeath.addListener([this]() {
            sceneManager->requestChange(SceneId::GameOver);
        });
    }

private:
    AppContext *appContext;
};

}