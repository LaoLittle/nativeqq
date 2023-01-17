#ifndef NATIVEQQ_WTLOGIN_H
#define NATIVEQQ_WTLOGIN_H

#include "oicq/oicq.h"
#include "net/client.h"


namespace oicq {
    class Oicq; // advance declaration of existence

    enum class StMode: std::uint8_t {
        GetStByPassword = 1,

    };

    /**
     * To be honest,
     * if you don't understand the agreement,
     * you can't understand it here!
     */
    class WloginHelper {
        oicq::Oicq *oicq;
        std::shared_ptr<oicq::OicqClient> client;

        void run(oicq::StMode mode);
    public:
        WloginHelper(oicq::Oicq *_oicq);

        void login(oicq::StMode mode);
    };
}


#endif //NATIVEQQ_WTLOGIN_H
