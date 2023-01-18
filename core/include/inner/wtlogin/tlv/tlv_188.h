#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <util/tc_md5.h>

namespace tlv {
    tars::TC_PackIn get_tlv188_data(char* device_id, unsigned int device_id_len) {
        return build_tlv(0x188, [&](auto& pack){
            auto md5 = tars::TC_MD5::md5bin(device_id, device_id_len);
            pack.write(md5.data(), md5.size());
        });
    }
}