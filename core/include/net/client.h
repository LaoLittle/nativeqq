#ifndef NATIVEQQ_CLIENT_H
#define NATIVEQQ_CLIENT_H

#include "functional"
#include "uvw.hpp"
#include "uvw/idle.h"

#define USE_NEW_LOOP false

using namespace uvw;

namespace oicq {
    class OicqClient {
    public:
        OicqClient();

        ~OicqClient();

    protected:
        std::shared_ptr<Loop> myLoop;
        std::shared_ptr<TCPHandle> client;

        bool heartbeat_state = false;
        std::shared_ptr<uvw::TimerHandle> _internal_timer;
        std::shared_ptr<uvw::TimerHandle> _timeout_timer;

        /**
         * 初始化一些uvw在OICQ的配置
         */
        void init();

        void runLoop() const {
            myLoop->run();
        }

    public:
        /**
         * 发起链接
         */
        void connect(const std::string& host, int port);

        void connect(Addr& addr);

        bool isActive() {
            return client && client->active() && !client->closing();
        }

        std::shared_ptr<TCPHandle>& getTcpHandle() {
            // 为接下来奇怪的操作提供可操作性
            return client;
        }
    };
}

#endif //NATIVEQQ_CLIENT_H
