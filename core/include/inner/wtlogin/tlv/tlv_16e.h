#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv16e_data(char* model, unsigned int model_len) {
        return build_tlv(0x16e, [&](auto& pack){
            pack.write(model, model_len);
        });
    }
}