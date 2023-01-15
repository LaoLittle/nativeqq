#include "oicq/oicq.h"
#include "oicq/util/oicq_util.h"

using namespace uvw;

namespace oicq {
    Oicq::Oicq() {
#ifdef USE_NEW_LOOP
        auto loop = Loop::getDefault();
#else
        auto loop = Loop::create();
#endif
        defaultLoop = loop;
    }

    Oicq::~Oicq() {
        defaultLoop->walk([](auto &&h){
            if (!h.closing()) {
                h.close();
            }
        });
    }

    void Oicq::getStByPwd(long uin, std::string pwd) {
        SPDLOG_INFO("Start pre login!");
        if (!protocol) {
            SPDLOG_ERROR("No specific protocol is set.");
            return;
        }
        if(state == OicqState::Online) {
            SPDLOG_ERROR("Already online, repeated login is prohibited.");
        } else {
            this->client = std::make_shared<OicqClient>(
                    this->defaultLoop
            ); // By creating a smart pointer, the previous client will be automatically released (the reference is 0)
            uvw::Addr addr;
            if (oicq::OICQ_Common::get_oicq_address(&addr, uin, "867109044454081", SUB_APPID, "") == 200) {
                auto handle = client->getTcpHandle();
                handle->once<ConnectEvent>([](const ConnectEvent &, TCPHandle &handle) {
                    SPDLOG_INFO("Successfully connected to the server.");

                });
                client->connect(addr); // Blockage occurs, do not write code later
            }
        } // if(state == OicqState::Online)
    }
}
