#ifndef CLASS_FACTORY_H
#define CLASS_FACTORY_H

#include "common.h"
#include <atomic>

template<class T> class ClassFactory : IClassFactory {
public:
	ClassFactory() : useCount(1) {
		dllUseCount++;
	}

	~ClassFactory() {
		assert(dllUseCount > 0);
		assert(useCount == 0);
		dllUseCount--;
	}

	ULONG STDMETHODCALLTYPE AddRef() {
		return ++useCount;
	}

	ULONG STDMETHODCALLTYPE Release() {
		assert(useCount > 0);
		useCount--;

		if (useCount == 0) {
			delete(this);
			return 0;
		}

		return useCount;
	}

	HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) {
		if (fLock) {
			AddRef();
		}
		else {
			Release();
		}

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE CreateInstance(
		/* [annotation][unique][in] */
		_In_opt_  IUnknown* pUnkOuter,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void** ppvObject) {
		try {
			if (pUnkOuter) return CLASS_E_NOAGGREGATION;

			PropExtCL* cl = new T();

			HRESULT hasInterface = cl->QueryInterface(riid, ppvObject);

			cl->Release();

			return hasInterface;
		}
		catch (std::bad_alloc ba) {
			return E_OUTOFMEMORY;
		}
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) {
		if (!ppvObject) return E_POINTER;

		if (riid == IID_IUnknown) {
			*ppvObject = (IUnknown*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IClassFactory) {
			*ppvObject = (IClassFactory*)this;
			AddRef();
			return S_OK;
		}

		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
private:
	std::atomic<int> useCount;
};

#endif