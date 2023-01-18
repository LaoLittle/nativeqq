#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <string>

namespace tlv {
    tars::TC_PackIn get_tlv124_data(
            char* os_type, unsigned int os_type_len,
            int sdk_id, short net_type,
            char* sim, unsigned int sim_len,
            char* apn, unsigned int apn_len
    ) {
        return build_tlv(0x124, [&](auto& pack){
            // os type
            pack << (short) (os_type_len);
            pack.write(os_type, os_type_len);

            // os ver
            auto os_ver = to_string(sdk_id);
            pack << (short) (os_ver.length());
            pack.write(os_ver.data(), os_ver.size());

            // net type
            // 1 mobile net
            // 2 wifi
            // 0 other
            pack << (short) (net_type);

            // sim info(operator name)
            // e.g. 中国移动，中国联通，小米移动...
            if (net_type == 1) { // Can submit only on mobile network
                pack << (short) (sim_len);
                pack.write(sim, sim_len);
            }

            pack << (int) (apn_len);
            pack.write(apn, apn_len);
        });
    }
}
