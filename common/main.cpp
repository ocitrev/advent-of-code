#include "pch.h"
#include "main.hpp"
#include "input.hpp"
#include <crtdbg.h>

void Main();
static std::filesystem::path thisExePath;

int wmain([[maybe_unused]]int argc, wchar_t **argv)
{
    thisExePath = argv[0];

    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);

    Main();
}

std::filesystem::path const &GetExePath()
{
    return thisExePath;
}