// IModule.h
#pragma once
#include "Blackboard.h"

class ISceneComponent {
public:
    virtual ~ISceneComponent() = default;

    virtual void onAwake() = 0;

    virtual void onDestroy() = 0;

    void setBlackboard(Blackboard* board) {
         blackboard = board;
    }

protected:
    Blackboard *blackboard = nullptr;
};
