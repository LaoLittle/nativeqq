#include "net/client.h"

void oicq::OicqClient::connect(const std::string& host, int port) {
    sDefault->connect(host, port);
}

void oicq::OicqClient::connect(Addr& addr) {
    sDefault->connect(addr);
}