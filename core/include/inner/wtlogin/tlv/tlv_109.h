#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <util/tc_md5.h>

namespace tlv {
    tars::TC_PackIn get_tlv109_data(char* device_id, unsigned int len) {
        return build_tlv(0x109, [&](auto& pack){
            auto md5 = tars::TC_MD5::md5bin(device_id, len);
            pack.write(md5.data(), md5.size());
        });
    }
}