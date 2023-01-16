#ifndef NATIVEQQ_OICQ_H
#define NATIVEQQ_OICQ_H

#include "inner/userdata.h"
#include "inner/device.h"
#include "inner/wtlogin.h"
#include "net/packet.h"
#include "net/client.h"
#include "protocol.h"

namespace oicq {
    class WloginHelper; // advance declaration of existence

    enum class OicqState {
        NoLogin, // not logged in
        HasToken, // Logged in successfully but not online
        Online, // online status
    };

    class Oicq {
    public:
        Oicq();

        ~Oicq();

        void setProtocol(std::shared_ptr<oicq::ProtocolBase> aProtocol) {
            this->protocolType = aProtocol->type;
            this->protocol = aProtocol;
        }

        std::shared_ptr<oicq::OicqClient> getClient() {
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

        void sendPacket(std::unique_ptr<oicq::ToService> toService);

    public:/*
         * Although I made these things public, I don't really recommend that you modify them.
         */
        /**
         * Device environment
         */
        std::shared_ptr<Device> device;
        /**
         * Protocol Type
         */
        ProtocolType protocolType = ProtocolType::Non;
        /**
         * Protocol Data Entity Class
         */
        std::shared_ptr<ProtocolBase> protocol;
        /**
         * Save some user data （generated before/login generated by qq login) to memory
         */
        UserData userData;
        /**
         * OicqClient Status
         */
        OicqState state = OicqState::NoLogin;

        /**
         * Do not ask
         */
        std::shared_ptr<oicq::WloginHelper> wtloginHelper;

    protected:
        /**
         * All thread tasks are created here.
         */
        std::shared_ptr<uvw::Loop> defaultLoop;

        /**
         * net package class
         */
        std::shared_ptr<oicq::OicqClient> client;
    };
}

#endif //NATIVEQQ_OICQ_H
