/*!
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file evens_queue.hpp
 * */

#pragma once
#include "component_notification.hpp"
#include "generic/threadsafe_queue.hpp"
#include "agent-framework/generic/singleton.hpp"

namespace agent_framework {
namespace eventing {

class EventsQueue : public ::generic::ThreadSafeQueue<ComponentNotification>,
        public agent_framework::generic::Singleton<EventsQueue> {
public:
    virtual ~EventsQueue();

    void push_back(ComponentNotification notification) {
        ::generic::ThreadSafeQueue<ComponentNotification>::push_back(std::move(notification));
    }

    void push_back(ComponentNotification&& notification) {
        ::generic::ThreadSafeQueue<ComponentNotification>::push_back(std::move(notification));
    }

    void push_back(EventDataVec events) {
        ComponentNotification notification{};
        notification.set_notifications(std::move(events));
        ::generic::ThreadSafeQueue<ComponentNotification>::push_back(std::move(notification));
    }

    void push_back(EventData event) {
        ComponentNotification notification{};
        notification.set_notifications({std::move(event)});
        ::generic::ThreadSafeQueue<ComponentNotification>::push_back(std::move(notification));
    }


private:
    friend class agent_framework::generic::Singleton<EventsQueue>;
    EventsQueue() {}
};

}
}

