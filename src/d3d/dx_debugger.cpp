#include "dx_debugger.h"

#include <memory>

ID3D11Debugger::ID3D11Debugger()
{   
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID,void **);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32 );
	if (!hModDxgiDebug)
	{
        throw EXCEPTION();
	}

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress( hModDxgiDebug,"DXGIGetDebugInterface" ))
	);

	if (!DxgiGetDebugInterface)
	{
        throw EXCEPTION();
	}

	__D3D_TRY(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &DXInfoQueue));
}

void ID3D11Debugger::Start()
{
	Next = DXInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

void ID3D11Debugger::End(const std::string& File, const unsigned int Line)
{
	const auto end = DXInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = Next; i < end; ++i)
	{
		SIZE_T messageLength;
        DXInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);

		// allocate memory for message
		const std::unique_ptr<byte[]> bytes = std::make_unique<byte[]>( messageLength );
		DXGI_INFO_QUEUE_MESSAGE* message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

        DXInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength);

		Messages.emplace_back(message->pDescription);
	}

    for (size_t i = 0; i < Messages.size(); ++i)
    {
        throw std::runtime_error(File + ", " + std::to_string(Line) + "\n\n" + Messages[i]);
    }
}