#include "main.hpp"
#ifdef _WIN32
#    include <Windows.h>
#    include <crtdbg.h>

static int ConsoleInit()
{
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR); // NOLINT
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR); // NOLINT
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR); // NOLINT
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}

static int consoleInit = ConsoleInit();

extern "C"
{
    int __forceConsoleInit;
}

#endif
