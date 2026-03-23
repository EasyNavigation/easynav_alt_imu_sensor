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

#include <iostream>
#include <string>

#include "sensor_msgs/msg/imu.hpp"

#include "easynav_alt_imu_sensor/AltIMUPerceptionHandler.hpp"

namespace easynav_alt_imu
{

rclcpp::SubscriptionBase::SharedPtr
AltIMUPerceptionHandler::create_subscription(
  const std::string & topic,
  const std::string & type,
  std::shared_ptr<easynav::PerceptionBase> target,
  rclcpp::CallbackGroup::SharedPtr cb_group)
{
  if (type != "sensor_msgs/msg/Imu") {
    throw std::runtime_error(
      "Unsupported message type for AltIMUPerceptionHandler: " + type);
  }

  auto & node = *get_node();
  auto options = rclcpp::SubscriptionOptions();
  options.callback_group = cb_group;

  const auto clock_type = node.get_clock()->get_clock_type();

  return node.create_subscription<sensor_msgs::msg::Imu>(
    topic, rclcpp::QoS(1),
    [target, clock_type](const sensor_msgs::msg::Imu::SharedPtr msg)
    {
      std::cerr << "imu alternative\n";

      auto typed_target = std::dynamic_pointer_cast<easynav::IMUPerception>(target);
      typed_target->stamp = rclcpp::Time(msg->header.stamp, clock_type);
      typed_target->frame_id = msg->header.frame_id;
      typed_target->new_data = true;
      typed_target->data = *msg;
      typed_target->valid = true;
    },
    options);
}

}  // namespace easynav_alt_imu

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
  easynav_alt_imu::AltIMUPerceptionHandler, easynav::PerceptionHandler)
