#pragma once

#include "../../helpers/Event.h"

namespace Breakout {

class Brick {

public:
    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    enum class BrickType {
        NormalOrange,
        ArmorPurple,
        ArmorBrown,
        DynamicBlue,
        DynamicGreen,
        ExplodingYellow,
        StaticGray
    };

    Brick(int gx, int gy, BrickType type)
            : bounds{gx * BRICK_WIDTH, gy * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT},
              gridX(gx), gridY(gy), type(type) {
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
    BrickType getType() const { return type; }
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
    BrickType type;

protected:
    Rect bounds;

    // Brick state flags
    bool isDestroyed = false;
    bool isDestructible = true;
    bool isDamaged = false;
    bool isDynamic = false;

    int gridX = 0;
    int gridY = 0;
};

}