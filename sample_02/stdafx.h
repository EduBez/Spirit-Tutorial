// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

#if 0
    #define BOOST_SPIRIT_DEBUG
#endif

#include <boost/spirit/core.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/tree/tree_to_xml.hpp>
#include <boost/spirit/utility.hpp>
