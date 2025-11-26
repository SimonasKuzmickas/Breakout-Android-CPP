#pragma once
#include "../GraphicsManager.h"

namespace Breakout {

class UILayoutGameOver : public UILayoutHandler {
public:
    UILayoutGameOver(AppContext *context) : UILayoutHandler(context) {}

    Event<> onRetryPressed;
    Event<> onMenuPressed;

    void onAwake() override {
        UILayoutHandler::onAwake();

        resourceBackground = loadImage("background.png");
        resourceStartButton = loadImage("startbutton.png");
        resourceBlock = loadImage("block.png");
        resourcesTitle = loadImage("title.png");

        inputHandler = getComponent<UILayoutHandler>();
    }

    void onUpdate() override {
        drawImage(resourceBackground, 0, 0, 1920, 1080);
        drawImage(resourceBlock, 580, 0, 680, 1080);
        drawImage(resourcesTitle, 600, 540, 650, 660);

        if (drawButton(resourceStartButton, 700, 440, 440, 200)) {
            onMenuPressed.invoke();
        }

        if (drawButton(resourceStartButton, 700, 220, 440, 200)) {
            onRetryPressed.invoke();
        }

        graphicsAPI.flip();
    }

private:
    GLuint resourceBackground;
    GLuint resourceStartButton;
    GLuint resourceBlock;
    GLuint resourcesTitle;

    std::shared_ptr<UILayoutHandler> inputHandler;
};

}