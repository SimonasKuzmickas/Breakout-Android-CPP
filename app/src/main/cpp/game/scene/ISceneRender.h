#pragma once

class ISceneRender {
public:
    virtual ~ISceneRender() = default;
    virtual void onRender() = 0;
};