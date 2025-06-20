#include "managers/EventBus.hpp"


template<typename EventType>
void EventBus::Subscribe(std::function<void(const EventType&)> listener)
{
    listeners[typeid(EventType).name()].push_back(
        [listener](const void* event){
            listener(*static_cast<const EventType*>(event));
        }
    );
}

template<typename EventType>
void EventBus::Emit(const EventType& event)
{
    const char* typeName = typeid(EventType).name();
    for (auto& func : listeners[typeName])
    {
        func(&event);
    }
}
