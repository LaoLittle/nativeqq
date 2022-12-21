#include "oicq/oicq.h"
#include "oicq/util/oicq_util.h"

void oicq::Oicq::getStByPwd(long uin, const char* pwdMd5) {
    if (!client.isActive()) {
        uvw::Addr addr;
        if (oicq::OICQ_Common::get_oicq_address(&addr, 10001, "867109044454081", SUB_APPID) == 200) {
            client.connect(addr); // 发生堵塞 后面不要写代码
        }
    }

}