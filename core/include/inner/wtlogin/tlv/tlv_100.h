#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv100_data(short db_ver, int msf_sso_ver, int app_id, int sub_app_id,int main_sig_map) {
        return build_tlv(0x100, [&](auto& pack){
            pack << (short) (db_ver);
            pack << (int) (msf_sso_ver);
            pack << (int) (app_id);
            pack << (int) (sub_app_id);
            pack << (int) (0);
            pack << (int) (main_sig_map);
        });
    }
}