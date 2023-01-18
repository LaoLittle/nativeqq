#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv542_data() {
        return build_tlv(0x542, [&](auto& pack){
            pack << (int) (0);
            // TODO(check sms)
        });
    }
}