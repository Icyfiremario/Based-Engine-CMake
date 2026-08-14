#ifndef BEUTILS_H
#define BEUTILS_H

// STD
#include <functional>

namespace BasedEngine
{
    namespace Utils
    {
        template<typename T, typename... Rest>
        void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
        {
            seed ^= std::hash<T>{}(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
            (hashCombine(seed, rest), ...);
        }
    }

    enum API : unsigned
    {
        OPENGL,
        VULKAN,
        DIRECTX,
    };

    static inline int currentAPI = VULKAN;
}

#endif // BEUTILS_H
