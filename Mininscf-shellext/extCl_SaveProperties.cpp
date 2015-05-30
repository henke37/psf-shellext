#include "extCl.h"

HRESULT PropExtCL::SaveProperties() {
	HRESULT hresult;
	assert(contentStream);

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
}