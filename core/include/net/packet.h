#ifndef NATIVEQQ_PACKET_H
#define NATIVEQQ_PACKET_H

#include <iostream>
#include <string>
#include "oicq/protocol.h"

namespace oicq {
    enum class PacketType: std::int8_t {
        Non,
        Msf,
        Heartbeat,
        LoginPacket,
        FetchQRCode,
        ExchangeSt,
        ExchangeSig,
        ServicePacket,
        SvcRegister,
    };

    class ToService {
    public:
        /**
         * Basic data
         */
        PacketType type = PacketType::Non;
        std::string cmd;
        int seq = 0; // No need to fill in and provided by OICQ
        std::unique_ptr<char[]> data;
        unsigned int dataLen = 0;

        /**
         * Secondary data
         */
        std::shared_ptr<char[]> firstToken;
        unsigned int firstTokenLen = 0;
        std::shared_ptr<char[]> secondToken;
        unsigned int secondTokenLen = 0;
    };

    class FromService {
    public:
        std::string cmd;
        int seq;
        std::unique_ptr<char[]> data;
        unsigned int dataLen;
    };
}

#endif //NATIVEQQ_PACKET_H
