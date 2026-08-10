#ifndef BDXMINGWFIX_H
#define BDXMINGWFIX_H

#if defined(__MINGW32__)
#include <guiddef.h>

// Fallback macro definition if it isn't already exposed
#ifndef __CRT_UUID_DECL
#define __CRT_UUID_DECL(type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern "C++" { \
        template<> inline const GUID &__mingw_uuidof<type>() { \
            static const GUID __uuid_inst = {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}; \
            return __uuid_inst; \
        } \
    }
#endif

// Predefined UUIDs for DirectX 12 interfaces missing in MinGW
__CRT_UUID_DECL(ID3D12Debug,  0x344488b7, 0x6846, 0x474b, 0xb9, 0x89, 0xf0, 0x27, 0x44, 0x82, 0x45, 0xe0)
__CRT_UUID_DECL(ID3D12Device, 0x189819f1, 0x1db6, 0x4b57, 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7)

#endif

#endif // BDXMINGWFIX_H
