// IModule.h
#pragma once

class IModule {
public:
    virtual ~IModule() = default;

    virtual void onStart() = 0;
    virtual void onShutdown() = 0;
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