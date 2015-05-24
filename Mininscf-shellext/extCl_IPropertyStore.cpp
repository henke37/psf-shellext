#include "extCl.h"

HRESULT PropExtCL::GetAt(DWORD iProp, PROPERTYKEY *pkey) { return propCache->GetAt(iProp,pkey); }
HRESULT PropExtCL::GetCount(DWORD *cProps) { return propCache->GetCount(cProps); }
HRESULT PropExtCL::GetValue(REFPROPERTYKEY key,PROPVARIANT *pv) { return propCache->GetValue(key,pv); }
HRESULT PropExtCL::SetValue(REFPROPERTYKEY key,REFPROPVARIANT propvar) {
	if(readOnlyMode) {
		return STG_E_ACCESSDENIED;
	}
	if(IsPropertyWritable(key)==S_FALSE) {
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
	}
	return propCache->SetValue(key,propvar);
}

HRESULT PropExtCL::Commit() {
	HRESULT hresult;
	if(readOnlyMode) {
		return STG_E_ACCESSDENIED;
	}

	hresult=SaveProperties();
	if(!SUCCEEDED(hresult)) {
		return hresult;
	}

	hresult=contentStream->Commit(STGC_DEFAULT);
	if(!SUCCEEDED(hresult)) {
		return hresult;
	}
	return S_OK;
};