#pragma once

#include "IScene.h"

namespace Breakout {

class ISceneManager {
public:
    virtual std::unique_ptr<IScene> createScene(int id) = 0;

    void requestChange(int sceneId) {
        pendingScene = sceneId;
    }

    void update() {
        if (currentScene) {
            currentScene->update();
        }
        if (pendingScene > -1) {
            changeState(createScene(pendingScene));
            pendingScene = -1;
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
    int pendingScene = -1;
};

}