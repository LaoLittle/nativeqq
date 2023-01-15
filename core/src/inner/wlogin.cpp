
#include "oicq/protocol.h"
#include "inner/wtlogin.h"

#include <uvw/thread.h>

#include "spdlog/spdlog.h"

namespace oicq {
    WloginHelper::WloginHelper(Oicq* _oicq): oicq(_oicq), client(oicq->getClient()) {
        SPDLOG_INFO("Start the login process.");
    }

    void WloginHelper::run() {
        // SPDLOG_INFO("Thread(WloginHelper::login) {}", uv_thread_self());
        
    }

    void WloginHelper::login() {
        // Release UVW's own created thread to avoid strange bugs.
        auto loop = oicq->getLoop();
        auto handle = loop->resource<uvw::Thread>([this](const std::shared_ptr<void>&) {
            this->run();
        });
        handle->run();
    }
}
