# <license_header>
#
# Copyright (c) 2015-2017 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# </license_header>

set(SOURCES
    error_codes.cpp
    gami_exception.cpp
    not_found.cpp
    invalid_field.cpp
    invalid_uuid.cpp
    invalid_value.cpp
    invalid_collection.cpp
    not_implemented.cpp
    method_not_allowed.cpp
    iscsi_error.cpp
    lvm_error.cpp
    fm10000_error.cpp
    ipmi_error.cpp
    unsupported_field.cpp
    unsupported_value.cpp
    certificate_error.cpp
    pcie_fabric_error.cpp
    network_error.cpp
)

add_library(exceptions OBJECT ${SOURCES})
