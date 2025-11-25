#pragma once

class ISceneUpdate {
public:
    virtual ~ISceneUpdate() = default;
    virtual void onUpdate() = 0;
};