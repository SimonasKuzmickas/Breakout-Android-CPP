#pragma once

#include <vector>
#include <memory>
#include "ISceneComponent.h"
#include "ISceneComponentUpdate.h"
#include "ISceneComponentRender.h"

class SceneManager;

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

        for (auto &c: components)
        {
            c->onAwake();
        }
    }

    void update() {
        onUpdate();

        for (auto &c: components) {
            if (auto *updatable = dynamic_cast<ISceneComponentUpdate *>(c.get())) {
                updatable->onUpdate();
            }
        }
    }

    void render() {
        for (auto &c: components) {
            if (auto *renderable = dynamic_cast<ISceneComponentRender *>(c.get())) {
                renderable->onRender();
            }
        }
    }

    void destroy() {
        onDestroy();

        for (auto &c: components) {
            c->onDestroy();
        }
    }

    void setManager(SceneManager* manager) { sceneManager = manager; }

private:
    std::vector<std::shared_ptr<ISceneComponent>> components;
    Blackboard blackboard;

protected:
    SceneManager* sceneManager = nullptr;
};