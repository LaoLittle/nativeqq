#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv8_data(int locale_id) {
        return build_tlv(0x8, [&](auto& pack){
            pack << (short) (0);
            pack << locale_id;
            pack << (short) (0);
        });
    }
}