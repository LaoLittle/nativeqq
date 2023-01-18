#ifndef NATIVEQQ_WTLOGIN09_H
#define NATIVEQQ_WTLOGIN09_H

#include "inner/wtlogin/request.h"
#include <util/tc_pack.h>

#include "inner/wtlogin/tlv/tlv_1.h"
#include "inner/wtlogin/tlv/tlv_8.h"
#include "inner/wtlogin/tlv/tlv_18.h"
#include "inner/wtlogin/tlv/tlv_106.h"
#include "inner/wtlogin/tlv/tlv_116.h"
#include "inner/wtlogin/tlv/tlv_100.h"
#include "inner/wtlogin/tlv/tlv_107.h"
#include "inner/wtlogin/tlv/tlv_142.h"
#include "inner/wtlogin/tlv/tlv_144.h"
#include "inner/wtlogin/tlv/tlv_145.h"
#include "inner/wtlogin/tlv/tlv_147.h"
#include "inner/wtlogin/tlv/tlv_154.h"
#include "inner/wtlogin/tlv/tlv_141.h"
#include "inner/wtlogin/tlv/tlv_511.h"
#include "inner/wtlogin/tlv/tlv_187.h"
#include "inner/wtlogin/tlv/tlv_188.h"
#include "inner/wtlogin/tlv/tlv_191.h"
#include "inner/wtlogin/tlv/tlv_177.h"
#include "inner/wtlogin/tlv/tlv_516.h"
#include "inner/wtlogin/tlv/tlv_521.h"
#include "inner/wtlogin/tlv/tlv_525.h"
#include "inner/wtlogin/tlv/tlv_545.h"

namespace oicq::wtlogin {
    // password login
    class wtlogin09: public wt_request {
        void make_tlv(tars::TC_PackIn& pack, oicq::Oicq *oicq) {
            auto& userData = oicq->userData;
            auto& session = userData.session;
            auto protocol = oicq->protocol;
            auto device = oicq->device;

            pack << 22; // tlv size
            pack << tlv::get_tlv18_data(protocol->pingVersion, protocol->ssoVersion, protocol->appId, userData.uin);
            pack << tlv::get_tlv1_data(protocol->ipVersion, userData.uin);
            auto guid = device->getGuid().get();
            pack << tlv::get_tlv106_data(userData.userMd5.data(), protocol->tgtgVersion,protocol->msfSsoVersion,
                protocol->appId, userData.uin, userData.pwdMd5.data(),
                device->tgtgt.get(), device->isGuidAvailable(), guid, protocol->subAppId
            );
            pack << tlv::get_tlv116_data();
            pack << tlv::get_tlv100_data();
            pack << tlv::get_tlv107_data();
            auto package = protocol->package;
            pack << tlv::get_tlv142_data(package.data(), package.size());
            auto device_id = device->getDeviceId();
            auto os_type = device->getOsType();
            auto& sim = device->sim;
            auto net_type = device->getNetType();
            auto apn = device->getApnName();
            {
                // Avoid unreasonable copying + excessive stack push without release
                auto& incremental = device->incremental;
                auto& fingerprint = device->fingerprint;
                auto& boot_id = device->bootId;
                auto& model = device->model;
                auto& brand = device->brand;
                pack << tlv::get_tlv144_data(
                    device->tgtgt.get(),
                    device_id.data(), device_id.size(),
                    incremental.data(), incremental.size(),
                    fingerprint.data(), fingerprint.size(),
                    boot_id.data(), boot_id.size(),
                    os_type.data(), os_type.size(),
                    device->sdkId, net_type,
                    sim.data(), sim.size(),
                    apn.data(), apn.size(),
                    true, guid,
                    model.data(), model.size(),
                    brand.data(), brand.size()
                );
                // Set scope so that the copied value is released when leave.
            } // tlv_144
            pack << tlv::get_tlv145_data(guid);
            auto& version = protocol->version;
            pack << tlv::get_tlv147_data(protocol->appId, version.data(), version.size(), protocol->signMd5.get());
            pack << tlv::get_tlv154_data(this->seq);
            pack << tlv::get_tlv141_data(sim.data(), sim.size(), net_type, apn.data(), apn.size());
            pack << tlv::get_tlv8_data(2052);
            pack << tlv::get_tlv511_data();
            auto& mac = device->macAddress;
            pack << tlv::get_tlv187_data(mac.data(), mac.size());
            pack << tlv::get_tlv188_data(device_id.data(), device_id.size());
            pack << tlv::get_tlv191_data();
            auto& sdk_ver = protocol->sdkVersion;
            pack << tlv::get_tlv177_data(protocol->builtTime, sdk_ver.data(), sdk_ver.size());
            pack << tlv::get_tlv516_data();
            pack << tlv::get_tlv521_data();
            pack << tlv::get_tlv525_data(userData.sigInfo.vector);
            // tlv 544
            auto& qimei = device->qimei;
            pack << tlv::get_tlv545_data(qimei.data(), qimei.length());
        }
    public:
        wtlogin09(): wt_request("wtlogin.login", 0x810, 0x9) {
            this->encType = 0x87;
        }
    };
}

#endif //NATIVEQQ_WTLOGIN09_H
