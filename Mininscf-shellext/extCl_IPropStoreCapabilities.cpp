#include "extCl.h"
#include "guids.h"
#include "pkeys.h"
#include <propkey.h>
#include <Propvarutil.h>

HRESULT PropExtCL::IsPropertyWritable(REFPROPERTYKEY key) {

	if(key==PKEY_PSF_FadeTime) return S_OK;
	if(key==PKEY_Title) return S_OK;
	if(key==PKEY_Music_Artist) return S_OK;
	if(key==PKEY_Music_Genre) return S_OK;
	if(key==PKEY_Comment) return S_OK;
	if(key==PKEY_Copyright) return S_OK;
	if(key==PKEY_Media_Year) return S_OK;
	if(key==PKEY_Music_TrackNumber) return S_OK;

	if(key==PKEY_PSF_Mini_Library) return S_FALSE;
	if(key==PKEY_Media_Duration) return S_FALSE;

	return S_FALSE;
}