#pragma once
#include <cstdio>

#if defined(_WIN32) && !defined(__GNUC__)
#pragma comment(linker, "/include:__forceConsoleInit")
#endif

bool IsTerminal(std::FILE *stream);

// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
#define CSI(x) "\x1b[" x // NOLINT: cppcoreguidelines-macro-usage
