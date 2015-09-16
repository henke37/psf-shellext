#include "guids.h"
#include <propkey.h>
#include <string>
#include <memory>
#include "extCl.h"
#include <Propvarutil.h>
#include "pkeys.h"
#include "string2int_t.h"
#include "psfParser.h"

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop,const std::string &tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;
	try {
		int wStrLen;
		WCHAR *wStr;

		wStrLen=MultiByteToWideChar(CP_UTF8,0,tagValue.c_str(),-1,NULL,0);
		wStr=new WCHAR[wStrLen];

		MultiByteToWideChar(CP_UTF8,0,tagValue.c_str(),-1,wStr,wStrLen);

		hresult=InitPropVariantFromString(wStr,&propVar);
		retIfFail;

		hresult=propCache->SetValue(prop,propVar);
		retIfFail;

		//TODO: use a smart ptr to avoid leaking memory here
		delete[] wStr;

		return S_OK;
	} catch (std::bad_alloc ba) {
		return E_OUTOFMEMORY;
	}
}

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop, uint32_t tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;

	hresult=InitPropVariantFromUInt32(tagValue,&propVar);
	retIfFail;

	hresult=propCache->SetValue(prop,propVar);
	retIfFail;

	return S_OK;
}

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop, int32_t tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;

	hresult=InitPropVariantFromInt32(tagValue,&propVar);
	retIfFail;

	hresult=propCache->SetValue(prop,propVar);
	retIfFail;

	return S_OK;
}

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop, int16_t tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;

	hresult=InitPropVariantFromInt16(tagValue,&propVar);
	retIfFail;

	hresult=propCache->SetValue(prop,propVar);
	retIfFail;

	return S_OK;
}

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop, uint16_t tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;

	hresult=InitPropVariantFromUInt16(tagValue,&propVar);
	retIfFail;

	hresult=propCache->SetValue(prop,propVar);
	retIfFail;

	return S_OK;
}

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop, int64_t tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;

	hresult=InitPropVariantFromInt64(tagValue,&propVar);
	retIfFail;

	hresult=propCache->SetValue(prop,propVar);
	retIfFail;

	return S_OK;
}

HRESULT PropExtCL::SetValue(struct _tagpropertykey const &prop, uint64_t tagValue) {
	PROPVARIANT propVar;
	HRESULT hresult;

	hresult=InitPropVariantFromUInt64(tagValue,&propVar);
	retIfFail;

	hresult=propCache->SetValue(prop,propVar);
	retIfFail;

	return S_OK;
}