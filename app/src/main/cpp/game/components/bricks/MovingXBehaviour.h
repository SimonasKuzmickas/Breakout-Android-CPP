
#include "../brick/IBrick.h"

namespace Breakout {

class MovingXBehaviour : public IBrick {
public:
    MovingXBehaviour(int gx, int gy)
            : IBrick(gx, gy, IBrick::BrickType::DynamicBlue) {
        isDynamic = true;
    }

    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    virtual void update() {
        float amplitude = 80.0f;
        float speed = 2.0f;

        float t = GameTime::realtimeSinceStartup();
        float offsetX = amplitude * std::sin(speed * t);

        bounds.x = gridX * BRICK_WIDTH + offsetX;
        bounds.y = gridY * BRICK_HEIGHT;
        bounds.w = BRICK_WIDTH;
        bounds.h = BRICK_HEIGHT;
    }

    void hit() {
        isDestroyed = true;
    }
};

}