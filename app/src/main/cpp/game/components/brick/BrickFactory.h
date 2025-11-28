#pragma once

#include "../../helpers/Event.h"
#include "Brick.h"
#include "ArmorBrick.h"
#include "StandardBrick.h"
#include "ExplodingBrick.h"
#include "MovingBrickX.h"
#include "MovingArmorBrickY.h"
#include "StaticBrick.h"

namespace Breakout {

struct BrickFactory {

    static std::shared_ptr<Brick> createBrick(Brick::BrickType type, int gridX, int gridY) {
            switch (type) {
                case Brick::BrickType::NormalOrange:
                    return std::make_shared<StandardBrick>(gridX, gridY, type);
                case Brick::BrickType::ArmorPurple:
                case Brick::BrickType::ArmorBrown:
                    return std::make_shared<ArmorBrick>(gridX, gridY, type);
                case Brick::BrickType::ExplodingYellow:
                    return std::make_shared<ExplodingBrick>(gridX, gridY, type);
                case Brick::BrickType::DynamicBlue:
                    return std::make_shared<MovingBrickX>(gridX, gridY, type);
                case Brick::BrickType::DynamicGreen:
                    return std::make_shared<MovingArmorBrickY>(gridX, gridY, type);
                case Brick::BrickType::StaticGray:
                    return std::make_shared<StaticBrick>(gridX, gridY, type);
            }

        return std::make_shared<StandardBrick>(gridX, gridY, type);
    }
};

}
