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
        oicq::Oicq oicq;
        oicq.setProtocol(oicq::ProtocolType::Linux);
        oicq.getStByPwd(10001, "123456");
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}