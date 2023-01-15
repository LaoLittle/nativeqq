#ifndef NATIVEQQ_USERDATA_H
#define NATIVEQQ_USERDATA_H

#include <iostream>
#include <vector>

namespace oicq {
    struct SimpleUserInfo {
        std::string nick;
        int age;
    };

    struct UserData {
        std::string name;
        long uin;
        std::vector<char> pwdMd5;

        SimpleUserInfo simpleUserInfo;
    };
}

#endif //NATIVEQQ_USERDATA_H
