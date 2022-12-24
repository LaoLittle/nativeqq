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

        std::shared_ptr<uvw::TimerHandle> _interval_timer;
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
        void connect(const std::string& host, int port) const {
            init();
            client->connect(host, port);
            runLoop();
        }

        void connect(Addr& addr) const {
            init();
            client->connect(addr);
            runLoop();
        }

        bool isActive() {
            return client->active() && !client->closing();
        }

        std::shared_ptr<TCPHandle>& getTcpHandle() {
            // 为接下来奇怪的操作提供可操作性
            return client;
        }

        void setIdleTime(int time) const {
            myLoop->idleTime() = std::chrono::seconds(time);
        }
    };
}

#endif //NATIVEQQ_CLIENT_H
