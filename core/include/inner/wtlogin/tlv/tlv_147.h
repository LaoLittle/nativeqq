#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv147_data(
        int app_id,
        char* version, int ver_len,
        char* signMd5
    ) {
        return build_tlv(0x147, [&](auto& pack){
            pack << app_id;
            pack << (short) (ver_len);
            pack.write(version, ver_len);
            pack << (short) (16);
            pack.write(signMd5, 16);
        });
    }
}