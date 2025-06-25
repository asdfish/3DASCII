#pragma once

#include <set>
#include "entity.hpp"
#include <memory>

class System
{
    public:
        std::set<Entity> entities;
};