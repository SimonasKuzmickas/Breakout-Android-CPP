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
        resourcesRetryButton = loadImage("retry.png");
        resourcesMenuButton = loadImage("menu.png");
        resourceBlock = loadImage("block.png");
        resourcesGameOver = loadImage("gameover.png");

        inputHandler = getComponent<UILayoutHandler>();
    }

    void onUpdate() override {
        drawImage(resourceBackground, 0, 0, 1920, 1080);
        drawImage(resourceBlock, 580, 0, 680, 1080);
        drawImage(resourcesGameOver, 580, 650, 650, 360);

        if (drawButton(resourcesMenuButton, 700, 440, 440, 160)) {
            onMenuPressed.invoke();
        }

        if (drawButton(resourcesRetryButton, 700, 220, 440, 160)) {
            onRetryPressed.invoke();
        }

        graphicsAPI.flip();
    }

private:
    GLuint resourceBackground;
    GLuint resourcesRetryButton;
    GLuint resourcesMenuButton;
    GLuint resourceBlock;
    GLuint resourcesGameOver;

    std::shared_ptr<UILayoutHandler> inputHandler;
};

}