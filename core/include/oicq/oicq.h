#ifndef NATIVEQQ_OICQ_H
#define NATIVEQQ_OICQ_H

#include "net/client.h"

namespace oicq {
    /**
     * OICQ主类
     */
    class Oicq {

    protected:
        /**
         * net发包类
         */
        OicqClient client;
    };
}

#endif //NATIVEQQ_OICQ_H
