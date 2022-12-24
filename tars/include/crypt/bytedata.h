#ifndef NATIVEQQ_BYTEDATA_H
#define NATIVEQQ_BYTEDATA_H

#include <stddef.h>
#include <stdint.h>
#include "util/tc_common.h"
#include "util/tc_md5.h"
#include "crypt/hmac_sha256.h"
#include "crypt/salsa20.h"
#include "crypt/crc32.h"

#include <sys/time.h>

namespace ByteData {
    vector<char> _gen_key(const char* seed) {
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

        vector<char> buffer(str.begin(), str.end());
        return buffer;
    }

    void _init_state(const vector<char>& key, int state[256]) {
        for(int i = 0;i < 256;i++)
            state[i] = i;
        //char _key[256];
        //for(int i = 0;i < 256;i++)
        //    _key[i] = key[i % key.size()];
        int index = 0;
        for(int i = 0;i < 256;i++) {
            //index = (index + _key[i] + state[i]) % 256;
            index = (index + key[i % key.size()] + state[i]) % 256;
            auto tmp = state[i];
            state[i] = state[index];
            state[index] = tmp;
        }
    }

    vector<char> _encrypt_data(const vector<char>& key, const std::string &data, size_t dataLen) {
        int state[256];
        _init_state(key, state);
        int i1 = 0, i2 = 0;
        char _ori_data[dataLen];
        for(int i = 0;i < dataLen;i++) {
            i1 = (i1 + 1) % 256;
            i2 = (i2 + state[i1]) % 256;
            auto tmp = state[i1];
            state[i1] = state[i2];
            state[i2] = tmp;
            auto key_index = (state[i2] + state[i1]) & 0xff;
            _ori_data[i] = (char) ((data[i] & ~state[key_index]) | (state[key_index] & ~data[i]));
        }
        vector<char> buffer(_ori_data, _ori_data + dataLen);
        return buffer;
    }

    void _pack(tars::TC_PackIn *in, int16_t ver, char *data, size_t len) {
        *in << ver;
        *in << ((int16_t)len);
        (*in).write(data, len);
    }

    vector<char> getByte(char* data, size_t dataLen) {
        auto seed = tars::TC_Common::randomStrGen(10);
        auto key = _gen_key(seed.c_str());
        //char _key[256];
        //for(int i = 0;i < 256;i++)
        //    _key[i] = key[i % key.size()];

        int64_t time = tars::TC_Common::now2ms();
        unsigned char time_data[8];
        tars::TC_Common::int64_to_bytes(time_data, time);

        auto content = (tars::TC_PackIn()
                .write(data, dataLen)
                .write(reinterpret_cast<const char *>(time_data), 8))
                        .getBuffer();

        unsigned char p10[32];
        CHMAC_SHA256((unsigned char*)(key.data()), key.size())
                .Write((unsigned char*)(content.data()), content.size())
                .Finalize(p10);
        auto p11 = _encrypt_data(key, content, content.size());

        uint8_t n[8] = { 0x9a, 0xd4, 0x7e, 0x38, 0x90, 0xa5, 0xeb, 0x50};
        uint8_t p12[content.length()];
        if(s20_crypt((uint8_t *)(key.data()), S20_KEYLEN_256, n, 0, p12, content.length()) == S20_SUCCESS) {
            for(int i = 0;i < content.size();i++) {
                p12[i] = data[i] ^ p12[i];
            }
        }


        auto outer = tars::TC_PackIn();
        outer.write("heha", 4);
        outer << 0x00000001 << 0x01000000;
        outer << 0 << ((char) 1);
        outer << 0x01000500;
        outer << 0;

        /*
---------jhexdump start---------
dc7b9500  68 65 68 61 00 00 00 01 01 00 00 00 00 00 00 00  heha............
dc7b9510  01 01 00 05 00 00 00 00 00 b6 d9 c2 91 00 00 00  ................
dc7b9520  02 00 00 00 a6 00 01 00 08 00 00 01 85 44 3b 7c  .............D;|
dc7b9530  2b 00 02 00 0a 6a 6a 24 38 74 39 6b 39 3a 65 00  +....jj$8t9k9:e.
dc7b9540  03 00 04 01 00 00 01 00 05 00 04 01 00 00 01 00  ................
dc7b9550  04 00 04 00 00 00 00 00 06 00 04 01 00 00 04 00  ................
dc7b9560  07 00 04 01 00 00 05 00 08 00 04 01 00 00 06 00  ................
dc7b9570  09 00 20 11 d4 31 40 a2 52 86 f8 10 4d b8 94 7b  .. ..1@.R...M..{
dc7b9580  1b ef 83 47 d0 91 37 ac 7c 12 04 44 ac 13 d3 93  ...G..7.|..D....
dc7b9590  04 f2 ab 00 0a 00 10 13 bf c5 05 5b 37 59 24 3c  ...........[7Y$<
dc7b95a0  bc c5 d3 98 53 46 aa 00 0b 00 10 5d 14 de 3d 1b  ....SF.....]..=.
dc7b95b0  dc 75 fa b5 f3 64 a6 fc e8 fc 10 00 0c 00 04 01  .u...d..........
dc7b95c0  00 00 01 00 0d 00 04 00 00 00 00                 ...........
---------jhexdump end---------
         */

        auto inner = tars::TC_PackIn();
        inner << 0x00000002;
        inner << 4+8 + 4+0xa + (8 * (4+4)) + 4+32 + 4+16 + 4+16;
        _pack(&inner, 0x1, (char*) time_data, 0x8);
        _pack(&inner, 0x2, seed.data(), 0xa);
        _pack(&inner, 0x3, (char*) "\u0001\u0000\u0000\u0001", 0x4);
        _pack(&inner, 0x5, (char*) "\u0001\u0000\u0000\u0001", 0x4);
        _pack(&inner, 0x4, (char*) "\u0000\u0000\u0000\u0000", 0x4);
        _pack(&inner, 0x6, (char*) "\u0001\u0000\u0000\u0004", 0x4);
        _pack(&inner, 0x7, (char*) "\u0001\u0000\u0000\u0005", 0x4);
        _pack(&inner, 0x8, (char*) "\u0001\u0000\u0000\u0006", 0x4);
        _pack(&inner, 0x9, (char*) p10, 0x20);
        _pack(&inner, 0xa, tars::TC_MD5::md5bin(p11.data(), p11.size()).data(), 0x10);
        _pack(&inner, 0xb, tars::TC_MD5::md5bin((char *)p12, content.size()).data(), 0x10);
        _pack(&inner, 0xc, (char*) "\u0001\u0000\u0000\u0001", 0x4);
        _pack(&inner, 0xd, (char*) "\u0000\u0000\u0000\u0000", 0x4);

        outer << crc32(inner.getBuffer().c_str(), (int) inner.length());
        outer.write(inner.topacket().c_str(), inner.length());

        auto result = outer.topacket();
        vector<char> buffer(result.begin(), result.end());
        return buffer;
    }
}

#endif //NATIVEQQ_BYTEDATA_H
