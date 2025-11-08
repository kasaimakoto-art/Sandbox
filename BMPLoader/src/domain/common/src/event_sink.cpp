/**
 * @file event_sink.cpp
 * @brief IEventSink::publish テンプレートの実装。
 */
#include "../include/event_sink.hpp"

namespace kaf::domain::common{
    template<class E>
    void IEventSink::publish(const E& event)noexcept { publish_erased(typeid(E), &event); }
}
