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
 * @file responses/storage/add_iscsi_target.cpp
 *
 * @brief Storage AddIscsiTarget response implementation
 * */

#include "agent-framework/module/responses/storage/add_iscsi_target.hpp"
#include "agent-framework/module/constants/storage.hpp"
#include "agent-framework/module/constants/common.hpp"
#include "json-wrapper/json-wrapper.hpp"

using namespace agent_framework::model::responses;
using namespace agent_framework::model::literals;

AddIscsiTarget::AddIscsiTarget(const std::string& target,
                               Oem oem) :
    m_oem(oem), m_target(target) {}

json::Json AddIscsiTarget::to_json() const {
    json::Json value;
    value[IscsiTarget::TARGET] = m_target;
    value[literals::Oem::OEM] = m_oem.to_json();
    return value;
}

AddIscsiTarget AddIscsiTarget::from_json(const json::Json& json) {
    AddIscsiTarget request{
        json[IscsiTarget::TARGET],
        agent_framework::model::attribute::Oem{}
    };

    return request;
}
