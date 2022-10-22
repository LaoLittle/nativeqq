#include "net/client.h"

#include "util/tc_common.h"

void oicq::OicqClient::connect(const std::string& host, int port) const {
    tcpHandle->connect(host, port);
    auto loop = Loop::getDefault();
    loop->run();
}

void oicq::OicqClient::connect(Addr& addr) const {
    tcpHandle->connect(addr);
    auto loop = Loop::getDefault();
    loop->run();
}

