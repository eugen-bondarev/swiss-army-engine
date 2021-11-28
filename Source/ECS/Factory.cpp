// #include "Factory.h"

// #include <stdexcept>

// #define DEBUG_ECS

// namespace ECSFactory
// {
//     void* Instantiate(const std::string& name)
//     {
// #ifdef DEBUG_ECS
//         if (factory.bases.find(name) == factory.bases.end())
//         {
//             throw std::runtime_error("Component " + name + " has not been registered.");
//         }
// #endif
//         return factory.bases[name]->Instantiate();
//     }

//     size_t GetSizeOf(const std::string& name)
//     {
//         return factory.sizesInBytes[name];
//     }
// }