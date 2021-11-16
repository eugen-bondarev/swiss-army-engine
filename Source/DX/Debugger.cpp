#include "Debugger.h"

namespace DX {

Debugger::Debugger()
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

	__DX_TRY(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &DXInfoQueue));
}

void Debugger::Start()
{
	Next = DXInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

void Debugger::End(const std::string& File, const unsigned int Line)
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

}