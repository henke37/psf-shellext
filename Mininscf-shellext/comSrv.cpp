#include "common.h"
#include "comGlobals.h"
#include "extCl.h"
#include <new>
#include <assert.h>

int dllUseCount=0;

STDAPI DllCanUnloadNow(void);
STDAPI DllGetClassObject(
  _In_  REFCLSID rclsid,
  _In_  REFIID   riid,
  _Out_ LPVOID   *ppv
);

class ClassFactory : IClassFactory {
	~ClassFactory();

public:

	ClassFactory();

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

	HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) {
		if(fLock) {
			AddRef();
		} else {
			Release();
		}

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [annotation][unique][in] */ 
            _In_opt_  IUnknown *pUnkOuter,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
private:
	ULONG useCount;
};

STDAPI DllCanUnloadNow() {
	if(dllUseCount==0) {
		return S_OK;
	} else {
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(
  _In_  REFCLSID rclsid,
  _In_  REFIID   riid,
  _Out_ LPVOID   *ppv
) {
	if(riid==IID_IClassFactory) {
		*ppv=new ClassFactory();
		return S_OK;
	}

	return CLASS_E_CLASSNOTAVAILABLE;
}

ClassFactory::ClassFactory() : useCount(1) {
	dllUseCount++;
}

ClassFactory::~ClassFactory() {
	assert(dllUseCount > 0);
	assert(useCount == 0);
	dllUseCount--;
}

HRESULT ClassFactory::QueryInterface(REFIID riid,_COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
	if(!ppvObject) return E_POINTER;

	if(riid==IID_IUnknown) {
		*ppvObject=(IUnknown*)this;
		AddRef();
		return S_OK;
	} else if(riid==IID_IClassFactory) {
		*ppvObject=(IClassFactory*)this;
		AddRef();
		return S_OK;
	}

	*ppvObject=NULL;
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE ClassFactory::CreateInstance( 
	_In_opt_  IUnknown *pUnkOuter,
	_In_  REFIID riid,
	_COM_Outptr_  void **ppvObject) {
	if(pUnkOuter) return CLASS_E_NOAGGREGATION;

	PropExtCL *cl=new(std::nothrow) PropExtCL();

	if(!cl) {
		return E_OUTOFMEMORY;
	}

	HRESULT hasInterface=cl->QueryInterface(riid,ppvObject);

	if(!SUCCEEDED(hasInterface)) {
		cl->Release();
	}

	return hasInterface;
}