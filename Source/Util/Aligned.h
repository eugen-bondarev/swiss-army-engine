#pragma once

#include <memory>
#include "../Common/Common.h"

namespace Util
{
    namespace Mem
    {
        namespace Aligned
        {
            void SetMinUniformBufferOffsetAlignment(const unsigned int value);
            unsigned int GetMinUniformBufferOffsetAlignment();

            template <typename T>
            T* Alloc(const size_t size, const size_t alignment)
            {
                void* data{nullptr};
#if defined(_MSC_VER) || defined(__MINGW32__)
                data = _aligned_malloc(size, alignment);
#else
                const int res{posix_memalign(&data, alignment, size)};
                if (res != 0)
                {
                    data = nullptr;
                }
#endif
                return static_cast<T*>(data);
            }

            void Free(void* data);
        }
    }
}

template <int Size>
class RawDynamicAlignment
{
public:
    inline static void Calculate()
    {
        static const unsigned int minAlignment{Util::Mem::Aligned::GetMinUniformBufferOffsetAlignment()};

        if (minAlignment > 0) 
        {
            value = (Size + minAlignment - 1) & static_cast<unsigned int>(~(minAlignment - 1));
        }
        else
        {
            value = Size;
        }
    }

    template <bool PerformCalculation = false>
    inline static unsigned int Get()
    {
        if (PerformCalculation)
        {
            Calculate();
        }

        return value;
    }

private:
    inline static unsigned int value{0};
};

template <typename T>
using DynamicAlignment = RawDynamicAlignment<sizeof(T)>;

template <typename T>
class Aligned
{
public:
    Aligned(const size_t numInstances)
    {
        DynamicAlignment<T>::Calculate();
        data = Util::Mem::Aligned::Alloc<T>(numInstances * DynamicAlignment<T>::Get(), DynamicAlignment<T>::Get());
    }

    ~Aligned()
    {
        Util::Mem::Aligned::Free(data);
    }

    T& operator[](const size_t i)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<uint64_t>(data) + i * DynamicAlignment<T>::Get());
    }

    const T& operator[](const size_t i) const
    {
        return *reinterpret_cast<T*>(reinterpret_cast<uint64_t>(data) + i * DynamicAlignment<T>::Get());
    }

    const void* GetPtr() const
    {
        return data;
    }

private:
    T* data;
};