#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv177_data(unsigned int built_time, char* sdk_ver, unsigned int sdk_ver_len) {
        return build_tlv(0x177, [&](auto& pack){
            pack << true;
            pack << built_time;
            pack << (short) (sdk_ver_len);
            pack.write(sdk_ver, sdk_ver_len);
        });
    }
}