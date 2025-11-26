#pragma once

class ISceneComponentRender {
public:
    virtual ~ISceneComponentRender() = default;
    virtual void onRender() = 0;
};