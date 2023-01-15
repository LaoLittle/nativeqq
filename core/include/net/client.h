#ifndef NATIVEQQ_CLIENT_H
#define NATIVEQQ_CLIENT_H

#include "functional"
#include "uvw.hpp"

/**
 * Whether to create a new loop for all subsequent oicq-clients
 */
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
         * Initialize some uvw configurations in OICQ
         */
        void init();

        void runLoop() const {
            myLoop->run();
        }

    public:
        /**
         * initiate connect
         */
        void connect(const std::string& host, int port);

        void connect(uvw::Addr& addr);

        bool isActive() {
            return client && client->active() && !client->closing();
        }

        // Provide operability for the next strange operation
        std::shared_ptr<uvw::TCPHandle>& getTcpHandle() {
            return client;
        }

        void close();
    };
}

#endif //NATIVEQQ_CLIENT_H
