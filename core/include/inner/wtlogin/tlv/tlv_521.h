#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv521_data() {
        return build_tlv(0x521, [&](auto& pack){
            pack << (int) (0);
            pack << (short) (0);
        });
    }
}