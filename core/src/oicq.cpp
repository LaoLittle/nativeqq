#include "oicq/oicq.h"
#include "oicq/util/oicq_util.h"

void oicq::Oicq::getStByPwd(long uin, std::string pwd) {
    SPDLOG_INFO("Start pre login!");
    if (!this->protocol) {
        SPDLOG_ERROR("No specific protocol is set.");
        return;
    }
    if (!client.isActive()) {
        spdlog::info("Connecting to server!");
        uvw::Addr addr;
        if (oicq::OICQ_Common::get_oicq_address(&addr, uin, "867109044454081", SUB_APPID, "") == 200) {
            client.connect(addr); // 发生堵塞 后面不要写代码
        }
    }


}