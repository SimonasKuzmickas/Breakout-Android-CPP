#pragma once

#include <vector>
#include <memory>
#include "ISceneComponent.h"

namespace Breakout {

class ISceneManager;

class IScene {
protected:
    virtual void onStart() = 0;

    virtual void onUpdate() = 0;

    virtual void onDestroy() = 0;

public:
    virtual ~IScene() = default;

    void addComponent(std::shared_ptr<ISceneComponent> component) {
        components.push_back(component);
        component->setComponents(&components);
    }

    void start() {
        onStart();

        for (auto &c: components) {
            c->onAwake();
        }
    }

    void update() {
        onUpdate();

        for (auto &c: components) {
            c->onUpdate();
        }
    }

    void destroy() {
        onDestroy();

        for (auto &c: components) {
            c->onDestroy();
        }
    }

    void setManager(ISceneManager *manager) { sceneManager = manager; }

private:
    std::vector<std::shared_ptr<ISceneComponent>> components;

protected:
    ISceneManager *sceneManager = nullptr;
};

}