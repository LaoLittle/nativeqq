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
        OicqClient() {
#ifdef USE_NEW_LOOP
            auto loop = Loop::getDefault();
#else
            auto loop = Loop::create();
#endif
            auto tcp = loop->resource<TCPHandle>();

            // loop->stop() 【尽快】结束当前循环，下一次可以继续开启
            // loop->close() 释放该循环内所有资源

            this->myLoop = loop;
            this->tcpHandle = tcp;
        }

        ~OicqClient() {
            // 优选方案：将所有handle资源关闭，关闭所有延时handle后，程序即退出
            // 备选方案：直接释放loop(close)，程序退出
            myLoop->walk([](auto &&h){
                if (!h.closing()) {
                    h.close();
                }
            });
        }
    protected:
        std::shared_ptr<Loop> myLoop;
        std::shared_ptr<TCPHandle> tcpHandle;

        /**
         * 初始化一些uvw在OICQ的配置
         */
        void init() const {
            myLoop->configure(uvw::Loop::Configure::IDLE_TIME, std::chrono::seconds(2700));
            myLoop->idleTime() = std::chrono::seconds(2700); // 默认idle时间间隔
            auto idleHandle = myLoop->resource<uvw::IdleHandle>();
            idleHandle->on<uvw::IdleEvent>([this](const auto&, auto &idle_handle) {
                printf("idle\n");
            });
            idleHandle->start();
        }

        void runLoop() const {
            myLoop->run();
        }

    public:
        /**
         * 发起链接
         */
        void connect(const std::string& host, int port) const {
            init();
            tcpHandle->connect(host, port);
            runLoop();
        }

        void connect(Addr& addr) const {
            init();
            tcpHandle->connect(addr);
            runLoop();
        }

        bool isActive() {
            return tcpHandle->active() && !tcpHandle->closing();
        }

        std::shared_ptr<TCPHandle>& getTcpHandle() {
            // 为接下来奇怪的操作提供可操作性
            return tcpHandle;
        }

        void setIdleTime(int time) const {
            myLoop->idleTime() = std::chrono::seconds(time);
        }
    };
}

#endif //NATIVEQQ_CLIENT_H
