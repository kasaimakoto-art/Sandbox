/**
 * @file event_bus.cpp
 * @brief EventBus の実装。
 */
#include "../include/event_bus.hpp"

namespace kaf::infra::application{
    void EventBus::publish_erased(const std::type_info& typeInfo,const void* pointer) noexcept {
        auto it = subscriptions_.find(std::type_index{typeInfo});
        if(it == subscriptions_.end()){
            return;
        }
        for(auto&  func: it->second){
            func(pointer);
        }
    }
    template<class E>
    void EventBus::subscribe(std::function<void(const E&)> func) {
        subscriptions_[std::type_index{typeid(E)}].push_back(
            [func=std::move(func)](const void* pointer){ func(*static_cast<const E*>(pointer)); }
        );
    }
}
