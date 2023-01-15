#ifndef NATIVEQQ_OICQ_H
#define NATIVEQQ_OICQ_H

#define SUB_APPID 0x200302d5

#include <utility>

#include "inner/protocol.h"
#include "net/client.h"

#include "spdlog/spdlog.h"

namespace oicq {
    enum class OicqState {
        NoLogin, // not logged in
        HasToken, // Logged in successfully but not online
        Online, // online status
    };

    class Oicq {
    public:
        Oicq();

        ~Oicq();

        /**
         * Set protocol parameters
         * @param _protocol
         */
        void setProtocol(std::shared_ptr<ProtocolBase>& _protocol) {
            this->protocol = std::move(_protocol);
        }

        /**
         * password login
         * @param uin
         * @param pwdMd5
         */
        void getStByPwd(long uin, std::string pwdMd5);
    public:
        OicqState state = OicqState::NoLogin;

    protected:
        /**
         * All thread tasks are created here.
         */
        std::shared_ptr<uvw::Loop> defaultLoop;

        std::shared_ptr<ProtocolBase> protocol;

        /**
         * net package class
         */
        std::shared_ptr<OicqClient> client;
    };
}

#endif //NATIVEQQ_OICQ_H
