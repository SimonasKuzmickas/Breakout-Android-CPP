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
        for (auto& m : modules) m->onUpdate();
    }

    void render() {
        for (auto& m : modules) m->onRender();
    }

    void shutdown() {
        for (auto& m : modules) m->onShutdown();
    }

private:
    std::vector<std::shared_ptr<IModule>> modules;
};