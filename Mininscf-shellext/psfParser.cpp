#include "common.h"
#include "psfParser.h"

#define retIfFail { if(!SUCCEEDED(hresult)) return hresult; }
#define retIfNonOk if(hresult==S_FALSE) { return E_FAIL; } else retIfFail

PsfParser::PsfParser(IStream *_stream) : stream(_stream) {
	stream->AddRef();
}

PsfParser::~PsfParser() {
	stream->Release();
}

HRESULT PsfParser::readLong(uint32_t &out) {
	HRESULT hresult;	
	ULONG bytesRead;
	CHAR readBuf[4];
	hresult=stream->Read(&version,1,&bytesRead);
	retIfNonOk;

	out=readBuf[0] | readBuf[1]<<8 | readBuf[2]<<16 | readBuf[3] <<24;

	return S_OK;
}



HRESULT PsfParser::parse() {
	STATSTG streamStat;
	ULONG bytesRead;
	HRESULT hresult;

	hresult=stream->Stat(&streamStat,STATFLAG_NONAME);
	retIfFail;

	if(streamStat.cbSize.QuadPart<MIN_PCF_SIZE) {
		return E_INVALIDARG;
	}

	LARGE_INTEGER seekPos;
	seekPos.QuadPart=0;
	hresult=stream->Seek(seekPos,STREAM_SEEK_SET,nullptr);
	retIfFail;

	CHAR fileSig[3];
	hresult=stream->Read(fileSig,3,&bytesRead);
	retIfNonOk;

	if(strcmp(fileSig,"PSF")!=0) return E_INVALIDARG;

	hresult=stream->Read(&version,1,&bytesRead);
	retIfNonOk;

	uint32_t reservatedLength,compressedLength,compressedCrc;

	hresult=readLong(reservatedLength);
	retIfNonOk;
	hresult=readLong(compressedLength);
	retIfNonOk;
	hresult=readLong(compressedCrc);
	retIfNonOk;

	seekPos.QuadPart=reservatedLength+compressedLength;
	hresult=stream->Seek(seekPos,STREAM_SEEK_CUR,nullptr);
	retIfFail;

	CHAR tagSig[5];
	hresult=stream->Read(tagSig,5,&bytesRead);
	retIfFail;
	if(hresult==S_FALSE) return S_FALSE;

	if(strcmp(tagSig,"[TAG]")!=0) return E_INVALIDARG;

	seekPos.QuadPart=0;
	hresult=stream->Seek(seekPos,STREAM_SEEK_CUR,&tagStart);
	retIfFail;

	return parseTags();
}

#define ReadBuffLen 256

HRESULT PsfParser::parseTags() {
	HRESULT hresult;
	bool inName=true;
	std::string name;
	std::string value;
	do {
		CHAR readBuff[ReadBuffLen];
		ULONG bytesRead;
		hresult=stream->Read(readBuff,ReadBuffLen,&bytesRead);
		retIfFail;

		for(ULONG i=0;i<bytesRead;++i) {
			CHAR &c=readBuff[i];

			if(c=='=') {
				inName=false;
			} else if(c=='\n') {
				inName=true;

				auto existingItr=tags.find(name);
				if(existingItr!=tags.end()) {
					existingItr->second+="\n";
					existingItr->second+=value;
				} else {
					tags.insert(std::make_pair(name,value));
				}
				name.clear();
				value.clear();
			} else if(inName) {
				name+=c;
			} else {
				value+=c;
			}

		}
	} while(hresult==S_OK);

	return S_OK;
}
