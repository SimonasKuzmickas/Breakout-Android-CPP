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

class ISceneUpdate {
public:
    virtual ~ISceneUpdate() = default;
    virtual void onUpdate() = 0;
};

class ISceneRender {
public:
    virtual ~ISceneRender() = default;
    virtual void onRender() = 0;
};