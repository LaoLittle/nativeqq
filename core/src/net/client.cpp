#include "net/client.h"

#include <string>

using namespace uvw;

namespace oicq {
    OicqClient::OicqClient(std::shared_ptr<Loop> loop) {
        auto tcp = loop->resource<TCPHandle>();
        // loop->stop() [As soon as possible] End the current cycle, and you can continue to open it next time
        // loop->close() Release all resources in the loop
        this->myLoop = loop;
        this->client = tcp;
    }

    OicqClient::~OicqClient()  {
        this->close();
    }

    void OicqClient::init() {
        client->noDelay(true);
        client->keepAlive(true, TCPHandle::Time{128});
        client->simultaneousAccepts();

        // TODO(waiting for a heartbeat)
        _timeout_timer = myLoop->resource<TimerHandle>();
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

    void OicqClient::close() {
        if (isActive()) {
            // Preferred solution: close all handle resources, and after closing all delay handles, the program will exit
            // Alternative solution: release loop(close) directly, and the program exits
            this->client->clear();
            this->client->close();
        } // if (isActive())
    }
}
