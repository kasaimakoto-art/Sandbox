/**
 * @file event_bus.hpp
 * @brief シンプルなイベントバスの宣言。
 */
#ifndef __EVENTBUS_H__
#define __EVENTBUS_H__

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <vector>

#include "../../../domain/common/include/event_sink.hpp"

namespace kaf::infra::application{
    /**
     * @class EventBus
     * @brief 型をキーにした購読と配信を提供します。
     */
    class EventBus final : public kaf::domain::common::IEventSink {
        /** @brief 型ごとのサブスクリプション保持。 */
        std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> subscriptions_;
        /**
         * @brief IEventSink の型消去 publish 実装。
         * @param typeInfo イベントの型情報
         * @param pointer  イベント実体へのポインタ
         */
        void publish_erased(const std::type_info& typeInfo,const void* pointer) noexcept override;
    public:
        /**
         * @brief 指定型のイベント購読を登録します。
         * @tparam E イベント型
         * @param fn コールバック（const E&）
         */
        template<class E>
        void subscribe(std::function<void(const E&)> fn);
    };
}

#endif
