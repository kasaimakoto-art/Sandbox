#include "../include/event_sink.hpp"

namespace kaf::domain::common{
    template<class E>
    void IEventSink::publish(const E& event)noexcept { publish_erased(typeid(E), &event); }
}