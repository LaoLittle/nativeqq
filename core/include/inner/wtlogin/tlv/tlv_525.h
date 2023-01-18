#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <inner/wtlogin/tlv/tlv_536.h>

namespace tlv {
    tars::TC_PackIn get_tlv525_data(std::vector<oicq::LoginExtraData>& login_extra_data) {
        return build_tlv(0x525, [&](auto& pack){
            pack << (short) (1);
            pack << tlv::get_tlv536_data(login_extra_data);
        });
    }
}