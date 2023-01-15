#ifndef NATIVEQQ_PROTOCOL_H
#define NATIVEQQ_PROTOCOL_H

#include <utility>

#include "inner/device.h"

namespace oicq {
    typedef DeviceType ProtocolType;

    struct ProtocolBase {
        ProtocolType type;
        long appId = 0x10;
        long subAppId;

        ProtocolBase(ProtocolType type, long subAppId) {
            this->type = type;
            this->subAppId = subAppId;
        }
    };

    struct Android: ProtocolBase {


        Android(): ProtocolBase(ProtocolType::Android, 666) {

        }
    };
}

#endif //NATIVEQQ_PROTOCOL_H
