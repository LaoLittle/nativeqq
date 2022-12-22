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
        char* seed = TC_Common::str2bin("4f274c3f286b54372a40612428095143565e3140").data();
        auto out = ByteData::getByte(seed, 20);
        printf("out: %s\n", TC_Common::bin2str(out.data(), out.size()).c_str());

        oicq::Oicq oicq;
        const char* pwdMd5 = new char[16]{
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
        };
        // oicq.getStByPwd(1372362033, pwdMd5);
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}