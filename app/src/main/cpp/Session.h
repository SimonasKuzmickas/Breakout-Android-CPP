// Session.h
#pragma once
#include <vector>
#include <memory>
#include "IModule.h"

class Session {
public:
    void addModule(std::shared_ptr<IModule> module) {
        modules.push_back(module);
    }

    void start() {
        for (auto& m : modules) m->onStart();
    }

    void update() {
        for (auto& m : modules) {
            if (auto* updatable = dynamic_cast<IModuleUpdate*>(m.get())) {
                updatable->onUpdate();
            }
        }
    }

    void render() {
        for (auto& m : modules) {
            if (auto* renderable = dynamic_cast<IModuleRender*>(m.get())) {
                renderable->onRender();
            }
        }
    }

    void shutdown() {
        for (auto& m : modules) m->onShutdown();
    }

private:
    std::vector<std::shared_ptr<IModule>> modules;
};