#include "inner/wtlogin/request.h"

#include <spdlog/spdlog.h>

namespace oicq::wtlogin {
    void wt_request::make_tlv(tars::TC_PackIn& pack, oicq::Oicq *oicq) {
        SPDLOG_ERROR("Incorrectly declared wt request");
    }

    void wt_request::build_encrypt_head(
            tars::TC_PackIn& pack, std::shared_ptr<char[]> randomKey, int ecdh_version,
            bool need_rollback, int rollback_flag
            ) {
        if (!need_rollback) {
            pack << (char) (2);
            pack << (char) (1);
            pack.write(randomKey.get(), 16);
            pack << (short) (0x131);
            pack << (short) (ecdh_version);
        } else {
            pack << (char) (1);
            pack << (char) (rollback_flag == 2 ? 3 : 2);
            pack.write(randomKey.get(), 16);
            pack << (short) (0x102);
            pack << (short) (0);
        }
    }

    void wt_request::build_encrypt_data(tars::TC_PackIn& pack, oicq::Oicq* oicq) {
        auto& userData = oicq->userData;
        auto& session = userData.session;
        auto& ecdh = session.ecdh;

        std::shared_ptr<char[]> public_key;
        unsigned int public_key_size = 0;
        if (encryptMethod == EncryptMethod::EM_ST) {
            auto& ticket = userData.sigInfo.wtSessionTicket;
            public_key = ticket.ticket;
            public_key_size = ticket.size;
        } // encryptMethod == EncryptMethod::EM_ST
        else if (encryptMethod == EncryptMethod::EM_ECDH) {
            public_key = ecdh.public_key;
            public_key_size = ecdh.public_key_size;
        } // encryptMethod == EncryptMethod::EM_ECDH
        if (encryptMethod != EncryptMethod::EM_ST) {
            if (encryptMethod == EncryptMethod::EM_RB) {
                build_encrypt_head(pack, session.randomKey, ecdh.ecdh_version, true, session.rollbackFlag);
            } else {
                build_encrypt_head(pack, session.randomKey, ecdh.ecdh_version);
            } // encryptMethod == EncryptMethod::EM_RB
        } // encryptMethod != EncryptMethod::EM_ST
        if (encryptMethod != EncryptMethod::EM_RB) {
            pack << (short) (public_key_size);
            pack.write(public_key.get(), public_key_size);
        } // encryptMethod != EncryptMethod::EM_RB

        tars::TC_PackIn bodyPack;
        bodyPack << (short) (this->subCmd);

    }

    oicq::PacketType wt_request::get_packet_type() {
        return PacketType::LoginPacket;
    }

    void wt_request::build(oicq::Oicq *oicq) {
        auto to = std::make_unique<ToService>();
        to->cmd = this->cmd;
        to->seq = oicq->userData.session.nextSeq();


    }
}