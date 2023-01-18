#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv141_data(
        char* sim, unsigned int sim_len,
        short net_type,
        char* apn, unsigned int apn_len
    ) {
        return build_tlv(0x141, [&](auto& pack){
            pack << (short) (1); // version

            pack << (short) (sim_len);
            pack.write(sim, sim_len);
            pack << (short) (net_type);
            pack << (short) (apn_len);
            pack.write(apn, apn_len);
        });
    }
}