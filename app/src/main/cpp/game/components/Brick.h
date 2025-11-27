#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
#include "../helpers/AssetLoader.h"
#include <list>

using json = nlohmann::json;

namespace Breakout {

class LevelSystem;

struct Brick {
public:
    enum class BrickType {
        NormalOrange, // 0
        HardPurple, // 1
        HardBrown, // 2
        DynamicBlue, // 3
        DynamicGreen, // 4
        ExplodingYellow, // 5
        StaticGray // 6
    };

    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    Event<> onDamaged;
    Event<Brick*> onExplode;

    Brick(int gridX, int gridY, BrickType type)
            : bounds{gridX * BRICK_WIDTH, gridY * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT},
              type(type),
              gridX(gridX),
              gridY(gridY) {}

    const Rect &getBounds() const { return bounds; }
    BrickType getType() const { return type; }
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }
    bool getIsDestroyed() const { return isDestroyed; }
    bool getIsDamaged() const { return isDamaged; }
    bool getIsDynamic() const { return type == BrickType::DynamicBlue || type == BrickType::DynamicGreen; }
    bool getIsDestructible() const{ return type != BrickType::StaticGray; }

    void update() {
        switch (type) {
            case BrickType::DynamicBlue: {
                float amplitude = 80.0f;
                float speed = 2.0f;

                float t = GameTime::realtimeSinceStartup();
                float offsetX = amplitude * std::sin(speed * t);

                bounds.x = gridX * BRICK_WIDTH + offsetX;
                bounds.y = gridY * BRICK_HEIGHT;
                break;
            }

            case BrickType::DynamicGreen: {
                float amplitude = 40.0f;
                float speed = 1.5f;

                float t = GameTime::realtimeSinceStartup();
                float offsetY = amplitude * std::sin(speed * t);

                bounds.x = gridX * BRICK_WIDTH;
                bounds.y = gridY * BRICK_HEIGHT + offsetY;
                break;
            }

            case BrickType::ExplodingYellow:
                if(isDamaged) {
                    explosionTimer += GameTime::deltaTime();
                    if(explosionTimer >= 0.2f) {
                        onExplode.invoke(this);
                        isDestroyed = true;
                    }
                }
                break;
        }
    }

    void hit() {
        switch (type) {
            case BrickType::HardPurple:
            case BrickType::HardBrown:
                if (!isDamaged) {
                    isDamaged = true;
                    onDamaged.invoke();
                    return; // first hit only damages
                }
                // second hit destroys
                isDestroyed = true;
                return;

            case BrickType::ExplodingYellow:
                if (!isDamaged) {
                    isDamaged = true;
                    onDamaged.invoke();
                    return; // explosion handled in update()
                }
                return;

            case BrickType::StaticGray:
                return; // indestructible
        }

        isDestroyed = true;
    }

private:
    Rect bounds;
    int gridX = 0;
    int gridY = 0;
    BrickType type;
    bool isDestroyed = false;
    bool isDamaged = false;
    float explosionTimer = 0.0f;
};

}