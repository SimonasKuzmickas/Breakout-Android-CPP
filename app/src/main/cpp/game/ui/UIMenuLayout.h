#pragma once

#include <GLES2/gl2.h>
#include "../scene/ISceneComponent.h"
#include "../scene/ISceneComponentRender.h"
#include "../helpers/Event.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class UIMenuLayout : public ISceneComponent, public ISceneComponentRender {
public:
    Event<> onStartPressed;

    void onAwake() override {
        renderer = getComponent<UIRenderer>();
        resourceBackground = renderer->loadImage("background.png");
        resourceBlock = renderer->loadImage("block.png");
        resourceStartButton = renderer->loadImage("startbutton.png");
        resourcesTitle = renderer->loadImage("title.png");
    }

    void onDestroy() override { }

    void onRender() override {
        renderer->drawImage(resourceBlock, 580, 0, 680, 1080);
        renderer->drawImage(resourcesTitle, 600, 540, 650, 660);

        if(renderer->drawButton(resourceStartButton, 700, 440, 440, 200))
        {
            onStartPressed.invoke();
        }
    }

private:
    GLuint resourceBackground;
    GLuint resourceBlock;
    GLuint resourceStartButton;
    GLuint resourcesTitle;
    std::shared_ptr<UIRenderer> renderer;
};