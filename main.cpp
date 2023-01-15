#include <iostream>
#include "tup/tup.h"
#include "uvw.hpp"
#include "crypt/hmac_sha256.h"

#include <uv.h>
#include "uv/uv_thread.h"

#include "oicq/oicq.h"
#include "oicq/protocol.h"

using namespace tars;

int main() {
    try {
        auto aProtocol = std::make_shared<oicq::Android>();
        oicq::Oicq oicq;
        oicq.device.deviceType = oicq::DeviceType::Android;
        oicq.setProtocol(aProtocol);
        oicq.getStByPwd(10001, "123456");
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}