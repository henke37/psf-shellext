#include "extCl.h"
#include "psfParser.h"

PropExtCL::PropExtCL() : useCount(1), initialized(false), contentStream(nullptr), propCache(nullptr), parser(nullptr) {
	++dllUseCount;
}

PropExtCL::~PropExtCL() {
	if(contentStream) {
		contentStream->Release();
	}
	if(propCache) {
		propCache->Release();
	}
	if(parser) {
		delete parser;
	}

	assert(useCount==0);
	--dllUseCount;
}

HRESULT PropExtCL::QueryInterface(REFIID riid,_COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
	if(!ppvObject) return E_POINTER;

	if(riid==IID_IUnknown) {
		*ppvObject=(IUnknown *) this;
		AddRef();
		return S_OK;
	} else if(riid==IID_IInitializeWithStream) {
		*ppvObject=(IInitializeWithStream *) this;
		AddRef();
		return S_OK;
	} else if(riid==IID_IPropertyStore) {
		*ppvObject=(IPropertyStore *) this;
		AddRef();
		return S_OK;
	}

	*ppvObject=NULL;
	return E_NOINTERFACE;
}