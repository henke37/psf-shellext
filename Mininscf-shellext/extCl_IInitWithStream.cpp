#include "extCl.h"

HRESULT PropExtCL::Initialize(IStream *pstream,DWORD grfMode) {
	HRESULT hresult;
	if(initialized) {
		return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);
	}

	readOnlyMode= grfMode==STGM_READ;
	contentStream=pstream;

	hresult=LoadProperties();
	if(!SUCCEEDED(hresult)) {
		return hresult;
	}

	return S_OK;
}