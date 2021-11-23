#include "Debugger.h"

namespace DX
{
    Debugger::Debugger()
    {
        // define function signature of DXGIGetDebugInterface
        typedef HRESULT(WINAPI * DXGIGetDebugInterface)(REFIID, void **);

        // load the dll that contains the function DXGIGetDebugInterface
        const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!hModDxgiDebug)
        {
            throw EXCEPTION();
        }

        // get address of DXGIGetDebugInterface in dll
        const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
            reinterpret_cast<void *>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));

        if (!DxgiGetDebugInterface)
        {
            throw EXCEPTION();
        }

        __DX_TRY(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &dxInfoQueue));
    }

    void Debugger::Start()
    {
        next = dxInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    }

    void Debugger::End(const std::string& file, const unsigned int line)
    {
        const auto end = dxInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

        for (auto i = next; i < end; ++i)
        {
            SIZE_T messageLength;
            dxInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);

            // allocate memory for message
            const std::unique_ptr<byte[]> bytes = std::make_unique<byte[]>(messageLength);
            DXGI_INFO_QUEUE_MESSAGE *message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE *>(bytes.get());

            dxInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength);

            messages.emplace_back(message->pDescription);
        }

        for (size_t i = 0; i < messages.size(); ++i)
        {
            throw std::runtime_error(file + ", " + std::to_string(line) + "\n\n" + messages[i]);
        }
    }
}