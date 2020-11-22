#pragma once
#include <cstdio>

#if defined(_WIN32) && !defined(__GNUC__)
#    pragma comment(linker, "/include:__forceConsoleInit")
#endif

bool IsTerminal(std::FILE *stream);
