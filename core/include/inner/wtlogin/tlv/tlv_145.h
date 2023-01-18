#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv145_data(char* guid) {
        return build_tlv(0x145, [&](auto& pack){
            pack.write(guid, 16);
        });
    }
}