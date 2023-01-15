#ifndef NATIVEQQ_PROTOCOL_H
#define NATIVEQQ_PROTOCOL_H

#include <utility>

#include "inner/device.h"

namespace oicq {
    typedef DeviceType ProtocolType;

    class ProtocolBase {
    public:
        ProtocolType type;
        long appId = 0x10;
        long subAppId;

        ProtocolBase(ProtocolType type, long subAppId): type(type), subAppId(subAppId) {

        }
    };

    class Android: public ProtocolBase {
    public:
        Android(): ProtocolBase(ProtocolType::Android, 0x200302d5) {

        }
    };
}

#endif //NATIVEQQ_PROTOCOL_H
