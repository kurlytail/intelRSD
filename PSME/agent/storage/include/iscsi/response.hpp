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
 * @file response.hpp
 * @brief tgt response inteface
*/

#pragma once
#include "errors.hpp"
#include <string>
#include <vector>

namespace agent {
namespace storage {
namespace iscsi {
namespace tgt {

/*! POD response data */
struct ResponseData {
    std::uint32_t m_error{std::uint32_t(Errors::Types::UNKNOWN_ERROR)};
    std::uint32_t m_length{};
};

/*! Class represents tgtadm response */
class Response {
   public:
    /*! additional data type */
    using extra_data_t = std::vector<char>;

    /*!
     * @brief Get response raw pointer
     * @return Return raw pointer to response POD data
     */
    char* data();

    /*!
     * @brief Get response size
     * @return response size
     */
    std::size_t get_response_pod_size() const;

    /*!
     * @brief Get response error code
     * @return Error code
     */
    Errors::Types get_error() const;

    /*!
     * @brief Check if response if valid
     * @return True on success, otherwise false
     */
    bool is_valid() const;

    /*!
     * @brief Get response additional length
     * @return Additional length
     */
    std::uint32_t get_length() const;

    /*!
     * @brief Get extra data from response
     * @return Extra data
     */
    extra_data_t& get_extra_data();

private:
    ResponseData m_response_data{};
    extra_data_t m_extra_data{};
};
}
}
}
}

