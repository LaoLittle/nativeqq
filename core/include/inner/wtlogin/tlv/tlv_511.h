#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    vector<std::string> get_base_domains();

    tars::TC_PackIn get_tlv511_data(vector<std::string> domains = get_base_domains()) {
        return build_tlv(0x511, [&](auto& pack){
            pack << (short) (domains.size());
            for (auto &domain: domains) {
                auto end = domain.find(")");
                char b = 1;
                if (domain.find('(') == 0 && end < domain.size()) {
                    auto tmp = atoi(domain.substr(1, end - 1).c_str());
                    if ((1048576 & tmp) <= 0) {
                        b = 0;
                    }
                    if ((tmp & 0x08000000) > 0) {
                        b = b | 2;
                    }
                }
                pack << b;
                pack << (short) domain.size();
                pack.write(domain.data(), domain.size());
            }
        });
    }

    vector<std::string> get_base_domains(){
        vector<std::string> domains(14);
        domains[0] = "office.qq.com";
        domains[1] = "qun.qq.com";
        domains[2] = "gamecenter.qq.com";
        domains[3] = "docs.qq.com";
        domains[4] = "mail.qq.com";
        domains[5] = "ti.qq.com";
        domains[6] = "vip.qq.com";
        domains[7] = "tenpay.com";
        domains[8] = "qqweb.qq.com";
        domains[9] = "qzone.qq.com";
        domains[10] = "mma.qq.com";
        domains[11] = "game.qq.com";
        domains[12] = "openmobile.qq.com";
        domains[13] = "connect.qq.com";
        return domains;
    }
}