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
    enum BrickType {
        Standard,
        Armor,
        Armor2,
        MovingX,
        MovingY,
        Exploding,
        Static
    };
    static std::shared_ptr<Brick> createBrick(int look, BrickType brickType, int gridX, int gridY) {
            switch (brickType) {
                case BrickType::Standard:       return std::make_shared<StandardBrick>(gridX, gridY, look);
                case BrickType::Armor:
                case BrickType::Armor2:         return std::make_shared<ArmorBrick>(gridX, gridY, look);
                case BrickType::MovingX:        return std::make_shared<MovingBrickX>(gridX, gridY, look);
                case BrickType::MovingY:        return std::make_shared<MovingArmorBrickY>(gridX, gridY, look);
                case BrickType::Exploding:      return std::make_shared<ExplodingBrick>(gridX, gridY, look);
                case BrickType::Static:         return std::make_shared<StaticBrick>(gridX, gridY, look);
            }

        return std::make_shared<StandardBrick>(gridX, gridY, look);
    }
};

}
