#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv154_data(int seq) {
        return build_tlv(0x154, [&](auto& pack){
            pack << seq;
        });
    }
}