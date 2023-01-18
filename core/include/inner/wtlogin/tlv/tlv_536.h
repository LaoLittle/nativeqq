#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

#include "inner/userdata.h"

namespace tlv {
    tars::TC_PackIn get_tlv536_data(std::vector<oicq::LoginExtraData>& vector) {
        return build_tlv(0x536, [&](auto& pack){
            pack << (char) (1);
            pack << (char) (vector.size());
            for (auto& data: vector) {
                pack << (long) data.uin;
                pack << (char) data.size;
                pack.write(data.ip.get(), data.size);
                pack << data.time;
                pack << data.appId;
            }
        });
    }
}