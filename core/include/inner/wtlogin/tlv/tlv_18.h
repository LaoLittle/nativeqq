#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv18_data(short ping_version, int sso_ver, int app_id, unsigned int uin) {
        return build_tlv(0x18, [&](auto& pack){
            pack << (short) (ping_version);
            pack << (int) (sso_ver);
            pack << (int) (app_id);
            pack << (int) (0);
            pack << uin;
            pack << (int) (0);
        });
    }
}