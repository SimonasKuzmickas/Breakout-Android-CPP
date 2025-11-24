// Session.h
#pragma once
#include <vector>
#include <memory>
#include "IModule.h"

class IModule;

class Blackboard {
public:
    explicit Blackboard(const std::vector<std::shared_ptr<IModule>>& modules)
            : modulesRef(modules) {}

    template <typename T>
    std::shared_ptr<T> getModule() const {
        for (auto& m : modulesRef) {
            if (auto casted = std::dynamic_pointer_cast<T>(m)) {
                return casted;
            }
        }
        return nullptr;
    }

private:
    const std::vector<std::shared_ptr<IModule>>& modulesRef; // non-owning reference

};