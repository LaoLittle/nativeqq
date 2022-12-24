#ifndef NATIVEQQ_PROTOBUF_H
#define NATIVEQQ_PROTOBUF_H

#include <iostream>
#include <functional>
#include "util/tc_ex.h"
#include "util/tc_pack.h"
#include "util/tc_common.h"

namespace tars {
    namespace ProtobufStruct {
        enum class ProtobufStructType {
            VARINT = 0,
            FIX64 = 1,
            LENGTH = 2, // 3,4 was deprecated
            FIX32 = 5
        };
    }

    class Protobuf {
    private:
        enum class Type {
            VARINT,
            BIN,
            BIN_EX,
            LPBYTE_EX,
            TREE,
            FIX32,
            FIX64
        };

        struct VarInt {
            char *VarInt;
            int Length = 0;
        };

        struct BinData {
            uint32_t Length;
            VarInt LengthEx;
            char *Bin;
        };

        struct Tree {
            VarInt Field;
            Type Type;
            void *Data;

            std::vector<Tree> *Child;
            VarInt *Length;
        };

        struct LinkList {
            Tree *BaseTree;
            LinkList *Superior = nullptr;
        };
        /**
         * inner value
         */
        LinkList *List;

    public:
        Protobuf() {
            List = new LinkList;
            List->BaseTree = new Tree;
            List->BaseTree->Child = new std::vector<Tree>;
        };

        ~Protobuf() {
            delete List->BaseTree->Child;
            delete List->BaseTree;
            delete List;
        }

    private:
        VarInt Int2Varint(int64_t l) const {
            VarInt VarInt;
            VarInt.VarInt =  new char[10];
            VarInt.Length =  0;
            while (l >= 0x80) {
                VarInt.VarInt[VarInt.Length++] = (char) ((l & 0x7F) | 0x80);
                l >>= 7;
            }
            VarInt.VarInt[VarInt.Length++] = (char) l;
            return VarInt;
        }

        void Int2Varint(int64_t l, VarInt *VarInt) const {
            VarInt->VarInt = new char[10];
            while (l >= 0x80) {
                VarInt->VarInt[VarInt->Length++] = (char) ((l & 0x7F) | 0x80);
                l >>= 7;
            }
            VarInt->VarInt[VarInt->Length++] = (char) l;
        }

        inline VarInt GetField(uint32_t Field, ProtobufStruct::ProtobufStructType ProtobufStructType) const {
            return Int2Varint(Field << 3 | static_cast<int>(ProtobufStructType));
        }

        inline void GetField(uint32_t Field, ProtobufStruct::ProtobufStructType ProtobufStructType, VarInt &OutField) const {
            Int2Varint(Field << 3 | static_cast<int>(ProtobufStructType), &OutField);
        }

        inline void SetVarint(tars::TC_PackIn &Pack, VarInt *VarInt) const {
            Pack.write(VarInt->VarInt, VarInt->Length);
        }

        uint32_t Calculate(Tree *Tree) const {
            uint32_t Length = 0;
            for (auto & i : *Tree->Child)
            {
                Length += i.Field.Length;
                switch (i.Type)
                {
                    case Type::VARINT:
                        Length += ((VarInt *)i.Data)->Length;
                        break;
                    case Type::BIN:
                    case Type::BIN_EX:
                    case Type::LPBYTE_EX:
                        Length += ((BinData *)i.Data)->LengthEx.Length;
                        Length += ((BinData *)i.Data)->Length;
                        break;
                    case Type::TREE:
                    {
                        uint32_t Length_ = Calculate(&i);
                        Length += Length_;
                        Int2Varint(Length_, i.Length = new VarInt);
                        Length += i.Length->Length;
                    }
                        break;
                    case Type::FIX32:
                        Length += 4;
                        break;
                    case Type::FIX64:
                        Length += 8;
                        break;
                }
            }
            return Length;
        }

        void Recurse(tars::TC_PackIn &Pack, Tree* pTree) const {
            for (int i = 0; i < pTree->Child->size(); ++i)
            {
                SetVarint(Pack, &(*pTree->Child)[i].Field);
                auto Tpype = (*pTree->Child)[i].Type;
                switch (Tpype)
                {
                    case Type::VARINT:
                        SetVarint(Pack, (VarInt *)(*pTree->Child)[i].Data);
                        delete (VarInt *)(*pTree->Child)[i].Data;
                        break;
                    case Type::BIN:
                        SetVarint(Pack, &((BinData *)(*pTree->Child)[i].Data)->LengthEx);
                        Pack.write(((BinData *)(*pTree->Child)[i].Data)->Bin, ((BinData *)(*pTree->Child)[i].Data)->Length);
                        delete[](BinData *)(*pTree->Child)[i].Data;
                        break;
                    case Type::BIN_EX:
                        SetVarint(Pack, &((BinData *)(*pTree->Child)[i].Data)->LengthEx);
                        Pack.write(((BinData *)(*pTree->Child)[i].Data)->Bin, ((BinData *)(*pTree->Child)[i].Data)->Length);
                        delete[](((BinData *)(*pTree->Child)[i].Data)->Bin);
                        delete (BinData *)(*pTree->Child)[i].Data;
                        break;
                    case Type::LPBYTE_EX:
                        SetVarint(Pack, &((BinData *)(*pTree->Child)[i].Data)->LengthEx);
                        Pack.write(((BinData *)(*pTree->Child)[i].Data)->Bin, ((BinData *)(*pTree->Child)[i].Data)->Length);
                        delete[](((BinData *)(*pTree->Child)[i].Data)->Bin - 4);
                        delete (BinData *)(*pTree->Child)[i].Data;
                        break;
                    case Type::TREE:{
                        SetVarint(Pack, (*pTree->Child)[i].Length);
                        Recurse(Pack, &(*pTree->Child)[i]);
                        delete (*pTree->Child)[i].Length;
                        delete (*pTree->Child)[i].Child;
                        break;
                    }
                    case Type::FIX32:
                        Pack << *(int32_t *)(*pTree->Child)[i].Data;
                        delete (int32_t *)(*pTree->Child)[i].Data;
                        break;
                    case Type::FIX64:
                        Pack << *(int64_t *)(*pTree->Child)[i].Data;
                        delete (int64_t *)(*pTree->Child)[i].Data;
                        break;
                }
            }
        }

