#ifndef __EVENTBUS_H__
#define __EVENTBUS_H__

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <vector>

#include "../../../domain/common/include/event_sink.hpp"

namespace kaf::infra::application{
    class EventBus final : public kaf::domain::common::IEventSink {
        std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> subscriptions_;
        void publish_erased(const std::type_info& typeInfo,const void* pointer) noexcept override;
    public:
        template<class E>
        void subscribe(std::function<void(const E&)> fn);
    };
}

#endif