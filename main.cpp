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
        //char* seed = TC_Common::str2bin("4f274c3f286b54372a40612428095143565e3140").data();
        std::vector<char> key = ByteData::_gen_key();
        //uint8_t *out;
        //basic_string<char> data = TC_Common::str2bin("313632363430333535312c6e303033396579316d6d642c6e756c6c0000017aad34b7af");
        // hmac_sha256(out,(const uint8_t *) data.data(), data.size(),(const uint8_t *) key.data(), key.size());
        printf("out: %s\n", TC_Common::bin2str(key.data()).data());
        ByteData::getByte(key.data(), key.size());
        // printf("out: %s\n", TC_Common::bin2str(out).data());

        // aba3293cbe3f855642c6918c0ce8e2a60616ff834744cac3266d6f0ce43ec664

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