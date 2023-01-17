#include "inner/wtlogin/request.h"

#include <spdlog/spdlog.h>

namespace oicq::wtlogin {
    void wt_request::make_tlv(oicq::Oicq *oicq) {
        SPDLOG_ERROR("Incorrectly declared wt request");
    }

    void wt_request::build_encrypt_head(tars::TC_PackIn pack, std::shared_ptr<char[]> randomKey, int ecdh_version) {

    }

    oicq::PacketType wt_request::get_packet_type() {
        return PacketType::LoginPacket;
    }

    void wt_request::build(oicq::Oicq *oicq) {
        auto to = std::make_unique<ToService>();
        to->cmd = this->cmd;

    }
}