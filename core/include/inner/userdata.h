#ifndef NATIVEQQ_USERDATA_H
#define NATIVEQQ_USERDATA_H

#include <iostream>
#include <vector>
#include <atomic>

#include <inner/ecdh.h>

#include "util/tc_common.h"

namespace oicq {
    struct Ticket {
        std::shared_ptr<char[]> ticket;
        unsigned int size = 0;

        bool empty() {
            return !ticket || size == 0;
        }
    };

    struct SimpleUserInfo {
        std::string nick; // QQ account nick
        int age = 0; // QQ account age(which you set)
    };

    class Session {
        atomic<int> seqFactory; // an auto-incrementing sequence
        std::shared_ptr<char[]> msgSession; // random-4byte

    public:
        EcdhCrypt ecdh;

        std::shared_ptr<char[]> defaultTeaKey;
        std::shared_ptr<char[]> randomKey;

        Ticket ksid;

        Ticket rollbackTicket;
        int rollbackFlag;
    public:
        Session() {
            randomKey = std::make_shared<char[]>(16);
            tars::TC_Common::randomBytes(randomKey.get(), 16);
        }
    public:
        int nextSeq() {
            auto now = seqFactory.load();
            if(now == 0 || now >= 1000000) {
                seqFactory.store(tars::TC_Common::randInt(0, 100000));
            }
            seqFactory.fetch_add(1);
            return seqFactory.load();
        }

        std::shared_ptr<char[]> getSession() {
            if (!msgSession) {
                msgSession = std::make_shared<char[]>(4);
                tars::TC_Common::randomBytes(msgSession.get(), 4);
            }
            return msgSession;
        }
    };

    struct SigInfo {
        Ticket d2;
        std::shared_ptr<char[]> d2Key; // Size is definitely 16

        Ticket wtSessionTicket;
        std::shared_ptr<char[]> wtSessionTicketKey; // Size is definitely 16
    };

    struct UserData {
        // After the mobile phone number login operation is completed, uin will be given
        // name that will be used to log in with the mobile phone number and email address
        std::string name;
        unsigned int uin; // QQ num
        std::vector<char> pwdMd5; // md5 of password

        Session session;
        SigInfo sigInfo;
        SimpleUserInfo simpleUserInfo; // Part of the token and user information returned by login

        std::shared_ptr<char[]> getTeaKey(bool getDefault) {
            if (!session.defaultTeaKey) {
                session.defaultTeaKey = std::make_shared<char[]>(16);
            }
            if (getDefault) {
                return session.defaultTeaKey;
            } else {
                return sigInfo.d2Key;
            }
        }
    };
}

#endif //NATIVEQQ_USERDATA_H
