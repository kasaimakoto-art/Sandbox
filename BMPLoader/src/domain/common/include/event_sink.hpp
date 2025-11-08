/**
 * @file event_sink.hpp
 * @brief イベント送信（Publish）インターフェースの宣言。
 */
#ifndef __EVENTSINK_H__
#define __EVENTSINK_H__

#include <typeinfo>

namespace kaf::domain::common{
    /**
     * @interface IEventSink
     * @brief 型消去された publish 実装に委譲する、テンプレート publish を提供します。
     */
    struct IEventSink {
        virtual ~IEventSink() = default;
    private:
        /**
         * @brief 型消去されたイベントを配信します（実装側で実体化）。
         * @param typeInfo 実際の型情報
         * @param pointer  イベント実体へのポインタ
         */
        virtual void publish_erased(const std::type_info&, const void*) noexcept = 0;
    public:
        /**
         * @brief 型安全にイベントを配信します。
         * @tparam E イベント型
         * @param event 送信するイベント
         */
        template<class E>
        void publish(const E& event)noexcept;
    };
}


#endif
