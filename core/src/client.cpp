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
        _interval_timer = myLoop->resource<uvw::TimerHandle>();
        _interval_timer->on<uvw::TimerEvent>([this](const auto &, auto &) {
            _timeout_timer = myLoop->resource<uvw::TimerHandle>();
            _timeout_timer->on<uvw::TimerEvent>([this](const auto &, auto &) {
                // 心跳超时异常

                if (_internal_timer) {
                    _internal_timer->stop();
                }
                _interval_timer->again();
            });

            _internal_timer = myLoop->resource<uvw::TimerHandle>();
            _internal_timer->on<uvw::TimerEvent>([this](const auto &, auto &) {
                if (
                        // _internal_sequence < static_cast<uint8_t>(_config.packets_per_test)
                        true // 判断是否满足心跳包条件
                        ) {
                    _timeout_timer->stop();
                    _timeout_timer->start(uvw::TimerHandle::Time{2700 * 1000 + 3 * 1000}, uvw::TimerHandle::Time{0});
                    // 重新计算超时，因为这次没有超时
                    // _send_icmp_v4(_internal_sequence);
                    // 发生心跳包
                }
            });

            _timeout_timer->start(uvw::TimerHandle::Time{2700 * 1000 + 3 * 1000}, uvw::TimerHandle::Time{0});
            _internal_timer->start(uvw::TimerHandle::Time{2700 * 1000}, uvw::TimerHandle::Time{2700 * 1000});
        });
        _interval_timer->start(uvw::TimerHandle::Time{0}, uvw::TimerHandle::Time{2700 * 1000});
    }
}
