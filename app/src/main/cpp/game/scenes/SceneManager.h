#pragma once

#include "../helpers/Math.h"
#include "../scene/ISceneManager.h"
#include "MenuScene.h"
#include "SceneId.h"

class SceneManager : public ISceneManager {
public:
    explicit SceneManager(AppContext* context)
    {
        appContext = context;
    }

    std::unique_ptr<IScene> createScene(int id) override {
        switch (id) {
            case SceneId::Menu:
                return std::make_unique<MenuScene>(appContext);

            case SceneId::Game:
                return std::make_unique<GameScene>(appContext);

            case SceneId::GameOver:
                return std::make_unique<GameOverScene>(appContext);

            default:
                return nullptr; // or throw std::invalid_argument
        }
    }

private:
    AppContext* appContext;

};