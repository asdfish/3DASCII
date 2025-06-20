#pragma once

#include <set>
#include "entity.hpp"

class System
{
    public:
        std::set<Entity> entities;
};