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
        auto playerState = std::make_shared<PlayerState>();

        playerState->onDeath.addListener([this]() {
            sceneManager->requestChange(SceneId::GameOver);
        });

        addComponent(std::make_shared<GraphicsManager>(appContext));
        addComponent(std::make_shared<SoundsManager>(appContext));
        addComponent(std::make_shared<PlayerController>(appContext));
        addComponent(std::make_shared<LevelSystem>(appContext));
        addComponent(std::make_shared<PowerUpSystem>());
        addComponent(std::make_shared<BallSystem>());
        addComponent(std::make_shared<Paddle>());
        addComponent(std::make_shared<LaserShooter>());
        addComponent(playerState);
    }

private:
    AppContext *appContext;
};

}