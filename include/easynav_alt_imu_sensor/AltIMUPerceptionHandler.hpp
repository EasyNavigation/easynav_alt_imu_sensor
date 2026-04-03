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

/// \file
/// \brief Defines data structures and utilities for representing and processing IMU perceptions.
///
/// This file contains the AltIMUPerceptionHandler class, which handles subscriptions to IMU messages and transforms them into
/// IMUPerception instances. It also defines an alias for a collection of such perceptions.

#ifndef EASYNAV_ALT_IMU_SENSOR__ALTIMUPERCEPTIONHANDLER_HPP_
#define EASYNAV_ALT_IMU_SENSOR__ALTIMUPERCEPTIONHANDLER_HPP_

#include <string>
#include <vector>

#include "sensor_msgs/msg/imu.hpp"

#include "rclcpp_lifecycle/lifecycle_node.hpp"

#include "easynav_sensors/types/Perceptions.hpp"
#include "easynav_sensors/types/IMUPerception.hpp"

namespace easynav_alt_imu
{

/// \class AltIMUPerceptionHandler
/// \brief Handles the creation and updating of IMUPerception instances from sensor_msgs::msg::Imu messages.
///
/// This class provides methods to register subscriptions to IMU topics and update IMUPerception objects.
class AltIMUPerceptionHandler : public easynav::PerceptionHandler
{
public:
  /// \brief Optional post-initialization hook for subclasses.
  /// Here, the handler must reserve memory to store the perception data
  /// and create any Subscription or similar objects to read the data.
  void on_initialize() override;

  /// @brief Run one real-time sensor processing cycle.
  /// This method is called by the SensorsNode before executing its cycle_rt.
  /// Here the handler should update the NavState with the sensor data.
  /// If new data arrived before this call and the state is updated, it must return true.
  ///
  /// @param nav_state Pointer to the NavState to store the sensor data.
  /// @return True if new data was stored (to trigger processing).
  bool cycle_rt([[maybe_unused]] std::shared_ptr<easynav::NavState> nav_state) override;

private:
  /// \brief pointer to the perception data
  std::shared_ptr<easynav::IMUPerception> perception_data_ {nullptr};

  /// \brief pointer to the subscription object
  rclcpp::SubscriptionBase::SharedPtr perception_sub_;
};

/**
 * @typedef IMUPerceptions
 * @brief Alias for a vector of shared pointers to IMUPerception objects.
 *
 * The container can represent a time-ordered or batched collection, depending on producer logic.
 */
using IMUPerceptions =
  std::vector<std::shared_ptr<easynav::IMUPerception>>;

/// \brief Retrieves the latest timestamp among a set of IMU perceptions.
/// \param perceptions Container of IMU perceptions.
/// \return The most recent timestamp found in \p perceptions, or a default-constructed \c rclcpp::Time if \p perceptions is empty.
rclcpp::Time get_latest_imu_perceptions_stamp(const easynav::IMUPerceptions & perceptions);

}  // namespace easynav_alt_imu

#endif  // EASYNAV_ALT_IMU_SENSOR__ALTIMUPERCEPTIONHANDLER_HPP_
