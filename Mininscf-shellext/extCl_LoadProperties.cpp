#include "extCl.h"
#include "psfParser.h"
#include "pkeys.h"

#include <cstdint>

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

}