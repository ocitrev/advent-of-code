#include "main.hpp"
#ifdef _WIN32
#    include <crtdbg.h>
#endif

void Main();

#ifdef _WIN32
int wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t **argv)
{
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR); // NOLINT
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR); // NOLINT
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR); // NOLINT

    Main();
}
#else
int main()
{
    Main();
}
#endif
