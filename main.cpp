#include <iostream>
#include "tup/tup.h"
#include "uvw.hpp"
#include "crypt/hmac_sha256.h"

#include "oicq/oicq.h"
#include "oicq/protocol.h"

using namespace tars;

int main() {
    try {
        auto aProtocol = std::make_shared<oicq::Android>();
        auto device = std::make_shared<oicq::Device>(oicq::DeviceType::Android);
        device->deviceId = "fa88ca2833f243c4";
        device->incremental = "2209.40000.28.0";
        device->fingerprint = "Windows/windows_x86_64/windows_x86_64:12/SQ3A.220705.004/2209.40000.28.0:user/release-keys";
        device->bootId = "e12b98ff-79c7-4139-b0b2-06f62495ed73";
        device->sdkId = 12; // Android version in 'fingerprint' needs to correspond to 'sdkId'
        device->model = "Subsystem for Android(TM)";
        device->brand = "Windows";

        oicq::Oicq oicq;
        oicq.device = device;
        oicq.setProtocol(aProtocol);
        oicq.getStByPwd(10001, "123456");
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}