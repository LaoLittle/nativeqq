#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv107_data(int locale_id) {
        return build_tlv(0x107, [&](auto& pack){
            pack << (short) (0); // picType
            pack << (char) (0);
            pack << (short) (0);
            pack << (char) (1);
        });
    }
}