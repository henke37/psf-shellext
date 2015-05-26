#include "common.h"
#include "psfParser.h"
#include <cstring>

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

	//TODO: deal with the case of no tags at all in the existing file

	do {
		CHAR readBuff[ReadBuffLen];
		ULONG bytesRead;
		hresult=stream->Read(readBuff,ReadBuffLen,&bytesRead);
		retIfFail;

		for(ULONG i=0;i<bytesRead;++i) {
			CHAR &c=readBuff[i];

			if(c=='=') {
				//TODO: trim whitespace
				inName=false;
			} else if(c=='\r') {
			} else if(c=='\n') {
				inName=true;

				//TODO: trim whitespace

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

HRESULT PsfParser::saveTags() {
	HRESULT hresult;
	ULONG bytesWritten;

	LARGE_INTEGER seekPos;
	seekPos.QuadPart=tagStart.QuadPart;
	hresult=stream->Seek(seekPos,STREAM_SEEK_SET,nullptr);
	retIfFail;

	for(auto &kv: tags) {
		auto &key=kv.first;
		auto &value=kv.second;

		const char *keyBuff=key.c_str();
		const char *valueBuff=value.c_str();

		//such a mess for newline values

		const char *lineStart=keyBuff;
		for(;;) {
			const char *lineEnd=std::strstr(lineStart,"\n");
			bool lastLine= !lineEnd;
			int lineLen;
			if(lastLine) {
				lineLen=std::strlen(lineStart);
				lineEnd=lineStart+lineLen;
			} else {
				lineLen=lineStart-lineEnd;
			}

			hresult=stream->Write(keyBuff,key.length(),&bytesWritten);
			retIfFail;

			char eq[]={'='};
			hresult=stream->Write(eq,1,&bytesWritten);
			retIfFail;

			stream->Write(lineStart,lineLen,&bytesWritten);
			retIfFail;

			char nl[]={'\n'};
			hresult=stream->Write(nl,1,&bytesWritten);
			retIfFail;

			if(lastLine) {
				break;
			}

			lineStart=lineEnd+1;
		}
	}

	return S_OK;
}
