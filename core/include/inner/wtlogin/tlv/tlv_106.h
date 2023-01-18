#include "inner/wtlogin/tlv.h"

#include <string>
#include <util/tc_common.h>
#include <util/tc_tea.h>

namespace tlv {
    tars::TC_PackIn get_tlv106_data(
            char* user_md5,
            short tgtg_ver, int msf_sso_ver, int app_id, unsigned int uin,
            char* pwd_md5, char* tgtgt,
            bool guid_available, char* guid,
            int sub_app_id, int login_type = 1
            ) {
        return build_tlv(0x106, [&](auto& pack){
            tars::TC_PackIn packIn;
            packIn << tgtg_ver;
            packIn << std::rand();
            packIn << msf_sso_ver;
            packIn << app_id;
            packIn << 0;
            packIn << 0;
            packIn << uin;
            packIn << tars::TC_Common::now2s();
            pack.write("\u0000\u0000\u0000\u0000", 4);
            packIn << (char) (1);
            packIn.write(pwd_md5, 16);
            packIn.write(tgtgt, 16);
            packIn << 0;
            packIn << guid_available;
            packIn.write(guid, 16);
            packIn << sub_app_id;
            packIn << login_type;
            auto uin_str = to_string(uin);
            packIn << (short) (uin_str.length());
            packIn.write(uin_str.data(), uin_str.length());
            packIn << (short) (0);

            auto encrypt = tars::TC_Tea::encrypt(user_md5, packIn.topacket().data(), packIn.length());

            pack.write(encrypt.data(), encrypt.size());
        });
    }
}