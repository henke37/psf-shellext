#include "extCl.h"
#include "psfParser.h"
#include "pkeys.h"

#include <cstdint>

HRESULT PropExtCL::LoadProperties() {
	HRESULT hresult;
	assert(contentStream);

	try {

		hresult=PSCreateMemoryPropertyStore(IID_IPropertyStoreCache,(LPVOID *)&propCache);
		if(!SUCCEEDED(hresult)) {
			return hresult;
		}

		parser=new PsfParser(contentStream);

		hresult=parser->parse();
		retIfFail;

		hresult=SetValue(PKEY_PSF_FMTID,(uint8_t)parser->version);
		retIfFail;

		for(auto &kv: parser->tags) {
			auto &key=kv.first;
			auto &value=kv.second;

			hresult=storeTagAsProp(key,value);
			retIfFail;
		}

		return S_OK;

	} catch (std::bad_alloc ba) {
		return E_OUTOFMEMORY;
	}

}