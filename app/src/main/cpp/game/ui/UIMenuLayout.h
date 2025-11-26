#pragma once

class UIMenuLayout : public GraphicsManager {
public:
    UIMenuLayout(AppContext* context) : GraphicsManager(context) {}

    Event<> onStartPressed;

    void onAwake() override {
        GraphicsManager::onAwake();
        inputHandler = getComponent<UIInputHandler>();

        resourceBackground = loadImage("background.png");
        resourceBlock = loadImage("block.png");
        resourceStartButton = loadImage("startbutton.png");
        resourcesTitle = loadImage("title.png");
    }

    void onUpdate () override {
        drawLevel();
        drawPaddle();
        drawPowerUps();
        drawBalls();

        drawImage(resourceBlock, 580, 0, 680, 1080);
        drawImage(resourcesTitle, 600, 540, 650, 660);

        if(drawButton(resourceStartButton, 700, 440, 440, 200))
        {
            onStartPressed.invoke();
        }

        graphicsAPI.flip();
    }

    bool drawButton(GLuint textureId, float x, float y, float w, float h) {s
        drawImage(textureId, x, y, w, h);

        if(inputHandler->isPressed())
        {
            auto touchPosition = inputHandler->getPosition();
            auto imageBounds = Rect(x, y, w, h);
            auto inputBounds = Rect(touchPosition.x, touchPosition.y, 1, 1);

            if(imageBounds.overlaps(inputBounds))
            {
                return true;
            }
        }

        return false;
    }

private:
    GLuint resourceBackground;
    GLuint resourceBlock;
    GLuint resourceStartButton;
    GLuint resourcesTitle;
    std::shared_ptr<UIInputHandler> inputHandler;
};