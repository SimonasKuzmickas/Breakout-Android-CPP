#pragma once

namespace Breakout {

class UILayoutMenu : public UILayoutHandler {
public:
    UILayoutMenu(AppContext *context) : UILayoutHandler(context) {}

    Event<> onStartPressed;

    void onAwake() override {
        UILayoutHandler::onAwake();

        resourceBackground = loadImage("background.png");
        resourceBlock = loadImage("block.png");
        resourceStartButton = loadImage("startbutton.png");
        resourcesTitle = loadImage("title.png");
    }

    void onUpdate() override {
        drawLevel();
        drawPaddle();
        drawPowerUps();
        drawBalls();

        drawImage(resourceBlock, 580, 0, 680, 1080);
        drawImage(resourcesTitle, 600, 480, 650, 660);

        if (drawButton(resourceStartButton, 700, 300, 440, 160)) {
            onStartPressed.invoke();
        }

        graphicsAPI.flip();
    }

private:
    GLuint resourceBackground;
    GLuint resourceBlock;
    GLuint resourceStartButton;
    GLuint resourcesTitle;
};

}