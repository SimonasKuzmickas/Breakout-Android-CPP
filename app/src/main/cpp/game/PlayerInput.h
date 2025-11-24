#pragma once
#include "scene/ISceneComponent.h"
#include "Math.h"

enum class TouchAction {
    Down = 0,
    Up = 1,
    Move = 2,
    Cancel = 3,
    PointerDown = 5,
    PointerUp = 6
};

// Global pointer to the active PlayerInput instance
// Used by callbacks to call touch events into C++
class PlayerInput;
static PlayerInput* g_playerInput = nullptr;

class PlayerInput : public ISceneComponent, public ISceneUpdate {
public:

    void onAwake() override {
        g_playerInput = this;

        auto gameWorld = blackboard->getComponent<GameWorld>();
        paddle = blackboard->getComponent<Paddle>();

        worldBounds = gameWorld->getWorldBounds();
        position = Vector2(0, 0);
        isMoving = false;
    }

    void onDestroy() override {
        g_playerInput = nullptr;
    }

    void onUpdate() override {
        LOGI("playerInput: isMoving = %s", isMoving ? "true" : "false");
        if(isMoving)
        {
            if(position.x > worldBounds.w / 2)
            {
                paddle->move(1);
            } else {
                paddle->move(-1);
            }
        }
    }

    void onTouch(int pointerId, float x, float y, int action) {
        auto act = static_cast<TouchAction>(action);

        switch (act) {
            case TouchAction::Down:
            case TouchAction::PointerDown:
                activePointer = pointerId;
                position = Vector2(x, y);
                isMoving = true;
                break;

            case TouchAction::Move:
                if (pointerId == activePointer) {
                    position = Vector2(x, y);
                }
                break;

            case TouchAction::Up:
            case TouchAction::PointerUp:
            case TouchAction::Cancel:
                if (pointerId == activePointer) {
                    activePointer = -1;
                    isMoving = false;
                }
                break;

            default:
                break;
        }
    }

private:
    Rect worldBounds;
    Vector2 position;
    bool isMoving;
    int activePointer = -1;
    std::shared_ptr<Paddle> paddle;
};
