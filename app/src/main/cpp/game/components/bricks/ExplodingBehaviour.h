#pragma once

#include "../brick/IBrick.h"
#include "../brick/IBrickBehaviour.h"

namespace Breakout {

class ExplodingBehaviour : public IBrickBehavior {
public:
    void hit(IBrick& brick) override {
        if (!brick.getIsDamaged()) {
            brick.damage();
            return;
        }
    }

    virtual void update(IBrick& brick) {
        if (brick.getIsDamaged()) {
            explosionTimer -= GameTime::deltaTime();
            if(explosionTimer <= 0) {
                brick.explode();
                brick.destroy();
            }
        }
    };
private:
    float explosionTimer = 0.2f;
};

}