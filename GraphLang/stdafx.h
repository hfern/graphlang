// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <istream>
#include <string>
#include <tuple>


namespace GraphLang
{
	namespace Tokenizer
	{
		using TokenInput = std::istream;
		using TokenParseResult = std::tuple<bool, std::string>;
	};
};



// TODO: reference additional headers your program requires here
