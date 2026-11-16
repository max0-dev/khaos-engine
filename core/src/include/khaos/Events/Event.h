#pragma once

#include <functional>
#include <type_traits>
namespace KhaosCore {
    class Event {
      public:
        bool isHandled = false;
        virtual ~Event() = default;
    };

    class EventDispatcher {
      private:
        Event& mEvent;

      public:
        EventDispatcher(Event& event) : mEvent(event) {}
        template <typename T>
        void Dispatch(std::function<bool(T& derivedEvent)> callback)
            requires(std::is_base_of_v<Event, T>)
        {
            T* castedEvent = dynamic_cast<T*>(&mEvent);
            if (castedEvent != nullptr && !mEvent.isHandled) {
                mEvent.isHandled = callback(*castedEvent);
            }
        }
    };
} // namespace KhaosCore
