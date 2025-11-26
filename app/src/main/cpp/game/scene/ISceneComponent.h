#pragma once

#include "IScene.h"

namespace Breakout {

class ISceneComponent {
public:
    virtual ~ISceneComponent() = default;
    virtual void onAwake() = 0;
    virtual void onDestroy() = 0;
    virtual void onUpdate() = 0;

    void setComponents(std::vector<std::shared_ptr<ISceneComponent>>* comp) {
        componentsRef = comp;
    }

    template<typename T>
    std::shared_ptr<T> getComponent() const {
        if (!componentsRef) {
            return nullptr;
        }
        for (auto& m : *componentsRef) {
            if (auto casted = std::dynamic_pointer_cast<T>(m)) {
                return casted;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::shared_ptr<ISceneComponent>>* componentsRef = nullptr;
};

}