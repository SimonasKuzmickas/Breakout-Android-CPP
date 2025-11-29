#pragma once

#include "../../helpers/Event.h"

namespace Breakout {

class Brick {

public:
    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    Brick(int gx, int gy, int look)
            : bounds{gx * BRICK_WIDTH, gy * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT},
              gridX(gx), gridY(gy), look(look) {
    }

    virtual void update() {}
    virtual void hit() { isDestroyed = true; }

    void damage() {
        isDamaged = true;
        onDamage.invoke();
    }

    // Grid position getters
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }

    // State getters
    int getLook() const { return look; }
    bool getIsDamaged() { return isDamaged; }
    const Rect &getBounds() const { return bounds; }
    bool getIsDestroyed() const { return isDestroyed; }
    bool getIsDynamic() const { return isDynamic; }
    bool getIsDestructible() const { return isDestructible; }

    // Events
    Event<> onDamage;
    Event<Brick *> onExplode;
    Event<> onDeflect;

private:
    int look;

protected:
    Rect bounds;

    bool isDestroyed = false;
    bool isDestructible = true;
    bool isDamaged = false;
    bool isDynamic = false;

    int gridX = 0;
    int gridY = 0;
};

}