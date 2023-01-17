#ifndef NATIVEQQ_ECDH_H
#define NATIVEQQ_ECDH_H

#include <iostream>
#include <memory>
#include <util/tc_common.h>


namespace oicq {
    /**
     * Ecdh is not fully realized for the time being, which is of little significance
     */
    struct EcdhCrypt {
        int ecdh_version = 2;

        std::shared_ptr<char[]> public_key = std::shared_ptr<char[]>(new char[] {
            4 ,0 ,75 ,49 ,111 ,100 ,-23 ,-86 ,-33 ,37 ,-124 ,90 ,22 ,-57 ,127 ,122 ,-44 ,61 ,50 ,42 ,-123 ,-22 ,-82 ,-98 ,113 ,-98 ,-76 ,123 ,0 ,11 ,77 ,39 ,11 ,46 ,-103 ,79 ,8 ,118 ,83 ,96 ,-117 ,-51 ,-92 ,49 ,-51 ,-17 ,5 ,-28 ,52 ,-70 ,77 ,49 ,-30 ,83 ,-21 ,38 ,-19 ,63 ,-92 ,82 ,-103 ,65 ,84 ,-8 ,107
        });
        unsigned int public_key_size = 65;

        std::shared_ptr<char[]> share_key = std::shared_ptr<char[]>(new char[] {
            126 ,85 ,-91 ,51 ,12 ,-70 ,-83 ,-6 ,47 ,-108 ,-13 ,-68 ,0 ,4 ,-11 ,-124
        });
    };
}

#endif //NATIVEQQ_ECDH_H
