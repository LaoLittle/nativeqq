#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <util/tc_md5.h>

namespace tlv {
    tars::TC_PackIn get_tlv187_data(char* mac_address, unsigned int mac_address_len) {
        return build_tlv(0x187, [&](auto& pack){
            auto md5 = tars::TC_MD5::md5bin(mac_address, mac_address_len);
            pack.write(md5.data(), md5.size());
        });
    }
}