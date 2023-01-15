#ifndef NATIVEQQ_WTLOGIN_H
#define NATIVEQQ_WTLOGIN_H

#include "oicq/oicq.h"

namespace oicq {
    class WloginHelper {
        Oicq* oicq;
        std::shared_ptr<OicqClient> client;

        void run();
    public:
        WloginHelper(Oicq* _oicq);

        void login();
    };
}


#endif //NATIVEQQ_WTLOGIN_H
