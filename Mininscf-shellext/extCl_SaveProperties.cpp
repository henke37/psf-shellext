#include "extCl.h"

HRESULT PropExtCL::SaveProperties() {
	HRESULT hresult;
	assert(contentStream);
	try {

		DWORD propCount;
		hresult=propCache->GetCount(&propCount);
		for(DWORD propIndex=0;propIndex<propCount;++propIndex) {
			PROPERTYKEY pkey;
			hresult=propCache->GetAt(propIndex,&pkey);
			retIfFail;

			hresult=storePropAsTag(pkey);
			retIfFail;
		}

		return S_OK;
	} catch (std::bad_alloc ba) {
		return E_OUTOFMEMORY;
	}
}