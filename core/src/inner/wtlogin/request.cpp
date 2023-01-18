#include "inner/wtlogin/request.h"

#include <util/tc_tea.h>

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
        make_tlv(bodyPack, oicq);
        std::shared_ptr<char[]> teaKey;
        if (encryptMethod == EncryptMethod::EM_ST) {
            teaKey = userData.sigInfo.wtSessionTicketKey;
        } else if (encryptMethod == EncryptMethod::EM_ECDH) {
            teaKey = ecdh.share_key;
        } else if (encryptMethod == EncryptMethod::EM_RB) {
            teaKey = session.randomKey;
        }
        auto encrypt = tars::TC_Tea::encrypt(teaKey.get(), bodyPack.topacket().data(), bodyPack.length());
        pack.write(encrypt.data(), encrypt.size());
    }

    tars::TC_PackIn wt_request::build_body_data(oicq::Oicq *oicq) {
        tars::TC_PackIn encrypt;
        build_encrypt_data(encrypt, oicq);

        tars::TC_PackIn pack;
        pack << (char) (2);
        pack << (short) (27 + encrypt.length() + 2);
        pack << (short) (8001);
        pack << (short) (cmdId);
        pack << (short) (1);
        pack << oicq->userData.uin;

        pack << (char) (3);
        pack << (char) (encType);
        pack << (char) (0);
        pack << (int) (2);
        pack << (int) (0);
        pack << (int) (0);

        pack << encrypt;

        pack << (char) (3);

        return pack;
    }

    oicq::PacketType wt_request::get_packet_type() {
        return PacketType::LoginPacket;
    }

    void wt_request::build(oicq::Oicq *oicq) {
        auto to = std::make_unique<ToService>();
        to->cmd = this->cmd;
        to->seq = oicq->userData.session.nextSeq();
        this->seq = to->seq;
        auto body = build_body_data(oicq);
        to->dataLen = body.length();
        to->data.reset(body.topacket().data());

        to->firstToken = this->firstToken.ticket;
        to->firstTokenLen = this->firstToken.size;
        to->secondToken = this->secondToken.ticket;
        to->secondToken = this->secondToken.size;

        oicq->sendPacket(std::move(to));
    }
}