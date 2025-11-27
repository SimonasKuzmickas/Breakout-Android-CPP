#pragma once

#include "../../helpers/Event.h"

namespace Breakout {

class IBrick {
public:
    virtual ~IBrick() = default;
    virtual void update() = 0;
    virtual void hit() = 0;
    virtual bool getIsDamaged() = 0;
    virtual void destroy() = 0;
    virtual void damage() = 0;
    virtual void deflect() = 0;
    virtual void explode() = 0;
    virtual int getGridX() const = 0;
    virtual int getGridY() const = 0;
    virtual void setBounds(Rect) = 0;
    virtual void setDestructible(bool state) = 0;
};

}