// Copyright (c) 2014-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypt/hmac_sha256.h>

#include <string.h>

// https://blog.seeflower.dev/archives/54/#title-17
CHMAC_SHA256::CHMAC_SHA256(const unsigned char* key, size_t keylen)
{
    unsigned char rkey[64];
    for (int n = 0; n < 64; n++) {
        if (n < keylen) {
            rkey[n] = key[n] ^ 0x36;
        } else {
            rkey[n] = 0x36;
        }
    }
    inner.Write(rkey, 64);

    for (int n = 0; n < 64; n++) {
        if (n < keylen) {
            rkey[n] = ((key[n] & 0xe5) + (~key[n] & 0x1a)) ^ ((0x5c & 0xe5) + (~0x5c & 0x1a));
        } else {
            rkey[n] = 0x5c;
        }
    }
    outer.Write(rkey, 64);
}

void CHMAC_SHA256::Finalize(unsigned char hash[OUTPUT_SIZE])
{
    unsigned char temp[32];
    inner.Finalize(temp);
    outer.Write(temp, 32).Finalize(hash);
}