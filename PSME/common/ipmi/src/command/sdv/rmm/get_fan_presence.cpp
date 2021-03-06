/*!
 * @brief GetFanPresence IPMI command request and response implementation.
 *
 * @header{License}
 * @copyright Copyright (c) 2017 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @header{Filesystem}
 * @file sdv/rmm/get_fan_presence.hpp
 */

#include "ipmi/command/sdv/rmm/get_fan_presence.hpp"
#include "ipmi/command/sdv/enums.hpp"



using namespace ipmi;
using namespace ipmi::command::sdv;


request::GetFanPresence::GetFanPresence() : GetPresence(sdv::Cmd::GET_FAN_PRESENCE) {}


request::GetFanPresence::~GetFanPresence() {}


response::GetFanPresence::GetFanPresence() : GetPresence(sdv::Cmd::GET_FAN_PRESENCE) {}


response::GetFanPresence::~GetFanPresence() {}


bool response::GetFanPresence::is_present(std::uint8_t fan_index) const {
    return (fan_index >= get_slot_count()) ? false : (m_presence_mask & (1 << fan_index));
}


std::uint8_t response::GetFanPresence::get_slot_count() const {
    return SLOT_COUNT;
}
