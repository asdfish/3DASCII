#pragma once

#include <functional>
#include <vector>

class EventBus
{
    public:
        template<typename EventType>
        void Subscribe(std::function<void(const EventType&)> listener);

        template<typename EventType>
        void Emit(const EventType& event);
    private:
        std::unordered_map<const char*, std::vector<std::function<void(const void*)>>> listeners;
};