#pragma once
#include "IScene.h"

class SceneManager {
public:
    void requestChange(std::unique_ptr<IScene> newScene) {
        pendingScene = std::move(newScene);
    }

    void update() {
        if (currentScene) {
            currentScene->update();
            currentScene->render();
        }
        if (pendingScene) {
            changeState(std::move(pendingScene));
        }
    }

    void destroy() {
        if (currentScene) {
            currentScene->destroy();
            currentScene = nullptr;
        }
    }

private:
    void changeState(std::unique_ptr<IScene> newScene) {
        if (currentScene) currentScene->destroy();
        currentScene = std::move(newScene);
        currentScene->setManager(this);
        currentScene->start();
    }

    std::unique_ptr<IScene> currentScene;
    std::unique_ptr<IScene> pendingScene;
};