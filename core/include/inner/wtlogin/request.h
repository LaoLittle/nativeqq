#ifndef NATIVEQQ_REQUEST_H
#define NATIVEQQ_REQUEST_H

#include <net/packet.h>
#include <oicq/oicq.h>
#include <util/tc_pack.h>

namespace oicq::wtlogin {
    enum class EncryptMethod: std::int8_t {
        EM_ST, EM_ECDH, EM_RB
    };

    class wt_request {
        virtual void make_tlv(tars::TC_PackIn& pack, oicq::Oicq *oicq);

        virtual oicq::PacketType get_packet_type();

        void build_encrypt_head(
                tars::TC_PackIn& pack, std::shared_ptr<char[]> randomKey, int ecdh_version,
                bool need_rollback = false, int rollback_flag = 0
        );

        void build_encrypt_data(tars::TC_PackIn& pack, oicq::Oicq* oicq);
    public:
        wt_request(std::string cmd, int cmdId, int subCmd): cmd(cmd), cmdId(cmdId), subCmd(subCmd) {
            assert(!cmd.empty());
        }

        void build(oicq::Oicq *oicq);
    protected:
        std::string cmd;
        int cmdId = 0;
        int subCmd = 0;
        int encType = 0;
        EncryptMethod encryptMethod = EncryptMethod::EM_ECDH;

        std::shared_ptr<char[]> firstToken;
        unsigned int firstTokenLen = 0;
        std::shared_ptr<char[]> secondToken;
        unsigned int secondTokenLen = 0;
    };

    // password login
    class wtlogin09: public wt_request {
        void make_tlv(tars::TC_PackIn& pack, oicq::Oicq *oicq) {

        }
    };
}

#endif //NATIVEQQ_REQUEST_H
