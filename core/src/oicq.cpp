#include <string>

#include "oicq/oicq.h"
#include "oicq/util/oicq_util.h"
#include "util/tc_md5.h"
#include <util/tc_pack.h>
#include <util/tc_common.h>
#include <util/tc_tea.h>

#include "spdlog/spdlog.h"

using namespace uvw;
using namespace tars;

namespace oicq {
    Oicq::Oicq() {
#ifdef USE_NEW_LOOP
        // Obedient, use this!
        auto loop = Loop::getDefault();
#else
        // Created for what?
        // How many QQ do you want to keep?
        // Badass! ! ! !
        auto loop = Loop::create();
#endif
        this->defaultLoop = loop;
        this->packetHandler = std::make_shared<PacketHandler>(loop);
    }

    Oicq::~Oicq() {
        this->defaultLoop->walk([](auto &&h){
            if (!h.closing()) {
                h.close();
            }
        });
    }

    void Oicq::getStByPwd(unsigned int uin, std::string pwd) {
        SPDLOG_INFO("Start pre login!");
        if (this->protocolType == ProtocolType::Non) {
            // How can it run without setting the protocol type?
            SPDLOG_ERROR("No specific protocol is set.");
            return;
        } else if(this->protocolType != this->device->deviceType) {
            // The protocol type is different from the device type.
            // How do you run your 'IOSQQ' in the Android environment?
            SPDLOG_ERROR("Device type and Protocol type do not match.");
            return;
        } else if(this->state == OicqState::Online) {
            // If you are online, don't log in, okay!
            SPDLOG_ERROR("Already online, repeated login is prohibited.");
            return;
        } else {
            this->client = std::make_shared<OicqClient>(
                    this->defaultLoop
            ); // By creating a smart pointer, the previous client will be automatically released (when reference == 0)
            uvw::Addr addr;
            if (oicq::OICQ_Common::get_oicq_address(&addr, uin, this->device->deviceId, this->protocol->subAppId, "") == 200) {
                auto handle = this->client->getTcpHandle();
                // SPDLOG_INFO("Thread(Oicq::getStByPwd) {}", uv_thread_self()); // Only to test which the thread is.
                {
                    auto pwdMd5 = tars::TC_MD5::md5bin(pwd);
                    this->userData.uin = uin;
                    this->userData.name = to_string(uin);
                    this->userData.pwdMd5 = pwdMd5; // Will this be a problem?
                    tars::TC_PackIn tmp;
                    tmp.write(pwdMd5.data(), 16);
                    tmp << 0;
                    tmp << (unsigned int) uin;
                    this->userData.userMd5 = tars::TC_MD5::md5bin(tmp.topacket());
                } // init userdata
                handle->once<ConnectEvent>([&, this](const ConnectEvent &, TCPHandle &handle) {
                    SPDLOG_INFO("Successfully connected to the server({}:{}).", addr.ip, addr.port); // Why is it a yellow warning?
                    auto wlogin = std::make_shared<oicq::WloginHelper>(this);
                    this->wtloginHelper = wlogin;
                    wlogin->login(StMode::GetStByPassword); // Submit the operation type to WloginHelper for the next step.
                    //SPDLOG_INFO("WloginHelper Task already run.");
                });
                this->client->connect(addr); // Blockage occurs, do not write code later
            }
        } // if(state == OicqState::Online)
    }

    int getFlag1ByPacketType(PacketType type); // Get a FLAG of package
    char getFlag2ByPacketType(PacketType type);

