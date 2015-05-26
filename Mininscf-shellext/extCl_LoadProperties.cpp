#include "extCl.h"
#include "psfParser.h"

HRESULT PropExtCL::LoadProperties() {
	HRESULT hresult;
	assert(contentStream);

	hresult=PSCreateMemoryPropertyStore(IID_IPropertyStoreCache,(LPVOID *)&propCache);
	if(!SUCCEEDED(hresult)) {
		return hresult;
	}

	parser=new(std::nothrow) PsfParser(contentStream);
	if(!parser) return E_OUTOFMEMORY;

	hresult=parser->parse();
	if(!SUCCEEDED(hresult)) {
		return hresult;
	}

	return S_OK;

}