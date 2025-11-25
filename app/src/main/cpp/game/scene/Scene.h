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

    void addComponent(std::shared_ptr<ISceneComponent> component) {
        components.push_back(component);
        component->setBlackboard(&blackboard);
    }

    void start() {
        for (auto &m: components)
        {
            m->onAwake();
        }
    }

    void update() {
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
        for (auto &m: components) {
            m->onDestroy();
        }
    }

private:
    std::vector<std::shared_ptr<ISceneComponent>> components;
    Blackboard blackboard;
};