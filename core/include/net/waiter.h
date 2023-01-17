#ifndef NATIVEQQ_WAITER_H
#define NATIVEQQ_WAITER_H

#include <iostream>
#include <unordered_map>
#include <functional>

#include <uvw/thread.h>

#include <net/packet.h>

namespace oicq {
    class PacketWaiter {

    };

    // Packet receiver, providing asynchronous and synchronous packet receiving
    class PacketHandler {
        /**
         * avoid pit:
         * On Windows,
         * 'tryLock' for a mutex returns true whether it is locked or not
         */
        std::shared_ptr<uvw::Mutex> mutex;
        std::unordered_map<int, std::function<void()>> handlers;

    public:
        PacketHandler(std::shared_ptr<uvw::Loop> loop):
        mutex(loop->resource<uvw::Mutex>()) {

        }

        void initReceiver() {

        }
    public:
        void tryHandle(std::string cmd, int seq);
    };
}

#endif //NATIVEQQ_WAITER_H
