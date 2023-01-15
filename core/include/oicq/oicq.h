#ifndef NATIVEQQ_OICQ_H
#define NATIVEQQ_OICQ_H

#define SUB_APPID 0x200302d5

#include <utility>

#include "protocol.h"
#include "net/client.h"

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

        void setProtocol(ProtocolType _protocol) {
            this->protocol = _protocol;
        }

        std::shared_ptr<OicqClient> getClient() {
            return client;
        }

        std::shared_ptr<uvw::Loop> getLoop() {
            return defaultLoop;
        }

        /**
         * password login
         * @param uin
         * @param pwdMd5
         */
        void getStByPwd(long uin, std::string pwdMd5);
    public:
        ProtocolType protocol = ProtocolType::Non;
        OicqState state = OicqState::NoLogin;

    protected:
        /**
         * All thread tasks are created here.
         */
        std::shared_ptr<uvw::Loop> defaultLoop;

        /**
         * net package class
         */
        std::shared_ptr<OicqClient> client;
    };
}

#endif //NATIVEQQ_OICQ_H
