// IModule.h
#pragma once
#include "Blackboard.h"

class IModule {
public:
    virtual ~IModule() = default;

    virtual void onStart() = 0;

    virtual void onShutdown() = 0;

    void setBlackboard(Blackboard* board) {
         blackboard = board;
    }

protected:
    Blackboard *blackboard = nullptr;
};

class IModuleUpdate {
public:
    virtual ~IModuleUpdate() = default;
    virtual void onUpdate() = 0;
};

class IModuleRender {
public:
    virtual ~IModuleRender() = default;
    virtual void onRender() = 0;
};