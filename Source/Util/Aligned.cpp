#include "Aligned.h"

namespace Util
{
    namespace Mem
    {
        namespace Aligned
        {
            unsigned int minUniformBufferOffsetAlignment;

            void Free(void *data)
            {
#if defined(_MSC_VER) || defined(__MINGW32__)
                _aligned_free(data);
#else
                free(data);
#endif
            }
        }
    }
}