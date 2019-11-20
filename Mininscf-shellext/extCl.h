#ifndef EXTCL_H
#define EXTCL_H

#include "common.h"
#include "globals.h"
#include <assert.h>
#include <Propsys.h>
#include <string>
#include <cstdint>
#include <atomic>

class PsfParser;

class PropExtCL : IUnknown, IInitializeWithStream, IPropertyStore, IPropertyStoreCapabilities {
	~PropExtCL();

	HRESULT LoadProperties();
	HRESULT SaveProperties();

	HRESULT storeTagAsProp(const std::string &tagName, const std::string &tagValue);
	HRESULT storePropAsTag(REFPROPERTYKEY key);

	HRESULT SetValue(REFPROPERTYKEY key,const std::string &value);
	HRESULT SetValue(REFPROPERTYKEY key,const std::wstring &value);
	HRESULT SetValue(REFPROPERTYKEY key,uint64_t);
	HRESULT SetValue(REFPROPERTYKEY key,int64_t);
	HRESULT SetValue(REFPROPERTYKEY key,uint32_t);
	HRESULT SetValue(REFPROPERTYKEY key,int32_t);
	HRESULT SetValue(REFPROPERTYKEY key,uint16_t);
	HRESULT SetValue(REFPROPERTYKEY key,int16_t);

	HRESULT SetTagFromStringProp(REFPROPERTYKEY key,const std::string &tagName);
	HRESULT SetTagFromIntProp(REFPROPERTYKEY key,const std::string &tagName, int scale);
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
			return 0;
		}

		return useCount;
	}

private:
	bool initialized;
	bool readOnlyMode;
	std::atomic<int> useCount;
	IStream *contentStream;
	IPropertyStoreCache *propCache;
	PsfParser *parser;
};

#endif EXTCL_H