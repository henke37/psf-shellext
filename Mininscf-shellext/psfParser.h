#ifndef PSFPARSER_H
#define PSFPARSER_H

#include <map>
#include <string>
#include <cstdint>
#include "common.h"

#define MIN_PCF_SIZE 16

class PsfParser {

public:
	PsfParser(IStream *);
	~PsfParser();
	
	HRESULT parse();

	std::map<std::string,std::string> tags;
	
	CHAR version;

private:
	IStream *stream;
	HRESULT parseTags();
	HRESULT saveTags();

	ULARGE_INTEGER tagStart;

	HRESULT readLong(uint32_t &out);

};

#endif