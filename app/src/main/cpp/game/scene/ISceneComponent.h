#pragma once

#include "Blackboard.h"

class ISceneComponent {
public:
    virtual ~ISceneComponent() = default;
    virtual void onAwake() = 0;
    virtual void onDestroy() = 0;
    virtual void onUpdate() = 0;

    void setBlackboard(Blackboard* board) {
         blackboard = board;
    }

    template<typename T>
    std::shared_ptr<T> getComponent() const {
        if (!blackboard) {
            return nullptr;
        }
        return blackboard->getComponent<T>();
    }

private:
    Blackboard *blackboard = nullptr;
};
