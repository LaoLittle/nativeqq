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
        std::string detail;

        ProtocolBase(ProtocolType type, long subAppId, std::string detail)
        : type(type), subAppId(subAppId), detail(detail) {

        }
    };

    class Android: public ProtocolBase {
    public:
        Android(): ProtocolBase(ProtocolType::Android, 0x200302d5, "testversion") {

        }
    };
}

#endif //NATIVEQQ_PROTOCOL_H
