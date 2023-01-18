#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>
#include <tup/Protobuf.h>

namespace tlv {
    tars::TC_PackIn get_tlv52d_data(
            char* incremental, unsigned int incremental_len,
            char* fingerprint, unsigned int fingerprint_len,
            char* boot_id, unsigned int boot_id_len,
            char* device_id, unsigned int device_id_len
    ) {
        return build_tlv(0x52d, [&](auto& pack){
            tars::Protobuf protobuf;
            protobuf.writeBytes(1, "unknown", 7); // bootloader
            protobuf.writeBytes(2, "", 0); // version
            protobuf.writeBytes(3, "REL", 3); // codename
            protobuf.writeBytes(4, incremental, incremental_len); // incremental
            protobuf.writeBytes(5, fingerprint, fingerprint_len); // fingerprint
            protobuf.writeBytes(6, boot_id, boot_id_len); // boot_id
            protobuf.writeBytes(7, device_id, device_id_len); // device_id
            protobuf.writeBytes(8, "", 0); // baseband
            protobuf.writeBytes(9, incremental, incremental_len); // innerVer
            auto packet = protobuf.toPacket();
            pack.write(packet.data(), packet.size());
        });
    }
}