#include "net/client.h"

namespace oicq {
    OicqClient::OicqClient() {
#ifdef USE_NEW_LOOP
        auto loop = Loop::getDefault();
#else
        auto loop = Loop::create();
#endif
        auto tcp = loop->resource<TCPHandle>();

        // loop->stop() 【尽快】结束当前循环，下一次可以继续开启
        // loop->close() 释放该循环内所有资源

        this->myLoop = loop;
        this->client = tcp;
    }

    OicqClient::~OicqClient()  {
        // 优选方案：将所有handle资源关闭，关闭所有延时handle后，程序即退出
        // 备选方案：直接释放loop(close)，程序退出
        this->client->clear();
        this->client->close();
        myLoop->walk([](auto &&h){
            if (!h.closing()) {
                h.close();
            }
        });
    }

    void OicqClient::init() {
        _timeout_timer = myLoop->resource<uvw::TimerHandle>();
        _timeout_timer->on<uvw::TimerEvent>([this](const auto &, auto &) {
            printf("Timeout timer triggered.\n");
            if (!heartbeat_state) {
                printf("Timeout YES...\n");
                if (_internal_timer && !_internal_timer->active()) {
                    _internal_timer->stop();
                }
            }
        });

        _internal_timer = myLoop->resource<uvw::TimerHandle>();
        _internal_timer->on<uvw::TimerEvent>([this](const auto &, auto &) {
            if (heartbeat_state)
                heartbeat_state = false;
            // Unknown what to do?
            printf("Heartbeat timer trigger: %d\n", heartbeat_state);
            if (!heartbeat_state) {
                if (_timeout_timer && !_timeout_timer->active())
                    _timeout_timer->stop();
                _timeout_timer->start(uvw::TimerHandle::Time{5 * 1000}, uvw::TimerHandle::Time{0});
                // send_heartbeat(uin...);
                // At the receiving office, heartbeat received will be set to True...
            }
        });
        _internal_timer->start(uvw::TimerHandle::Time{270 * 1000}, uvw::TimerHandle::Time{270 * 1000});
    }

    void OicqClient::connect(Addr &addr) {
        init();
        client->connect(addr);
        runLoop();
    }

    void OicqClient::connect(const std::string &host, int port) {
        init();
        client->connect(host, port);
        runLoop();
    }
}
