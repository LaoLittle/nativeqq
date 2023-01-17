#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv1_data(short ip_version, unsigned int uin) {
        return build_tlv(0x1, [&](auto& pack){
            pack << ip_version;
            pack << rand();
            pack << uin;
            pack << tars::TC_Common::now2s();
            pack.write("\u0000\u0000\u0000\u0000", 4);
            pack << (short) (0);
        });
    }
}
