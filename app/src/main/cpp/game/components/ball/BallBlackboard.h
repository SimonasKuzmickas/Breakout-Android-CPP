#pragma once

namespace Breakout {

struct BallBlackboard {
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<Paddle> paddle;

    BallBlackboard(std::shared_ptr<LevelSystem> lvl,
                   std::shared_ptr<Paddle> pad)
            : levelSystem(std::move(lvl)),
              paddle(std::move(pad))
    {}
};

}