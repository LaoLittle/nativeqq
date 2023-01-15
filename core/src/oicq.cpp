#include "oicq/oicq.h"
#include "oicq/util/oicq_util.h"
#include "util/tc_md5.h"

#include "spdlog/spdlog.h"

using namespace uvw;

namespace oicq {
    Oicq::Oicq() {
#ifdef USE_NEW_LOOP
        // Obedient, use this!
        auto loop = Loop::getDefault();
#else
        // Created for what?
        // How many QQ do you want to keep?
        // Badass! ! ! !
        auto loop = Loop::create();
#endif
        this->defaultLoop = loop;
    }

    Oicq::~Oicq() {
        this->defaultLoop->walk([](auto &&h){
            if (!h.closing()) {
                h.close();
            }
        });
    }

    void Oicq::getStByPwd(long uin, std::string pwd) {
        SPDLOG_INFO("Start pre login!");
        if (this->protocolType == ProtocolType::Non) {
            // How can it run without setting the protocol type?
            SPDLOG_ERROR("No specific protocol is set.");
            return;
        } else if(this->protocolType != this->device.deviceType) {
            // The protocol type is different from the device type.
            // How do you run your 'IOSQQ' in the Android environment?
            SPDLOG_ERROR("Device type and Protocol type do not match.");
            return;
        } else if(this->state == OicqState::Online) {
            // If you are online, don't log in, okay!
            SPDLOG_ERROR("Already online, repeated login is prohibited.");
            return;
        } else {
            this->client = std::make_shared<OicqClient>(
                    this->defaultLoop
            ); // By creating a smart pointer, the previous client will be automatically released (when reference == 0)
            uvw::Addr addr;
            if (oicq::OICQ_Common::get_oicq_address(&addr, uin, "867109044454081", this->protocol->subAppId, "") == 200) {
                auto handle = this->client->getTcpHandle();
                // SPDLOG_INFO("Thread(Oicq::getStByPwd) {}", uv_thread_self()); // Only to test which the thread is.
                this->userData.uin = uin;
                this->userData.pwdMd5 = std::move(tars::TC_MD5::md5bin(pwd)); // Will this be a problem?
                handle->once<ConnectEvent>([&, this](const ConnectEvent &, TCPHandle &handle) {
                    SPDLOG_INFO("Successfully connected to the server({}:{}).", addr.ip, addr.port); // Why is it a yellow warning?
                    auto wlogin = std::make_shared<oicq::WloginHelper>(this);
                    this->wtloginHelper = wlogin;
                    wlogin->login(StMode::GetStByPassword); // Submit the operation type to WloginHelper for the next step.
                    //SPDLOG_INFO("WloginHelper Task already run.");
                });
                this->client->connect(addr); // Blockage occurs, do not write code later
            }
        } // if(state == OicqState::Online)
    }
}
