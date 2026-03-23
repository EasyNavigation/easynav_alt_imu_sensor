# easynav_alt_imu_sensor

Example package showing how to implement a custom `PerceptionHandler` plugin for [EasyNavigation](https://github.com/IntelligentRoboticsLabs/EasyNavigation).

## Overview

EasyNavigation's sensor layer (`easynav_sensors`) uses [pluginlib](https://docs.ros.org/en/rolling/Tutorials/Beginner-Client-Libraries/Pluginlib.html) to load **PerceptionHandler** plugins at runtime. Each handler is responsible for:

1. Subscribing to a ROS 2 topic.
2. Filling a `PerceptionBase`-derived object with the incoming data.
3. Exposing the typed perception to the rest of the navigation stack via `NavState`.

This package provides `AltIMUPerceptionHandler`, which inherits from the built-in `IMUPerceptionHandler` and overrides `create_subscription` to print `"imu alternative"` to `stderr` on every received message. The functional behaviour is otherwise identical to the default handler. It serves as a minimal, self-contained reference for writing your own handler.

## Plugin interface

A `PerceptionHandler` plugin must:

| Method | Required | Description |
|---|---|---|
| `group()` | Yes | Returns the key used to store perceptions in `NavState` (e.g. `"imu"`). |
| `create()` | Yes | Returns a new, empty `PerceptionBase`-derived instance. |
| `create_subscription()` | Yes | Creates and returns a ROS 2 subscription that populates the target perception. |
| `populate_nav_state()` | Yes | Casts and writes the typed perception vector into `NavState`. |
| `on_initialize()` | No | Optional hook called after `initialize()` to perform extra setup. |

The handler is initialised by the `SensorsNode` before `create_subscription` is called. The parent lifecycle node and sensor name are available via `get_node()` and `get_sensor_name()`.

## Package structure

```
easynav_alt_imu_sensor/
├── include/easynav_alt_imu_sensor/
│   └── AltIMUPerceptionHandler.hpp   # Class declaration
├── src/easynav_alt_imu_sensor/
│   └── AltIMUPerceptionHandler.cpp   # Implementation + PLUGINLIB_EXPORT_CLASS
├── easynav_alt_imu_sensor_plugins.xml  # pluginlib descriptor
├── CMakeLists.txt
└── package.xml
```

### Key files

**`AltIMUPerceptionHandler.hpp`** – declares the class inside the `easynav_alt_imu` namespace, inheriting from `easynav::IMUPerceptionHandler`:

```cpp
class AltIMUPerceptionHandler : public easynav::IMUPerceptionHandler
{
public:
  rclcpp::SubscriptionBase::SharedPtr create_subscription(
    const std::string & topic,
    const std::string & type,
    std::shared_ptr<easynav::PerceptionBase> target,
    rclcpp::CallbackGroup::SharedPtr cb_group) override;
};
```

**`AltIMUPerceptionHandler.cpp`** – implements `create_subscription` and registers the plugin:

```cpp
PLUGINLIB_EXPORT_CLASS(
  easynav_alt_imu::AltIMUPerceptionHandler, easynav::PerceptionHandler)
```

**`easynav_alt_imu_sensor_plugins.xml`** – tells pluginlib how to find the class:

```xml
<class_libraries>
  <library path="easynav_alt_imu_sensor">
    <class name="easynav_alt_imu_sensor/AltIMUPerceptionHandler"
           type="easynav_alt_imu::AltIMUPerceptionHandler"
           base_class_type="easynav::PerceptionHandler">
      <description>Alternative IMU perception handler.</description>
    </class>
  </library>
</class_libraries>
```

**`CMakeLists.txt`** – exports the plugin descriptor so `SensorsNode` can discover it:

```cmake
pluginlib_export_plugin_description_file(
  easynav_sensors easynav_alt_imu_sensor_plugins.xml)
```

## Using the plugin

In a sensor configuration file, set the `plugin` parameter for the sensor that should use this handler:

```yaml
sensors_node:
  ros__parameters:
    sensors: [imu]
    imu:
      topic: imu/data
      type: sensor_msgs/msg/Imu
      plugin: easynav_alt_imu_sensor/AltIMUPerceptionHandler
```

When `plugin` is omitted, `SensorsNode` auto-detects the default handler from the message type. Providing an explicit `plugin` value overrides that default, which is exactly what a custom plugin requires.

## Dependencies

| Package | Role |
|---|---|
| `easynav_sensors` | Provides `PerceptionHandler`, `IMUPerceptionHandler`, `IMUPerception` |
| `pluginlib` | Plugin loading infrastructure |
| `rclcpp_lifecycle` | Lifecycle node API |
| `sensor_msgs` | `sensor_msgs/msg/Imu` message type |

## Building

```bash
cd <your_ros2_ws>
colcon build --packages-select easynav_alt_imu_sensor
source install/setup.bash
```

The package requires `easynav_sensors` to be available in the workspace or installed in the ROS 2 prefix.

## License

Apache License 2.0 — see [LICENSE](LICENSE) for details.
