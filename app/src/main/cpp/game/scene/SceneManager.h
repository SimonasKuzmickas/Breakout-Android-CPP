#pragma once
#include "IScene.h"

class SceneManager {
public:
    void changeState(std::unique_ptr<IScene> newScene) {
        if (currentScene) {
            currentScene->destroy();
        }
        currentScene = std::move(newScene);
        currentScene->setManager(this);
        currentScene->start();
    }

    void update() {
        if (currentScene) {
            currentScene->update();
            currentScene->render();
        }
    }

    void destroy() {
        if (currentScene) {
            currentScene->destroy();
            currentScene = nullptr;
        }
    }

private:
    std::unique_ptr<IScene> currentScene;
};