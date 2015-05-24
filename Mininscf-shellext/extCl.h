#ifndef EXTCL_H
#define EXTCL_H

#include "common.h"
#include "globals.h"
#include <assert.h>
#include <Propsys.h>

class PropExtCL : IUnknown, IInitializeWithStream, IPropertyStore, IPropertyStoreCapabilities {
	~PropExtCL();

	HRESULT LoadProperties();
	HRESULT SaveProperties();
public:

	PropExtCL();

	//IInitializeWithStream
	HRESULT STDMETHODCALLTYPE Initialize(IStream *pstream,DWORD grfMode);

	//IPropertyStore
	HRESULT STDMETHODCALLTYPE GetAt(DWORD iProp, PROPERTYKEY *pkey);
	HRESULT STDMETHODCALLTYPE GetCount(DWORD *cProps);
	HRESULT STDMETHODCALLTYPE GetValue(REFPROPERTYKEY key,PROPVARIANT *pv);
	HRESULT STDMETHODCALLTYPE SetValue(REFPROPERTYKEY key,REFPROPVARIANT propvar);
	HRESULT STDMETHODCALLTYPE Commit();

	//IPropertyStoreCapabilities
	HRESULT STDMETHODCALLTYPE IsPropertyWritable(REFPROPERTYKEY key);

	//IUnknown

	HRESULT STDMETHODCALLTYPE QueryInterface( 
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG STDMETHODCALLTYPE AddRef() {
		return ++useCount;
	}

	ULONG STDMETHODCALLTYPE Release() {
		assert(useCount > 0);
		useCount--;

		if(useCount==0) {
			delete(this);
			dllUseCount--;
		}

		return useCount;
	}

private:
	bool initialized;
	bool readOnlyMode;
	ULONG useCount;
	IStream *contentStream;
	IPropertyStoreCache *propCache;
};

#endif EXTCL_H