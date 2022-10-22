#include "tup/Protobuf.h"

void tars::Protobuf::WriteFix64(uint32_t Field, int64_t l) {
    List->BaseTree->Child->emplace_back(Tree{
        GetField(Field, ProtobufStruct::ProtobufStructType::FIX64), Type::FIX64, [&]() -> void *{
            int64_t *l_ = new int64_t;
            *l_=l;
            return l_;
        }()
    });
}