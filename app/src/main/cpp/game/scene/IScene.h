#pragma once

#include <vector>
#include <memory>
#include "ISceneComponent.h"


class ISceneManager;

class IScene {
protected:
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onDestroy() = 0;

public:
    virtual ~IScene() = default;
    IScene()
            : blackboard(components)
    {}

    void addComponent(std::shared_ptr<ISceneComponent> component) {
        components.push_back(component);
        component->setBlackboard(&blackboard);
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

    void setManager(ISceneManager* manager) { sceneManager = manager; }

private:
    std::vector<std::shared_ptr<ISceneComponent>> components;
    Blackboard blackboard;

protected:
    ISceneManager* sceneManager = nullptr;
};