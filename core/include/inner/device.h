#ifndef NATIVEQQ_DEVICE_H
#define NATIVEQQ_DEVICE_H

namespace oicq {
    enum class DeviceType {
        Non,
        Ios, IosPAD,  // TODO
        Android, AndroidPad,
        MacOS, // TODO
        Linux, // TODO
        // not support Windows
    };

    struct Device {
        DeviceType deviceType = DeviceType::Non;

    };
}

#endif //NATIVEQQ_DEVICE_H
