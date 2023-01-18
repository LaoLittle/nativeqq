#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv191_data() {
        return build_tlv(0x191, [&](auto& pack){
            pack << (char) (0x82);
        });
    }
}