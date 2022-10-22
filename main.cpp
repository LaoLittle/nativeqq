#include <iostream>
#include "util/tc_tea.h"
#include "util/tc_common.h"
#include "util/tc_pack.h"
#include "tup/tup.h"
#include "tup/Protobuf.h"
#include "oicq/oicq.h"
#include "uvw.hpp"
#include "oicq/util/oicq_util.h"

using namespace tars;

int main() {
    try {
        oicq::Oicq oicq;
        uvw::Addr addr;
        if (oicq::OICQ_Common::get_oicq_address(&addr, 10001, "867109044454081", 0x200302d5) == 200) {
            printf("oicq ip: %s, port: %d\n", addr.ip.c_str(), addr.port);
        }
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}