    /**
     * 不建议阅读该区域代码！！！
     * Reading this area code is not recommended! ! !
     */
    void Oicq::sendPacket(std::unique_ptr<oicq::ToService> toService) {
        if (toService->seq == 0) {
            toService->seq = userData.session.nextSeq();
        }
        if (toService->type == PacketType::Non) {
            if (state == OicqState::Online) {
                toService->type = PacketType::ServicePacket;
            } else {
                toService->type = PacketType::LoginPacket;
            }
        }
        TC_PackIn packIn;
        packIn.writeBlockWithIntLen([&](TC_PackIn& packer){
            packer << getFlag1ByPacketType(toService->type);
            packer << getFlag2ByPacketType(toService->type);
            {
                if (toService->type == PacketType::ServicePacket ||
                    toService->type == PacketType::ExchangeSig) {
                    packer << toService->seq;
                } else {
                    if (toService->firstTokenLen == 0) {
                        packer << 4;
                    } else {
                        auto token = toService->firstToken;
                        auto len = toService->firstTokenLen;
                        packer << (int) (len + 4);
                        packer.write(token.get(), len);
                    }
                }
            }
            packer << (char)(0);
            {
                auto uin = to_string(this->userData.uin);
                packer << (int) (uin.length() + 4);
                packer.write(uin.c_str(), uin.length());
            }

            TC_PackIn data;
            data.writeBlockWithIntLen([&](TC_PackIn& dataPacker){
                switch (toService->type) {
                    case PacketType::ServicePacket:
                    case PacketType::ExchangeSig: {
                        assert(toService->cmd.length() != 0);
                        dataPacker << (int) (4 + toService->cmd.length());
                        dataPacker << toService->cmd;
                        dataPacker << 4 + 4;
                        dataPacker.write(userData.session.getSession().get(), 4);
                        break;
                    }
                    case PacketType::Heartbeat:
                    case PacketType::LoginPacket:
                    case PacketType::SvcRegister:
                    case PacketType::FetchQRCode:
                    case PacketType::ExchangeSt: {
                        dataPacker << toService->seq;
                        dataPacker << (unsigned int) (this->protocol->subAppId);
                        dataPacker << (unsigned int) (this->protocol->subAppId);
                        auto subNet = TC_Common::swap_int32(device->getSubNetType());
                        dataPacker << subNet;
                        dataPacker << 0;
                        dataPacker << 0x300;
                        {
                            auto len = toService->secondTokenLen;
                            dataPacker << (int) (len + 4);
                            if (len != 0) {
                                auto token = toService->secondToken;
                                dataPacker.write(token.get(), len);
                            }
                        } // secondTokenLen
                        {
                            assert(toService->cmd.length() != 0);
                            dataPacker << (int) (4 + toService->cmd.length());
                            dataPacker << toService->cmd;
                            dataPacker << 4 + 4;
                            dataPacker.write(userData.session.getSession().get(), 4);
                        } // cmd & session
                        {
                            auto& deviceId = device->getDeviceId();
                            auto len = deviceId.length();
                            dataPacker << len + 4;
                            if (len != 0) {
                                dataPacker.write(deviceId.c_str(), len);
                            }
                        } // deviceId
                        {
                            auto& ksid = userData.session.ksid;
                            dataPacker << ksid.size + 4;
                            if (!ksid.empty()) {
                                dataPacker.write(ksid.ticket.get(), ksid.size);
                            }
                        } // ksid
                        {
                            std::string& detail = protocol->detail;
                            auto len = detail.length();
                            assert(len != 0);
                            dataPacker << (short) (len + 2);
                            if (len != 0) {
                                dataPacker.write(detail.c_str(), len);
                            }
                        } // detail
                        {
                            std::string& qimei = device->qimei;
                            auto len = qimei.length();
                            dataPacker << len + 4;
                            if (len != 0) {
                                dataPacker.write(qimei.c_str(), len);
                            }
                        } // qimei
                        break;
                    }
                    default: assert(false);
                }
            }, 4); // head
            {
                auto len = toService->dataLen;
                data << (int) (len + 4);
                if (len != 0) {
                    data.write(toService->data.get(), len);
                }
            } // data(body)

            if (toService->type != PacketType::Heartbeat) {
                auto dataBytes = data.topacket();
                auto teaKey = userData.getTeaKey(toService->type != PacketType::ServicePacket &&
                                                 toService->type != PacketType::SvcRegister); // Not a service package nor a registration package.
                auto encrypt = TC_Tea::encrypt(teaKey.get(), dataBytes.c_str(), data.length());
                packer.write(encrypt.data(), encrypt.size());
            } else {
                packer << data;
            }
        }, 4);

        int result = client->getTcpHandle()->tryWrite((char *)packIn.topacket().c_str(), packIn.length());
        SPDLOG_INFO("Send ToService(result={}, uin={}, cmd={}, seq={}, len={})",
                    result, userData.uin, toService->cmd, toService->seq, packIn.length());
    }

    int getFlag1ByPacketType(PacketType type) {
        if (type == PacketType::ServicePacket) {
            return 0xB;
        }
        switch (type) {
            case PacketType::Heartbeat:
            case PacketType::SvcRegister:
            case PacketType::ExchangeSt:
            case PacketType::LoginPacket: {
                return 0xA;
            }
            case PacketType::FetchQRCode: {
                return 0x8;
            }
            case PacketType::ExchangeSig: {
                return 0xB;
            }
            case PacketType::Msf: {
                return 0;
            }
            default:
                assert(false);
        }
    }

    char getFlag2ByPacketType(PacketType type) {
        if(type == PacketType::ServicePacket) {
            return 0x1;
        } // Don't mess with the sorting, here is the sorting with branch prediction.
        switch (type) {
            case PacketType::SvcRegister: {
                return 0x1;
            }
            case PacketType::LoginPacket:
            case PacketType::FetchQRCode:
            case PacketType::ExchangeSt:
            case PacketType::ExchangeSig: {
                return 0x2;
            }
            case PacketType::Heartbeat:
            case PacketType::Msf: {
                return 0;
            }
            default:
                assert(false);
        }
    }
}
