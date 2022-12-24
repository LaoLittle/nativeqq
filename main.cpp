#include <iostream>
#include "util/tc_tea.h"
#include "util/tc_common.h"
#include "util/tc_pack.h"
#include "tup/tup.h"
#include "tup/Protobuf.h"
#include "oicq/oicq.h"
#include "uvw.hpp"
#include "oicq/util/oicq_util.h"
#include "crypt/bytedata.h"
#include "crypt/hmac_sha256.h"

using namespace tars;

int main() {
    try {
        oicq::Oicq oicq;
        const char* pwdMd5 = new char[16]{
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
        };
        oicq.getStByPwd(1372362033, pwdMd5);
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}