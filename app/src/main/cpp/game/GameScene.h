#pragma once

#include "scene/IScene.h"
#include "GraphicsManager.h"
#include "SoundsManager.h"
#include "PlayerInput.h"
#include "PlayerState.h"
#include "LevelSystem.h"
#include "PowerUpSystem.h"
#include "BallSystem.h"
#include "Paddle.h"
#include "LaserShooter.h"
#include "GameOverScene.h"

class GameScene : public IScene {
public:
    explicit GameScene(AppContext* context)
    {
        appContext = context;
    }

    void onStart() override {
        auto playerState = std::make_shared<PlayerState>();
        playerState->onDeath.subscribe([this]() {
            sceneManager->requestChange(std::make_unique<GameOverScene>(appContext));
        });

        addComponent(std::make_shared<GraphicsManager>(appContext));
        addComponent(std::make_shared<SoundsManager>(appContext));
        addComponent(std::make_shared<PlayerInput>(appContext));
        addComponent(std::make_shared<LevelSystem>());
        addComponent(std::make_shared<PowerUpSystem>());
        addComponent(std::make_shared<BallSystem>());
        addComponent(std::make_shared<Paddle>());
        addComponent(std::make_shared<LaserShooter>());
        addComponent(playerState);
    }

    void onUpdate() override {

    }

    void onDestroy() override {

    }

private:
    AppContext* appContext;
};