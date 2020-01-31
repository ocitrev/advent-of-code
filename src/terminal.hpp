#pragma once
#include <cstdio>

#ifdef _WIN32
#    pragma comment(linker, "/include:__forceConsoleInit")
#endif

bool IsTerminal(std::FILE *stream);