        void WriteFix64(uint32_t Field, int64_t l);

        void StepIn(uint32_t Field) {
            List->BaseTree->Child->emplace_back(Tree{GetField(Field, ProtobufStruct::ProtobufStructType::LENGTH), Type::TREE});
            List = new LinkList{&List->BaseTree->Child->back(), List};
            List->BaseTree->Child = new std::vector<Tree>;
        }

        void StepOut() {
            LinkList *tmp = List->Superior;
            delete List;
            List = tmp;
        }
    public:
        void writeStruct(int Field, const std::function<void()>& block) {
            StepIn(Field);
            block();
            StepOut();
        }

        void writeBool(int Field, bool z) {
            auto t = 0;
            if (z)
                t = 1;
            writeVarint(Field, t);
        }

        void writeVarint(int Field, int64_t l) {
            List->BaseTree->Child->emplace_back(Tree{
                    GetField(Field, ProtobufStruct::ProtobufStructType::VARINT), Type::VARINT, [&] () -> VarInt * {
                        auto * Data = new VarInt;
                        Int2Varint(l,Data);
                        return Data;
                    }()
            });
        }

        void writeFix32(int Field, int32_t i) {
            List->BaseTree->Child->emplace_back(Tree {
                    GetField(Field, ProtobufStruct::ProtobufStructType::FIX32), Type::FIX32, [&]() -> void *{
                        auto *i_ = new int32_t;*i_=i;
                        return i_;
                    }()
            });
        }

        void writeFix64(int Field, int64_t l) {
            WriteFix64(Field, l);
        }

        void writeString(int Field, char* str) {
            List->BaseTree->Child->emplace_back(Tree{
                    GetField(Field, ProtobufStruct::ProtobufStructType::LENGTH),
                    Type::BIN_EX,
                    [&] ()-> BinData *
                    {auto * Data=new BinData;Data->Length=strlen(str);
                        Int2Varint(Data->Length,&Data->LengthEx); Data->Bin=str;return Data; }()});
        }

        void writeString(int Field, const char* str) {
            List->BaseTree->Child->emplace_back(Tree{
                    GetField(Field, ProtobufStruct::ProtobufStructType::LENGTH),
                    Type::BIN,
                    [&]() -> BinData *
                    {auto * Data=new BinData;Data->Length=strlen((char*)str);
                        Int2Varint(Data->Length,&Data->LengthEx); Data->Bin=(char*)str;return Data; }()});
        }

        void writeBytes(int Field, const char* bin, uint32_t Length) {
            List->BaseTree->Child->emplace_back(Tree{
                    GetField(Field, ProtobufStruct::ProtobufStructType::LENGTH),
                    Type::BIN,
                    [&]() -> BinData *
                    {auto * Data=new BinData;Data->Length=Length;Int2Varint(Data->Length,&Data->LengthEx); Data->Bin=(char*)bin;return Data; }()});
        }

        void writeBytes(int Field, char* bin, uint32_t Length) {
            List->BaseTree->Child->emplace_back(Tree{
                    GetField(Field, ProtobufStruct::ProtobufStructType::LENGTH),
                    Type::BIN_EX,
                    [&]() -> BinData *
                    {auto * Data=new BinData;Data->Length=Length;Int2Varint(Data->Length,&Data->LengthEx); Data->Bin=bin;return Data; }()});
        }


        [[nodiscard]] string toPacket(size_t* size) const {
            tars::TC_PackIn Pack;
            Pack << Calculate(List->BaseTree) + 4;
            Recurse(Pack, List->BaseTree);
            List->BaseTree->Child->resize(0);
            *size = Pack.length();
            return Pack.topacket();
        }

        [[nodiscard]] vector<char> toPacket() const {
            tars::TC_PackIn Pack;
            Pack << Calculate(List->BaseTree) + 4;
            Recurse(Pack, List->BaseTree);
            List->BaseTree->Child->resize(0);
            const char* data = Pack.topacket().c_str();
            std::vector<char> vec(data, data + Pack.length());
            return vec;
        }
    };
}

#endif //NATIVEQQ_PROTOBUF_H
