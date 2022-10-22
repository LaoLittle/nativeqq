#ifndef NATIVEQQ_CLIENT_H
#define NATIVEQQ_CLIENT_H

#include "uvw.hpp"

using namespace uvw;

namespace oicq {
    class OicqClient {
    public:
        OicqClient() {
            auto loop = Loop::getDefault();
            auto tcp = loop->resource<TCPHandle>();
            sDefault = tcp;
        }

        ~OicqClient() {
            // sDefault->stop(); 不需要显示关闭

        }

    public:
        /**
         * 发起链接
         */
        void connect(const std::string& host, int port);
        void connect(Addr& addr);



    protected:
        std::shared_ptr<TCPHandle> sDefault;
    };
}

#endif //NATIVEQQ_CLIENT_H
