#include "common.h"
#include <Shlobj.h>
#include "guids.h"
#include <string.h>
#include <tchar.h>

DllExport HRESULT __stdcall DllRegisterServer(void);
DllExport HRESULT __stdcall DllUnregisterServer(void);

#define tcharStrByteLen(TStr) \
	(_tcslen(TStr)+1)*sizeof(TCHAR))

#define RegSetKeyValueStringLiteral( baseKey, subkey, valueName, valueData) \
	RegSetKeyValue(baseKey,TEXT(subkey),TEXT(valueName), REG_SZ, \
	TEXT(valueData), tcharStrByteLen(TEXT(valueData));
#define RegSetKeyValueString( baseKey, subKey, valueName, valueData, valueDataLen) \
	RegSetKeyValue(baseKey,TEXT(subKey),TEXT(valueName), REG_SZ, \
	valueData, valueDataLen);

#define FILEEXT ".mininscf"
#define EXTDESC "Mini NSCF Proxy Stub file Property handler"

HRESULT __stdcall DllRegisterServer(void) {
	LSTATUS status;

	
	TCHAR *dllPath;
	DWORD dllPathLen;//include the terminator, must be in octets

	TCHAR *guidStr;
	DWORD guidStrLen;//include the terminator, must be in octets

	{
		TCHAR *clsIdKeyName;
		HKEY clsIdKey;
		//HKEY_LOCAL_MACHINE\Software\Classes
		status=RegCreateKeyEx(HKEY_CLASSES_ROOT,clsIdKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&clsIdKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueStringLiteral(clsIdKey,"","",EXTDESC);
		RegSetKeyValue(clsIdKey,TEXT("InProcServer32"),NULL,REG_SZ,dllPath,dllPathLen);
		RegSetKeyValueStringLiteral(clsIdKey,"InProcServer32","ThreadingModel","Apartment");

		RegCloseKey(clsIdKey);
	}

	{
		TCHAR *clsTypeKeyName;
		HKEY clsTypeKey;

		status=RegCreateKeyEx(HKEY_CLASSES_ROOT,clsTypeKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&clsTypeKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueStringLiteral(clsTypeKey,"","","Mini NSCF Proxy Stub file");


		RegCloseKey(clsTypeKey);
	}

	{
		TCHAR *propHandlerKeyName=TEXT("Software/Microsoft/WindowsCurrentVersion/PropertySystem/PropertyHandlers");
		HKEY phKey;
		status=RegOpenKeyEx(HKEY_LOCAL_MACHINE,propHandlerKeyName,0,KEY_ALL_ACCESS,&phKey);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueString(phKey,FILEEXT,"",guidStr,guidStrLen);

		RegCloseKey(phKey);
	}

	{
		TCHAR *shextApprovedKeyName=TEXT("Software/Microsoft/WindowsCurrentVersion/Shell Extensions/Approved");
		HKEY shextApprovedKey;
		status=RegOpenKeyEx(HKEY_LOCAL_MACHINE,shextApprovedKeyName,0,KEY_ALL_ACCESS,&shextApprovedKey);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValue(shextApprovedKey,NULL,guidStr,REG_SZ, EXTDESC, tcharStrByteLen(TEXT(EXTDESC));

		RegCloseKey(shextApprovedKey);
	}

	{
		TCHAR *kindKeyName;
		HKEY kindKey;

		status=RegCreateKeyEx(HKEY_CLASSES_ROOT,kindKeyName,0,NULL,0,KEY_ALL_ACCESS,NULL,&kindKey,NULL);
		if(status!=ERROR_SUCCESS) {
			return HRESULT_FROM_WIN32(status);
		}

		RegSetKeyValueStringLiteral(kindKey,"",FILEEXT,"music");


		RegCloseKey(kindKey);
	}
	
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	return S_OK;
}

HRESULT __stdcall DllUnregisterServer(void) {

	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	return S_OK;
}