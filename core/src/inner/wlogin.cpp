
#include "oicq/protocol.h"
#include "inner/wtlogin.h"
#include <net/packet.h>

#include "spdlog/spdlog.h"

namespace oicq {
    WloginHelper::WloginHelper(Oicq* _oicq): oicq(_oicq), client(oicq->getClient()) {
        SPDLOG_INFO("Start the login process.");
    }

    void WloginHelper::run(oicq::StMode mode) {
        // Here is a new thread！
        //SPDLOG_INFO("Thread(WloginHelper::login) {}", uv_thread_self());
        SPDLOG_INFO("Login service is enabled, mode: {}", (uint8_t) mode);
        if (mode == StMode::GetStByPassword) {
            auto to = std::make_unique<ToService>();
            to->type = PacketType::LoginPacket;
            to->cmd = "wtlogin.login";
            oicq->sendPacket(std::move(to));
        }
    }

    void WloginHelper::login(oicq::StMode mode) {
        auto loop = oicq->getLoop();
        auto req = loop->resource<uvw::WorkReq>([this, mode]() {
            this->run(mode);
        });
        req->queue();
    }
}
