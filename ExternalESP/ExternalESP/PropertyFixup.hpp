#pragma once
#include <cstdint>

namespace SDK
{
    struct FProperty
    {
        uint64_t pad_0000[0x70 / 8];
    };

    template<typename T>
    struct TProperty : FProperty
    {
        T* GetPropertyValue(void* Object) const
        {
            return (T*)((uint8_t*)Object + Offset_Internal);
        }

        uint32_t Offset_Internal;
    };
}