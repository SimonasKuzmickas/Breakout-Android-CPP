#pragma once

namespace Breakout {

// Global pointer to the active PlayerController instance
// Used by callbacks to call touch events into C++
class PlayerController;
static PlayerController* g_playerController = nullptr;

class PlayerController : public ISceneComponent {
public:
    // --- Touch input actions
    enum class TouchAction {
        Down = 0,
        Up = 1,
        Move = 2,
        Cancel = 3,
        PointerDown = 5,
        PointerUp = 6
    };

    PlayerController(AppContext* context,
                     std::shared_ptr<Paddle> paddle)
            : paddle(std::move(paddle)){
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);
        screenBounds = Rect(0, 0, (float)width, (float)height);
    }

    void onAwake() override {
        g_playerController = this;
        position = Vector2(0, 0);
        isMoving = false;
        activePointer = -1;
    }

    void onDestroy() override {
        g_playerController = nullptr;
    }

    // Update paddle movement based on touch input
    void onUpdate() override {
        if (!isMoving) {
            paddle->move(Paddle::Direction::None);
            return;
        }

        auto paddleBounds = paddle->getBounds();
        float ratio = screenBounds.w / VIRTUAL_WIDTH;
        float center = paddleBounds.center().x * ratio;
        float offset = position.x - center;

        if (std::abs(offset) > paddleBounds.w * MOVE_THRESHOLD) {
            paddle->move(offset < 0.0f ? Paddle::Direction::Left : Paddle::Direction::Right);
        } else {
            paddle->move(Paddle::Direction::None);
        }
    }

    // --- Handle touch input
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
    static constexpr float VIRTUAL_WIDTH = 1920.0f;
    static constexpr float MOVE_THRESHOLD  = 0.25f;

    Rect screenBounds;
    Vector2 position;
    bool isMoving = false;
    int activePointer = -1;
    std::shared_ptr<Paddle> paddle;
};

}