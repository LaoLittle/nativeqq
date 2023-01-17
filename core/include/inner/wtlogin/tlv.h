#ifndef NATIVEQQ_TLV_H
#define NATIVEQQ_TLV_H

#include "util/tc_pack.h"
#include <functional>

namespace tlv {
    tars::TC_PackIn build_tlv(short type, std::function<void(tars::TC_PackIn&)> value) {
        tars::TC_PackIn pack;
        pack << type;
        pack.writeBlockWithShortLen(value, 0);
        return pack;
    }
}

#endif //NATIVEQQ_TLV_H
