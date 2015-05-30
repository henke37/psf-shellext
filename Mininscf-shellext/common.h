#ifndef COMMON_H
#define COMMON_H

#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOSERVICE
#define NOIME
#include <Windows.h>
#include <Unknwn.h>

#define DllExport extern "C" __declspec( dllexport ) 

#define retIfFail { if(!SUCCEEDED(hresult)) return hresult; }
#define retIfNonOk if(hresult==S_FALSE) { return E_FAIL; } else retIfFail


#endif