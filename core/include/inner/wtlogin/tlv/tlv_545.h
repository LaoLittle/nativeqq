#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv545_data(char* qimei, unsigned int qimei_len) {
        return build_tlv(0x545, [&](auto& pack){
            pack.write(qimei, qimei_len);
        });
    }
}