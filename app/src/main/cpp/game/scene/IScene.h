#pragma once

#include <vector>
#include <memory>
#include "ISceneComponent.h"

namespace Breakout {

class ISceneManager;

class IScene {
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

protected:
    // TODO: perhaps I could use empty implementations to reduce some code
    virtual void onStart() { };
    virtual void onUpdate() { };
    virtual void onDestroy() { };

    ISceneManager *sceneManager = nullptr;

private:
    std::vector<std::shared_ptr<ISceneComponent>> components;
};

}