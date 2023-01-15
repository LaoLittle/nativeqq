#ifndef NATIVEQQ_CLIENT_H
#define NATIVEQQ_CLIENT_H

#include "functional"
#include "uvw.hpp"

#define USE_NEW_LOOP false

namespace oicq {
    class OicqClient {
    public:
        OicqClient(std::shared_ptr<uvw::Loop> loop);

        ~OicqClient();
    protected:
        std::shared_ptr<uvw::Loop> myLoop;
        std::shared_ptr<uvw::TCPHandle> client;

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

        void connect(uvw::Addr& addr);

        bool isActive() {
            return client && client->active() && !client->closing();
        }

        std::shared_ptr<uvw::TCPHandle> getTcpHandle() {
            // 为接下来奇怪的操作提供可操作性
            return client;
        }

        void close();
    };
}

#endif //NATIVEQQ_CLIENT_H
