#ifndef NATIVEQQ_OICQ_UTIL_H
#define NATIVEQQ_OICQ_UTIL_H

#include "iostream"
#include "uvw.hpp"
#include "util/tc_tea.h"
#include "util/tc_common.h"
#include "httplib.h"

#include "tup/tup.h"
#include "oicq/wlogin_sdk/HttpServerList.h"

using namespace tars;

namespace oicq {
    class OICQ_Common {
    public:
        /**
         * 获取OICQ服务器地址
         * @param addr
         * @param uin
         * @param androidId
         * @param appId
         * @param imsi
         * @param cid
         * @param isWifi
         * @param lastTime
         * @return http请求错误码
         */
         static int get_oicq_address(uvw::Addr *addr, long uin, std::string androidId, long appId, std::string imsi, long cid = 0, bool isWifi = false, long lastTime = 6000) {
            HttpServerListReq httpServerListReq;
            httpServerListReq.uin = uin;
            httpServerListReq.linkType = 1;
            httpServerListReq.imsi = imsi;
            httpServerListReq.appId = appId;
            httpServerListReq.lastTime = lastTime / 1000;
            httpServerListReq.imei = androidId;
            if (cid >= 0) {
                httpServerListReq.cellid = cid;
            } else {
                httpServerListReq.cellid = 0;
            }
            // what is cellid? https://blog.xinrao.co/index.php/archives/12/
            // you get it by gsm, by the way need location permission
            if (isWifi) {
                httpServerListReq.netType = 100;
            } else {
                httpServerListReq.netType = 1;
            }
            tup::UniPacket req_uniPacket;
            req_uniPacket.setRequestId(0);
            req_uniPacket.setServantName("HttpServerListReq");
            req_uniPacket.setFuncName("HttpServerListReq");
            req_uniPacket.put("HttpServerListReq", httpServerListReq);
            const char* tea_key = new char[16]{-16, 68, 31, 95, -12, 45, -91, -113, -36, -9, -108, -102, -70, 98, -44, 17};
            vector<char> data;
            req_uniPacket.encode(data);
            auto encrypt = TC_Tea::encrypt(tea_key, data.data(), data.size());
            httplib::Client cli("http://configsvr.msf.3g.qq.com");
            auto res = cli.Post("/configsvr/serverlist.jsp?mType=getssolist", encrypt.data(), encrypt.size(), "jce");
            int status = res->status;
            if(status == 200) {
                auto body = res->body;
                auto body_length = res->get_header_value<size_t>("Content-Length");
                auto decrypt = TC_Tea::decrypt(tea_key, body.data(), body_length);
                tup::UniPacket resp_uniPacket;
                HttpServerListResp resp;
                resp_uniPacket.decode(decrypt.data(), decrypt.size());
                resp_uniPacket.get("HttpServerListRes", resp);
                if(!resp.v2g3gList.empty()) {
                    auto server = resp.v2g3gList.front();
                    (*addr).ip = server.ip;
                    (*addr).port = server.port;
                } else if (!resp.wifiList.empty()){
                    auto server = resp.wifiList.front();
                    (*addr).ip = server.ip;
                    (*addr).port = server.port;
                } else {
                    (*addr).ip = "msfwifi.3g.qq.com";
                    (*addr).port = 8080;
                }
            }
            delete []tea_key;
            return status;
        }

    };
}

#endif //NATIVEQQ_OICQ_UTIL_H
