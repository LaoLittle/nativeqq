#ifndef NATIVEQQ_CLIENT_H
#define NATIVEQQ_CLIENT_H

#include "functional"
#include "uvw.hpp"

using namespace uvw;

namespace oicq {
    class OicqClient {
    public:
        OicqClient() {
            printf("0000\n");
            auto loop = Loop::getDefault();
            auto tcp = loop->resource<TCPHandle>();
            tcp->on<ConnectEvent>([](auto&, auto&){
                printf("1111\n");
            });
            tcp->on<ConnectEvent>([](auto&, auto&){
                printf("2222\n");
            });
            tcpHandle = tcp;
        }

        ~OicqClient() {
            // tcpHandle->stop(); 不需要显示关闭

        }

    public:
        /**
         * 发起链接
         */
        void connect(const std::string& host, int port) const;
        void connect(Addr& addr) const;
    public:
        /**
         * 是否连接了服务器
         */
        bool isConnected = false;

    private:
        // 服务器连接事件器
        std::function<void()> _connect_event;

    protected:
        std::shared_ptr<TCPHandle> tcpHandle;
    };
}

#endif //NATIVEQQ_CLIENT_H
