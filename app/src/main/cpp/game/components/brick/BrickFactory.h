#pragma once

#include "../../helpers/Event.h"
#include "IBrick.h"
#include "../bricks/ArmorBehavior.h"
#include "../bricks/NormalBehavior.h"
#include "../bricks/ExplodingBehaviour.h"
#include "../bricks/MovingXBehaviour.h"
#include "../bricks/MovingYArmorBehaviour.h"
#include "../bricks/StaticBehaviour.h"

namespace Breakout {

struct BrickFactory {

    static std::shared_ptr<IBrick> createBrick(IBrick::BrickType type, int gridX, int gridY) {
            switch (type) {
                case IBrick::BrickType::NormalOrange:
                    return std::make_shared<NormalBehavior>(gridX, gridY);
                case IBrick::BrickType::ArmorPurple:
                case IBrick::BrickType::ArmorBrown:
                    return std::make_shared<ArmorBehavior>(gridX, gridY);
                case IBrick::BrickType::ExplodingYellow:
                    return std::make_shared<ExplodingBehaviour>(gridX, gridY);
                case IBrick::BrickType::DynamicBlue:
                    return std::make_shared<MovingXBehaviour>(gridX, gridY);
                case IBrick::BrickType::DynamicGreen:
                    return std::make_shared<MovingYArmorBehaviour>(gridX, gridY);
                case IBrick::BrickType::StaticGray:
                    return std::make_shared<StaticBehaviour>(gridX, gridY);
            }

        return std::make_shared<NormalBehavior>(gridX, gridY);
    }
};

}
