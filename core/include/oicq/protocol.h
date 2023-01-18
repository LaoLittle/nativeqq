#ifndef NATIVEQQ_PROTOCOL_H
#define NATIVEQQ_PROTOCOL_H

#include <utility>

#include "inner/device.h"

namespace oicq {
    typedef DeviceType ProtocolType;

    class ProtocolBase {
    public:
        ProtocolType type;
        int appId = 0x10;
        unsigned int subAppId;
        std::string detail;

        short dbVersion = 0;
        short ipVersion = 0;
        short pingVersion = 0;
        int ssoVersion = 0;

        short tgtgVersion = 0;

        int msfSsoVersion = 0;
        unsigned int builtTime = 0;

        std::string package;
        std::string version;
        std::string sdkVersion;

        std::shared_ptr<char[]> signMd5;

    public:
        ProtocolBase(ProtocolType type, unsigned int subAppId, std::string detail)
        : type(type), subAppId(subAppId), detail(detail) {

        }
    };

    class Android: public ProtocolBase {
    public:
        Android(): ProtocolBase(ProtocolType::Android, 0x200438E2, "testversion") {
            this->pingVersion = 0x1;
            this->ssoVersion = 0x600;
            this->ipVersion = 0x1;
            this->tgtgVersion = 0x4;
            this->msfSsoVersion = 0x13;
            this->dbVersion = 0x1;
            this->package = "com.tencent.mobileqq";
            this->appId = 0x10;
            this->builtTime = 0x639B02ED;
            this->version = "8.9.28";
            this->sdkVersion = "6.0.0.2532";
            this->signMd5 = std::shared_ptr<char[]>(new char[16]{
                -90, -73, 69, -65, 36, -94, -62, 119, 82, 119, 22, -10, -13, 110, -74, -115
            });
        }
    };
}

#endif //NATIVEQQ_PROTOCOL_H
