#include "common.h"
#include <Shlobj.h>
#include "guids.h"
#include <string.h>
#include <tchar.h>
#include "globals.h"

DllExport HRESULT __stdcall DllRegisterServer(void);
DllExport HRESULT __stdcall DllUnregisterServer(void);

#define tcharStrByteLen(TStr) \
	(_tcslen(TStr)+1)*sizeof(TCHAR))

#define RegSetKeyValueStringLiteral( baseKey, subkey, valueName, valueData) \
	RegSetKeyValue(baseKey,subkey,valueName, REG_SZ, \
	TEXT(valueData), tcharStrByteLen(TEXT(valueData));
#define RegSetKeyValueString( baseKey, subKey, valueName, valueData, valueDataLen) \
	RegSetKeyValue(baseKey,subKey,valueName, REG_SZ, \
	valueData, valueDataLen);

#define EXTDESC "PCF File Property handler"

#define extCount 3
static const LPTCH extensions[extCount]= {
	TEXT(".ncsf"), TEXT(".minincsf"), TEXT(".ncsflib")
};

HRESULT __stdcall DllRegisterServer(void) {
	LSTATUS status;
	//HRESULT hresult;
	
	TCHAR dllPath[MAX_PATH];
	//include the terminator, must be in octets
	DWORD dllPathLen=(GetModuleFileName(dllHinstance,dllPath,MAX_PATH)+1)*sizeof(TCHAR);
	
#define guidStrMaxLen 40
	TCHAR guidStr[guidStrMaxLen];
	
	//include the terminator, must be in octets
	DWORD guidStrLen=StringFromGUID2(PropExtCL_GUID,guidStr,guidStrMaxLen)*sizeof(TCHAR);

	{
		TCHAR clsIdKeyName[200];
		HKEY clsIdKey;

		wsprintf(clsIdKeyName,TEXT("Software\\Classes\\%s"),guidStr);

		status=RegCreateKeyEx(HKEY_LOCAL_MACHINE,clsIdKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&clsIdKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueStringLiteral(clsIdKey,NULL,NULL,EXTDESC);
		RegSetKeyValue(clsIdKey,TEXT("InProcServer32"),NULL,REG_SZ,dllPath,dllPathLen);
		RegSetKeyValueStringLiteral(clsIdKey,TEXT("InProcServer32"),TEXT("ThreadingModel"),"Apartment");

		RegCloseKey(clsIdKey);
	}

	{
		TCHAR *clsTypeKeyName=TEXT("Software\\Classes\\PSFFile");
		HKEY clsTypeKey;

		status=RegCreateKeyEx(HKEY_LOCAL_MACHINE,clsTypeKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&clsTypeKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueStringLiteral(clsTypeKey,NULL,NULL,"PCF Sequence file");


		RegCloseKey(clsTypeKey);
	}

	for(unsigned int extIndex=0;extIndex<extCount;++extIndex) {
		TCHAR clsExtKeyName[120];
		HKEY clsExtKey;
		TCHAR *extension=extensions[extIndex];

		wsprintf(clsExtKeyName,TEXT("Software\\Classes\\%s"),extension);

		status=RegCreateKeyEx(HKEY_LOCAL_MACHINE,clsExtKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&clsExtKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueStringLiteral(clsExtKey,NULL,NULL,"PSFFile");


		RegCloseKey(clsExtKey);
	}

	{
		TCHAR *propHandlerKeyName=TEXT("Software\\Microsoft\\WindowsCurrentVersion\\PropertySystem\\PropertyHandlers");
		HKEY phKey;
		status=RegOpenKeyEx(HKEY_LOCAL_MACHINE,propHandlerKeyName,0,KEY_ALL_ACCESS,&phKey);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		for(unsigned int extIndex=0;extIndex<extCount;++extIndex) {
			TCHAR *extension=extensions[extIndex];
			RegSetKeyValue(phKey,extension,NULL,REG_SZ,guidStr,guidStrLen);
		}

		RegCloseKey(phKey);
	}

	{
		TCHAR *shextApprovedKeyName=TEXT("Software\\Microsoft\\WindowsCurrentVersion\\Shell Extensions\\Approved");
		HKEY shextApprovedKey;
		status=RegOpenKeyEx(HKEY_LOCAL_MACHINE,shextApprovedKeyName,0,KEY_ALL_ACCESS,&shextApprovedKey);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValue(shextApprovedKey,NULL,guidStr,REG_SZ, EXTDESC, tcharStrByteLen(TEXT(EXTDESC));

		RegCloseKey(shextApprovedKey);
	}

	{
		TCHAR *kindKeyName=TEXT("Software\\Microsoft\\WindowsCurrentVersion\\Explorer\\KindMap");;
		HKEY kindKey;

		status=RegCreateKeyEx(HKEY_CLASSES_ROOT,kindKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&kindKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		for(unsigned int extIndex=0;extIndex<extCount;++extIndex) {
			TCHAR *extension=extensions[extIndex];
			RegSetKeyValueStringLiteral(kindKey,NULL,extension,"music");
		}


		RegCloseKey(kindKey);
	}

	/*
	{
		WCHAR propPath[MAX_PATH];
		hresult=PSRegisterPropertySchema(propPath);
		if(!SUCCEEDED(hresult)) {
			return hresult;
		}
	}*/
	
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	return S_OK;
}

HRESULT __stdcall DllUnregisterServer(void) {

	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	return S_OK;
}