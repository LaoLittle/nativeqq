#include <iostream>
#include "tup/tup.h"
#include "oicq/oicq.h"
#include "uvw.hpp"
#include "crypt/hmac_sha256.h"

#include <uv.h>
#include "uv/uv_thread.h"

using namespace tars;

int main() {
    try {
        oicq::Oicq oicq;

        oicq.getStByPwd(10001, "123456");
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}