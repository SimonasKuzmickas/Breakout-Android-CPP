#pragma once

#include "../../helpers/Event.h"
#include "IBrickBehaviour.h"
#include "IBrick.h"
#include "ArmorBehavior.h"
#include "NormalBehavior.h"
#include "ExplodingBehaviour.h"
#include "MovingXBehaviour.h"
#include "MovingYArmorBehaviour.h"
#include "StaticBehaviour.h"

namespace Breakout {

struct Brick : IBrick {

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
        behavior = createBehavior(type);

        behavior->start(*this);
    }

    void update() override { behavior->update(*this); }
    void hit() override { behavior->hit(*this); }
    void destroy() override { isDestroyed = true; }
    void damage() override {
        isDamaged = true;
        onDamage.invoke();
    }
    void deflect() override {
        onDeflect.invoke();
    }
    void setDestructible(bool state) {
        isDestructible = state;
    }

    void setBounds(Rect rect) override {bounds = rect; }

    void explode() override {
        onExplode.invoke(this);
    }

    int getGridX() const override { return gridX; }
    int getGridY() const override { return gridY; }
    bool getIsDamaged() override { return isDamaged; }

    const Rect &getBounds() const { return bounds; }
    bool getIsDestroyed() const { return isDestroyed; }
    bool getIsDynamic() const { return isDynamic; }
    bool getIsDestructible() const { return isDestructible; }

    BrickType getType() const { return type; }

    Event<> onDamage;
    Event<Brick *> onExplode;
    Event<> onDeflect;

private:
    std::unique_ptr<IBrickBehavior> behavior;

    static std::unique_ptr<IBrickBehavior> createBehavior(BrickType type) {
        switch (type) {
            case BrickType::NormalOrange:
                return std::make_unique<NormalBehavior>();

            case BrickType::ArmorPurple:
            case BrickType::ArmorBrown:
                return std::make_unique<ArmorBehavior>();

            case BrickType::ExplodingYellow:
                return std::make_unique<ExplodingBehaviour>();

            case BrickType::DynamicBlue:
                return std::make_unique<MovingXBehaviour>();

            case BrickType::DynamicGreen:
                return std::make_unique<MovingYArmorBehaviour>();

            case BrickType::StaticGray:
                return std::make_unique<StaticBehaviour>();
        }

        return nullptr;
    }

    BrickType type;
    Rect bounds;
    bool isDestroyed = false;
    bool isDestructible = true;
    bool isDamaged = false;
    bool isDynamic = false;
    int gridX = 0;
    int gridY = 0;
};

};

