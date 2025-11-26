#pragma once

#include <vector>
#include <memory>
#include "ISceneComponent.h"

namespace Breakout {

class ISceneComponent;

class Blackboard {
public:
    explicit Blackboard(const std::vector<std::shared_ptr<ISceneComponent>> &components)
            : componentsRef(components) {}

    template<typename T>
    std::shared_ptr<T> getComponent() const {
        for (auto &m: componentsRef) {
            if (auto casted = std::dynamic_pointer_cast<T>(m)) {
                return casted;
            }
        }
        return nullptr;
    }

private:
    const std::vector<std::shared_ptr<ISceneComponent>> &componentsRef;

};

}