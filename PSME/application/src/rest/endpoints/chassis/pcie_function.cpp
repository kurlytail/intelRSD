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
 * */

#include "psme/rest/endpoints/chassis/pcie_function.hpp"
#include "psme/rest/utils/status_helpers.hpp"



using namespace psme::rest;
using namespace psme::rest::constants;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#PCIeFunction.PCIeFunction";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#PCIeFunction.v1_0_0.PCIeFunction";
    r[Common::NAME] = "PCIe Function";

    r[constants::PcieFunction::CLASS_CODE] = json::Value::Type::NIL;
    r[Common::DESCRIPTION] = "PCIe Function description";
    r[constants::PcieFunction::DEVICE_CLASS] = json::Value::Type::NIL;
    r[constants::PcieFunction::DEVICE_ID] = json::Value::Type::NIL;
    r[constants::PcieFunction::FUNCTION_ID] = json::Value::Type::NIL;
    r[constants::PcieFunction::FUNCTION_TYPE] = json::Value::Type::NIL;
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[constants::PcieFunction::REVISION_ID] = json::Value::Type::NIL;

    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;

    r[constants::PcieFunction::SUBSYSTEM_ID] = json::Value::Type::NIL;
    r[constants::PcieFunction::SUBSYSTEM_VENDOR_ID] = json::Value::Type::NIL;
    r[constants::PcieFunction::VENDOR_ID] = json::Value::Type::NIL;

    r[Common::LINKS][constants::Chassis::DRIVES] = json::Value::Type::ARRAY;
    r[Common::LINKS][constants::PcieFunction::PCIE_DEVICE] = json::Value::Type::OBJECT;
    r[Common::LINKS][constants::StorageSubsystem::STORAGE_CONTROLLERS] = json::Value::Type::ARRAY;
    r[Common::LINKS][constants::System::ETHERNET_INTERFACES] = json::Value::Type::ARRAY;

    return r;
}


void fill_links(const agent_framework::model::PcieFunction& function,
                const std::string& device_uuid, json::Value& json) {
    if (!function.get_functional_device().has_value()) {
        return;
    }

    try {
        const auto drive_uuid = function.get_functional_device().value();

        json::Value drive_link;
        drive_link[Common::ODATA_ID] = endpoint::utils::get_component_url(
            agent_framework::model::enums::Component::Drive, drive_uuid);
        json[Common::LINKS][constants::Chassis::DRIVES].push_back(std::move(drive_link));

        const auto storage_subsystems = agent_framework::module::CommonComponents::get_instance()
            ->get_storage_subsystem_drives_manager().get_parents(drive_uuid);

        auto& controller_manager = agent_framework::module::ComputeComponents::get_instance()->get_storage_controller_manager();

        for (const auto& storage_uuid : storage_subsystems) {
            const auto controllers = controller_manager.get_keys(storage_uuid);
            for (const auto& controller_uuid: controllers) {
                json::Value controller_link;
                controller_link[Common::ODATA_ID] = endpoint::utils::get_component_url(
                    agent_framework::model::enums::Component::StorageController, controller_uuid);
                json[Common::LINKS][constants::StorageSubsystem::STORAGE_CONTROLLERS].push_back(std::move(controller_link));
            }
        }

        json[Common::LINKS][constants::PcieFunction::PCIE_DEVICE][Common::ODATA_ID] =
            endpoint::utils::get_component_url(agent_framework::model::enums::Component::PcieDevice, device_uuid);
    }
    catch (agent_framework::exceptions::InvalidUuid) {
        log_error(GET_LOGGER("rest"), "Function " + function.get_uuid() + " has functional device "
                                      + function.get_functional_device().value() +
                                      " which does not exist as a resource");
    }
}

}


endpoint::PcieFunction::PcieFunction(const std::string& path) : EndpointBase(path) {}


endpoint::PcieFunction::~PcieFunction() {}


void endpoint::PcieFunction::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    // the devices are under the same manager as the chassis from the URL
    auto chassis_manager_uuid = psme::rest::model::Find<agent_framework::model::Chassis>(
        req.params[PathParam::CHASSIS_ID]).get_one()->get_parent_uuid();

    const auto device =
        psme::rest::model::Find<agent_framework::model::PcieDevice>(req.params[PathParam::DEVICE_ID]).via(
            chassis_manager_uuid).get();

    const auto device_uuid = device.get_uuid();

    const auto function = psme::rest::model::Find<agent_framework::model::PcieFunction>(
        req.params[PathParam::FUNCTION_ID])
        .via(device_uuid).get();

    json[Common::ID] = req.params[PathParam::FUNCTION_ID];

    fill_links(function, device_uuid, json);

    json[constants::PcieFunction::CLASS_CODE] = function.get_pci_class_code();
    json[constants::PcieFunction::DEVICE_CLASS] = function.get_device_class();
    json[constants::PcieFunction::DEVICE_ID] = function.get_pci_device_id();
    try {
        if (function.get_function_id().has_value()) {
            json[constants::PcieFunction::FUNCTION_ID] = std::stoi(function.get_function_id());
        }
    }
    catch (const std::exception& ex) {
        log_warning(GET_LOGGER("rest"), "Invalid function id type:" << ex.what());
    }
    json[constants::PcieFunction::FUNCTION_TYPE] = function.get_function_type();
    json[constants::PcieFunction::REVISION_ID] = function.get_pci_revision_id();

    endpoint::status_to_json(function, json);
    json[Common::STATUS][Common::HEALTH_ROLLUP] = function.get_status().get_health();

    json[constants::PcieFunction::SUBSYSTEM_ID] = function.get_pci_subsystem_id();
    json[constants::PcieFunction::SUBSYSTEM_VENDOR_ID] = function.get_pci_subsystem_vendor_id();
    json[constants::PcieFunction::VENDOR_ID] = function.get_pci_vendor_id();

    set_response(res, json);
}
