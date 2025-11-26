#pragma once

class ISceneComponentUpdate {
public:
    virtual ~ISceneComponentUpdate() = default;
    virtual void onUpdate() = 0;
};