#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv142_data(char* package, unsigned int length) {
        return build_tlv(0x142, [&](auto& pack){
            pack << (short) (0);
            pack << (short) (length);
            pack.write(package, length);
        });
    }
}
