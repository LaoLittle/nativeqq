#ifndef NATIVEQQ_DEVICE_H
#define NATIVEQQ_DEVICE_H

#include <iostream>

namespace oicq {
    enum class DeviceType {
        Non,
        Ios, IosPAD,  // TODO
        Android, AndroidPad,
        MacOS, // TODO
        Linux, // TODO
        // not support Windows
    };

    enum class NetType {
        MOBILE_2G,
        MOBILE_3G,
        MOBILE_4G,
        MOBILE_5G,
        MOBILE_WIFI,
        Network
    };

    class Device {
    public:
        DeviceType deviceType = DeviceType::Non;
        NetType netType = NetType::MOBILE_WIFI;
        std::string imei;
        int sdkId;

        std::string qimei; // If it is in a low version QQ (<=8.4.8), please leave it empty to not provide.


        Device(DeviceType deviceType): deviceType(deviceType) {
            this->qimei = "";
        }

        std::string& getDeviceId() {
            return imei;
        }

        int getSubNetType() {
            auto sub = 0;
            if (netType == NetType::MOBILE_2G) {
                sub = 11 + 100;
            } else if (netType == NetType::MOBILE_3G) {
                sub = 12 + 100;
            } else if (netType == NetType::MOBILE_4G) {
                sub = 13 + 100;
            } else if (netType == NetType::MOBILE_5G) {
                sub = 20 + 0;
            } else if (netType == NetType::MOBILE_WIFI) {
                sub = 1;
            }
            return sub;
        }
    };
}

#endif //NATIVEQQ_DEVICE_H
