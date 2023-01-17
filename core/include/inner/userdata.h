#ifndef NATIVEQQ_USERDATA_H
#define NATIVEQQ_USERDATA_H

#include <iostream>
#include <vector>
#include <atomic>

#include "util/tc_common.h"

namespace oicq {
    struct SimpleUserInfo {
        std::string nick; // QQ account nick
        int age = 0; // QQ account age(which you set)
    };

    /*
     * Ecdh is not fully realized for the time being, which is of little significance
     */
    struct EcdhCrypt {
        int ecdh_version = 1;

        std::string public_key = tars::TC_Common::str2bin(
                "046fd49cf178a75ec9987070eccd72823cf482df9d4935cc55e3fc59f97999b634e92945c583d61b682a8c83ee221986d26a8ccdd05ae7ed8894d989d0e1beda17"
        );

        std::string share_key = tars::TC_Common::str2bin(
                "be329af588e601571ca3dd22dafb6037"
        );
    };

    class Session {
        atomic<int> seqFactory;
        std::shared_ptr<char[]> msgSession;

    public:
        EcdhCrypt ecdhCrypt;

        std::shared_ptr<char[]> defaultTeaKey;

        std::shared_ptr<char[]> ksid;
        unsigned int ksidLen = 0;

        int nextSeq() {
            auto now = seqFactory.load();
            if(now == 0 || now == 1000000) {
                seqFactory.store(tars::TC_Common::randInt(0, 100000));
            }
            seqFactory.fetch_add(1);
            return seqFactory.load();
        }

        std::shared_ptr<char[]> getSession() {
            if (!msgSession) {
                // I like to write dead, random anyway, I don't care.
                msgSession = std::shared_ptr<char[]>(new char[] {43, 56, 23, 32});
            }
            return msgSession;
        }
    };

    struct SigInfo {
        std::shared_ptr<char[]> d2Key; // Size is definitely 16

    };

    struct UserData {
        // After the mobile phone number login operation is completed, uin will be given
        // name that will be used to log in with the mobile phone number and email address
        std::string name;
        long uin; // QQ num
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
