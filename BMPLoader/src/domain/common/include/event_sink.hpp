#ifndef __EVENTSINK_H__
#define __EVENTSINK_H__

#include <typeinfo>

namespace kaf::domain::common{
    struct IEventSink {
        virtual ~IEventSink() = default;
    private:
        virtual void publish_erased(const std::type_info&, const void*) noexcept = 0;
    public:
        template<class E>
        void publish(const E& event)noexcept;
    };
}


#endif