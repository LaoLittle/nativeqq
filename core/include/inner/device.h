#ifndef NATIVEQQ_DEVICE_H
#define NATIVEQQ_DEVICE_H

#include <iostream>
#include <memory>
#include <util/tc_md5.h>

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
        std::shared_ptr<char[]> guid;

    public:
        DeviceType deviceType = DeviceType::Non;
        NetType netType = NetType::MOBILE_WIFI;

        std::string deviceId; // If the Android version is not high, please use imei
        std::string incremental;
        std::string fingerprint;
        std::string bootId;
        std::string model;
        std::string brand;
        int sdkId = 0;
        std::string sim; // e.g. 中国移动, 小米移动, 中国电信
        std::string apn; // e.g. cmcc/wifi
        std::string macAddress;
        std::string qimei; // If it is in a low version QQ (<=8.4.8), please leave it empty to not provide.

        std::shared_ptr<char[]> tgtgt;

    public:
        explicit Device(DeviceType deviceType): deviceType(deviceType) {
            this->qimei = "";
            this->sim = "";
            this->apn = "cmcc";
            this->macAddress = "02:00:00:00:00:00";
        }

    public:
        virtual bool isGuidAvailable() {
            return true;
        }

        std::shared_ptr<char[]> getGuid() {
            if (!guid) {
                auto md5 = tars::TC_MD5::md5bin(deviceId.append(macAddress));
                guid = std::make_shared<char[]>(16);
                std::copy(md5.begin(), md5.end(), guid.get());
            }
            return guid;
        }

        std::string getApnName() const {
            if (netType == NetType::MOBILE_WIFI) {
                return "wifi";
            } else {
                return apn;
            }
        }

        std::string getOsType() const {
            if (deviceType == DeviceType::Android ||
            deviceType == DeviceType::AndroidPad
            ) {
                return "android";
            } else {
                // TODO(Other platform OS Type)
                return "unknown";
            }
        }

        std::string& getDeviceId() {
            return deviceId;
        }

        short getNetType() const {
            if (netType == NetType::MOBILE_WIFI) {
                return 2;
            } else if (netType == NetType::Network) {
                return 0;
            }
            return 1;
        }

        int getSubNetType() const {
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
