#pragma once

#include <memory>
#include "../Common/Common.h"

namespace Util
{
    namespace Mem
    {
        namespace Aligned
        {
            extern unsigned int minUniformBufferOffsetAlignment;

            template <typename T>
            T* Alloc(size_t size, size_t alignment)
            {
                void *data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
                data = _aligned_malloc(size, alignment);
#else
                int res = posix_memalign(&data, alignment, size);
                if (res != 0)
                    data = nullptr;
#endif
                return (T*) data;
            }

            void Free(void *data);
        }
    }
}

template <typename T>
class DynamicAlignment
{
public:
    inline static void Calculate()
    {
        if (Util::Mem::Aligned::minUniformBufferOffsetAlignment > 0) 
        {
            DynamicAlignment<T>::value = ((sizeof(T) + Util::Mem::Aligned::minUniformBufferOffsetAlignment - 1) & static_cast<unsigned int>(~(Util::Mem::Aligned::minUniformBufferOffsetAlignment - 1)));
        }
        else
        {
            DynamicAlignment<T>::value = sizeof(T);
        }
    }

    inline static unsigned int Get()
    {
        return value;
    }

private:
    inline static unsigned int value{0};
};

template <typename T>
struct Aligned
{
    T* data;

    Aligned(unsigned int amount_of_instances)
    {
        DynamicAlignment<T>::Calculate();
        data = Util::Mem::Aligned::Alloc<T>(amount_of_instances * DynamicAlignment<T>::Get(), DynamicAlignment<T>::Get());
    }

    ~Aligned()
    {
        Util::Mem::Aligned::Free(data);
    }

    T& operator[](int i)
    {		
        T& item = *(T*)(((uint64_t)data + (i * DynamicAlignment<T>::Get())));

        return item;
    }

    const T& operator[](int i) const
    {		
        T& item = *(T*)(((uint64_t)data + (i * DynamicAlignment<T>::Get())));

        return item;
    }
};