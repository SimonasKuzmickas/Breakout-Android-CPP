#pragma once

#include "../GraphicsManager.h"
#include "../ui/UILayoutHandler.h"

namespace Breakout {

class UILayoutGameOver : public UILayoutHandler {
public:
    UILayoutGameOver(AppContext *context) : UILayoutHandler(context) {}

    Event<> onRetryPressed;
    Event<> onMenuPressed;

    void onAwake() override {
        UILayoutHandler::onAwake();

        resourceBackground = loadImage("bg_space.png");
        resourcesRetryButton = loadImage("ui_retry.png");
        resourcesMenuButton = loadImage("ui_menu.png");
        resourceBlock = loadImage("ui_block.png");
        resourcesGameOver = loadImage("ui_gameover.png");

        inputHandler = getComponent<UILayoutHandler>();
    }

    void onUpdate() override {
        drawImage(resourceBackground, 0, 0, 1920, 1080);
        drawImage(resourceBlock, 580, 0, 680, 1080);
        drawImage(resourcesGameOver, 580, 650, 650, 360);


        if (drawButton(resourcesRetryButton, 700, 440, 440, 160)) {
            onRetryPressed.invoke();
        }

        if (drawButton(resourcesMenuButton, 700, 220, 440, 160)) {
            onMenuPressed.invoke();
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