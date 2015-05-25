#include "extCl.h"
#include "guids.h"

HRESULT PropExtCL::IsPropertyWritable(REFPROPERTYKEY key) {

	if(key.fmtid==PCF_Prop_NCSF_FadeTime) return S_OK;

	return S_FALSE;
}