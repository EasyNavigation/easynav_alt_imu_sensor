// Copyright 2025 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef EASYNAV_ALT_IMU_SENSOR__ALTIMU_PERCEPTION_HANDLER_HPP_
#define EASYNAV_ALT_IMU_SENSOR__ALTIMU_PERCEPTION_HANDLER_HPP_

#include "easynav_sensors/types/IMUPerception.hpp"

namespace easynav_alt_imu
{

/// \brief Alternative IMU PerceptionHandler plugin.
///
/// Behaves identically to easynav::IMUPerceptionHandler but prints
/// "imu alternative" to std::cerr on every received message so it can
/// be confirmed that the alternative plugin is being used.
class AltIMUPerceptionHandler : public easynav::IMUPerceptionHandler
{
public:
  rclcpp::SubscriptionBase::SharedPtr create_subscription(
    const std::string & topic,
    const std::string & type,
    std::shared_ptr<easynav::PerceptionBase> target,
    rclcpp::CallbackGroup::SharedPtr cb_group) override;
};

}  // namespace easynav_alt_imu

#endif  // EASYNAV_ALT_IMU_SENSOR__ALTIMU_PERCEPTION_HANDLER_HPP_
