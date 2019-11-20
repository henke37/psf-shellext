#include "common.h"
#include "globals.h"
#include "extCl.h"
#include "guids.h"
#include "classFactory.h"
#include <new>
#include <assert.h>
#include <atomic>

std::atomic<int> dllUseCount=0;

STDAPI DllCanUnloadNow() {
	if(dllUseCount==0) {
		return S_OK;
	} else {
		return S_FALSE;
	}
}

template<class T> HRESULT MakeFactory(_In_  REFIID   riid,
	_Out_ LPVOID* ppv) {
	auto factory = new ClassFactory<T>();
	HRESULT querySuccess = factory->QueryInterface(riid, ppv);
	factory->Release();
	return querySuccess;
};

STDAPI DllGetClassObject(
  _In_  REFCLSID rclsid,
  _In_  REFIID   riid,
  _Out_ LPVOID   *ppv
) {
	if (!ppv) return E_POINTER;
	try {
		if (rclsid == PropExtCL_GUID) return MakeFactory<PropExtCL>(riid,ppv);
		return CLASS_E_CLASSNOTAVAILABLE; 
	} catch (std::bad_alloc ba) {
		return E_OUTOFMEMORY;
	}
}