#pragma once

#include "../helpers/Math.h"

namespace Breakout {

// Global pointer to the active PlayerController instance
// Used by callbacks to call touch events into C++
class PlayerController;
static PlayerController* g_playerController = nullptr;

class PlayerController : public ISceneComponent {
public:
    enum class TouchAction {
        Down = 0,
        Up = 1,
        Move = 2,
        Cancel = 3,
        PointerDown = 5,
        PointerUp = 6
    };

    explicit PlayerController(AppContext* context)
    {
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);

        isMoving = false;
        screenBounds = Rect(0, 0, (float)width, (float)height);
    }

    void onAwake() override {
        g_playerController = this;

        paddle = getComponent<Paddle>();
        position = Vector2(0, 0);
        isMoving = false;
    }

    void onDestroy() override {
        g_playerController = nullptr;
    }

    void onUpdate() override {
        if (!isMoving) {
            paddle->move(Paddle::Direction::None);
            return;
        }

        float center = screenBounds.w * 0.5f;
        float offset = position.x - center;

        if (offset < 0.0f) {
            paddle->move(Paddle::Direction::Left);
        } else if (offset > 0.0f) {
            paddle->move(Paddle::Direction::Right);
        } else {
            paddle->move(Paddle::Direction::None);
        };
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
    Rect screenBounds;
    Vector2 position;
    bool isMoving;
    int activePointer = -1;
    std::shared_ptr<Paddle> paddle;
};

}