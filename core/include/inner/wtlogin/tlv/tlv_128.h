#include "inner/wtlogin/tlv.h"

#include <util/tc_common.h>

namespace tlv {
    tars::TC_PackIn get_tlv128_data(
        bool guid_available, char* guid,
        char* model, unsigned int model_len,
        char* brand, unsigned int brand_len
    ) {
        return build_tlv(0x128, [&](auto& pack){
            pack << (short) (0);
            pack << false; // isGuidFromFileNull
            pack << guid_available; // isGuidAvailable
            pack << false; // isGuidChanged
            pack << (int) (0x11000000);

            pack << (short) (model_len);
            pack.write(model, model_len);

            pack << (short) (16);
            pack.write(guid, 16);

            pack << (short) (brand_len);
            pack.write(brand, brand_len);
        });
    }
}
