
#include "oicq/protocol.h"
#include "inner/wtlogin.h"

#include <uvw/thread.h>
#include "spdlog/spdlog.h"

namespace oicq {
    WloginHelper::WloginHelper(Oicq* _oicq): oicq(_oicq), client(oicq->getClient()) {
        SPDLOG_INFO("Start the login process.");
    }

//    WloginHelper::~WloginHelper() {
//        SPDLOG_INFO("WloginHelper ~~~~~~!");
//    }

    void WloginHelper::run(oicq::StMode mode) {
        // Here is a new thread！
        //SPDLOG_INFO("Thread(WloginHelper::login) {}", uv_thread_self());
        uv_sleep(5000);
        SPDLOG_INFO("Login service is enabled, mode: {}", (uint8_t) mode);
        if (mode == StMode::GetStByPassword) {

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
