#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv116_data(int misc_bitmap, int sub_sig_map) {
        return build_tlv(0x116, [&](auto& pack){
            pack << (char) (0);
            pack << misc_bitmap;
            pack << sub_sig_map;
            pack << (short) (0);

            pack << (char) (1);
            pack << (unsigned int) (0x5F5E10E2);
        });
    }
}