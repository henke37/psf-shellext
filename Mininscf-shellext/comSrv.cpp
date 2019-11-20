#include "common.h"
#include "globals.h"
#include "extCl.h"
#include "classFactory.h"
#include <new>
#include <assert.h>

int dllUseCount=0;

STDAPI DllCanUnloadNow(void);
STDAPI DllGetClassObject(
  _In_  REFCLSID rclsid,
  _In_  REFIID   riid,
  _Out_ LPVOID   *ppv
);


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
	try {

		if(riid==IID_IClassFactory) {
			*ppv=new ClassFactory<PropExtCL>();
			return S_OK;
		}

		return CLASS_E_CLASSNOTAVAILABLE; 
	} catch (std::bad_alloc ba) {
		return E_OUTOFMEMORY;
	}
}