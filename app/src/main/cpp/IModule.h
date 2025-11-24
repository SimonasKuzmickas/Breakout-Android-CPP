// IModule.h
#pragma once

class IModule {
public:
    virtual ~IModule() = default;

    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    virtual void onShutdown() = 0;
};