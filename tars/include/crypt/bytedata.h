#ifndef NATIVEQQ_BYTEDATA_H
#define NATIVEQQ_BYTEDATA_H

#include <stddef.h>
#include <stdint.h>
#include "util/tc_common.h"
#include <sys/time.h>


namespace ByteData {
    vector<char> _gen_key(const char* seed = "hhhhhhiiih") {
        std::string data = "(kT7*@a$L?";
        data.insert(0, data.substr(data.size() - 2, 2));
        data = data.substr(0, data.size() - 2);

        std::string data2 = "9+>6X)&O@a";
        data2.insert(0, data2.substr(data2.size() - 2, 2));
        data2 = data2.substr(0, data2.size() - 2);

        std::string rand_seed;
        for (int i = 0; i < 10; i++ ) {
            rand_seed += (char) (seed[i] ^ data2[i]);
        }

        auto str = rand_seed.substr(rand_seed.size() - 2) + data + rand_seed.substr(0, rand_seed.size() - 2);

        vector<char> buffer;
        buffer.resize(str.size());
        buffer.assign(str.begin(), str.end());

        return buffer;
    }

    vector<char> getByte(char* data, size_t dataLen) {
        struct timeval timeval{};
        gettimeofday(&timeval, nullptr);
        long long time_data = (((long long) timeval.tv_usec) / 1000) + (((long long) (timeval.tv_sec)) * 1000);

        auto content = (tars::TC_PackIn().write(data, dataLen) << time_data).getBuffer();
        printf("out: %s\n", TC_Common::bin2str(content).data());

        auto outer = tars::TC_PackIn();
        outer << "heha";
        outer << 1;
        outer << 0x01000000;


    }
}

#endif //NATIVEQQ_BYTEDATA_H
