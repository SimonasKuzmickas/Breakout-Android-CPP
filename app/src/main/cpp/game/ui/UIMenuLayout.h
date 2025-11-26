#pragma once

#include <GLES2/gl2.h>
#include "../scene/ISceneComponent.h"
#include "../scene/ISceneComponentRender.h"
#include "UIRenderer.h"

class UIMenuLayout : public ISceneComponent, public ISceneComponentRender {
public:
    void onAwake() override {
        renderer = getComponent<UIRenderer>();
        resourceBackground = renderer->loadImage("background.png");
    }

    void onDestroy() override { }

    void onRender() override {
        renderer->drawImage(resourceBackground, 0, 0, 1920, 1080);
    }

private:
    GLuint resourceBackground{};
    std::shared_ptr<UIRenderer> renderer;
};