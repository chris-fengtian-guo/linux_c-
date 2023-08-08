enum class DeviceType {
    CONTROLLER,
    SWEEP_ROBOT,
};

DeviceType deviceType;
if (device_type == "controller") {
    deviceType = DeviceType::CONTROLLER;
} else if (device_type == "sweep_robot") {
    deviceType = DeviceType::SWEEP_ROBOT;
}
