
#include <oicq/protocol.h>
#include <inner/wtlogin.h>
#include <inner/wtlogin/request.h>
#include <net/packet.h>

#include "spdlog/spdlog.h"

namespace oicq {
    WloginHelper::WloginHelper(Oicq* _oicq): oicq(_oicq), client(oicq->getClient()) {
        SPDLOG_INFO("Start the login process.");
    }

    void initAIO(Oicq *oicq, const shared_ptr<OicqClient>& client) {
        // Initialize various handlers
        auto handle = client->getTcpHandle();
        handle->on<uvw::ConnectEvent>([&](const auto&, auto&) {
            // TODO(Disconnection reconnection event)

        });
        handle->on<uvw::ErrorEvent>([](const auto&, auto&) {
            SPDLOG_ERROR("OicqClient occur serious error, but not crash, and recommend to restart.");
        });
        handle->on<uvw::DataEvent>([](const auto& data, auto&) {
            SPDLOG_DEBUG("Received data from server");
            if (data.length >= 4) {

            }
        });
    }

    void WloginHelper::run(oicq::StMode mode) {
        // Here is a new thread！
        //SPDLOG_INFO("Thread(WloginHelper::login) {}", uv_thread_self());
        SPDLOG_INFO("Login service is enabled, mode: {}", (uint8_t) mode);
        if (mode == StMode::GetStByPassword) {

        } // mode == StMode::GetStByPassword
    }

    void WloginHelper::login(oicq::StMode mode) {
        auto loop = oicq->getLoop();
        auto req = loop->resource<uvw::WorkReq>([this, mode]() {
            initAIO(this->oicq, this->client);
            this->run(mode);
        });
        req->queue();
    }
}
