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
        device->imei = "867109044454081";
        device->sdkId = 10; // Android10

        oicq::Oicq oicq;
        oicq.device = device;
        oicq.setProtocol(aProtocol);
        oicq.getStByPwd(10001, "123456");
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}