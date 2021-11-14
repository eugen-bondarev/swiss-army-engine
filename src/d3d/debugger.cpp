#include "debugger.h"

#include <memory>

Debugger::Debugger()
{   
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID,void **);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32 );
	if( hModDxgiDebug == nullptr )
	{
        throw EXCEPTION();
		// throw CHWND_LAST_EXCEPT();
	}

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress( hModDxgiDebug,"DXGIGetDebugInterface" ))
	);
	if( DxgiGetDebugInterface == nullptr )
	{
        throw EXCEPTION();
	}

	HRESULT hr;
	DxgiGetDebugInterface( __uuidof(IDXGIInfoQueue),&dxgiInfoQueue );
}

void Debugger::Start()
{
	next = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

void Debugger::End(const std::string& file, const unsigned int line)
{
	const auto end = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for(auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;
		// get the size of message i in bytes

        dxgiInfoQueue->GetMessage( DXGI_DEBUG_ALL,i,nullptr,&messageLength );

		// allocate memory for message
		auto bytes = std::make_unique<byte[]>( messageLength );
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		// get the message and push its description into the vector
        dxgiInfoQueue->GetMessage( DXGI_DEBUG_ALL,i,pMessage,&messageLength);

		messages.emplace_back( pMessage->pDescription );
	}

    for (size_t i = 0; i < messages.size(); i++)
    {
        throw std::runtime_error(file + ", " + std::to_string(line) + "\n\n" + messages[i]);
    }
}