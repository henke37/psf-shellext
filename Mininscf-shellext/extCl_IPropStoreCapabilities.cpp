#include "extCl.h"
#include "guids.h"
#include "pkeys.h"

HRESULT PropExtCL::IsPropertyWritable(REFPROPERTYKEY key) {

	if(key==PKEY_PSF_FadeTime) return S_OK;

	return S_FALSE;
}