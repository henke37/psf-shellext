#include "extCl.h"

HRESULT PropExtCL::LoadProperties() {
	HRESULT hresult;
	assert(contentStream);

	hresult=PSCreateMemoryPropertyStore(IID_IPropertyStoreCache,(LPVOID *)&propCache);
	if(!SUCCEEDED(hresult)) {
		return hresult;
	}

	return S_OK;

}