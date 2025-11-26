#pragma once

#include <vector>
#include <memory>
#include "ISceneComponent.h"
#include "ISceneUpdate.h"
#include "ISceneRender.h"

class Scene {
public:
    Scene()
            : blackboard(components)
    {}

    virtual ~Scene() = default;
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onDestroy() = 0;

    void addComponent(std::shared_ptr<ISceneComponent> component) {
        components.push_back(component);
        component->setBlackboard(&blackboard);
    }

    void start() {
        onStart();

        for (auto &m: components)
        {
            m->onAwake();
        }
    }

    void update() {
        onUpdate();

        for (auto &m: components) {
            if (auto *updatable = dynamic_cast<ISceneUpdate *>(m.get())) {
                updatable->onUpdate();
            }
        }
    }

    void render() {
        for (auto &m: components) {
            if (auto *renderable = dynamic_cast<ISceneRender *>(m.get())) {
                renderable->onRender();
            }
        }
    }

    void destroy() {
        onDestroy();

        for (auto &m: components) {
            m->onDestroy();
        }
    }

private:
    std::vector<std::shared_ptr<ISceneComponent>> components;
    Blackboard blackboard;
};