#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv516_data() {
        return build_tlv(0x516, [&](auto& pack){
            pack << (int) (0);
        });
    }
}
