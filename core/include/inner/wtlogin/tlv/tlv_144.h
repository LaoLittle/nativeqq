#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <util/tc_tea.h>

#include <inner/wtlogin/tlv/tlv_109.h>
#include <inner/wtlogin/tlv/tlv_52d.h>
#include <inner/wtlogin/tlv/tlv_124.h>
#include <inner/wtlogin/tlv/tlv_128.h>
#include <inner/wtlogin/tlv/tlv_16e.h>

namespace tlv {
    tars::TC_PackIn get_tlv144_data(char* key,
        char* device_id, unsigned int device_id_len,
        char* incremental, unsigned int incremental_len,
        char* fingerprint, unsigned int fingerprint_len,
        char* boot_id, unsigned int boot_id_len,
        char* os_type, unsigned int os_type_len,
        int sdk_id, short net_type,
        char* sim, unsigned int sim_len,
        char* apn, unsigned int apn_len,
        bool guid_available, char* guid,
        char* model, unsigned int model_len,
        char* brand, unsigned int brand_len
    ) {
        return build_tlv(0x144, [&](auto& pack){
            tars::TC_PackIn packIn;
            packIn << (short) (5);
            packIn << tlv::get_tlv109_data(device_id, device_id_len);
            packIn << tlv::get_tlv52d_data(
                incremental, incremental_len,
                fingerprint, fingerprint_len,
                boot_id, boot_id_len,
                device_id, device_id_len
            );
            packIn << tlv::get_tlv124_data(
                os_type, os_type_len,
                sdk_id, net_type,
                sim, sim_len,
                apn, apn_len
            );
            packIn << tlv::get_tlv128_data(
                guid_available, guid,
                model, model_len,
                brand, brand_len
            );
            packIn << tlv::get_tlv16e_data(
                model, model_len
            );
            auto encrypt = tars::TC_Tea::encrypt(key, packIn.topacket().data(), packIn.length());
            pack.write(encrypt.data(), encrypt.size());
        });
    }
}