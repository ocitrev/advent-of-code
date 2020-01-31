#include "terminal.hpp"

#ifdef _WIN32
#    include <Windows.h>
#    include <crtdbg.h>
#    include <cstdio>
#    include <cstring>
#    include <io.h>
#    include <type_traits>

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

#    ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#        define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#    endif

static bool EnableVTMode(HANDLE hStdHandle)
{
    // NOLINTNEXTLINE: cppcoreguidelines-pro-type-cstyle-cast
    if (hStdHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD fType = GetFileType(hStdHandle);

    // check if handle is a TTY
    if ((fType & FILE_TYPE_CHAR) == 0)
    {
        return false;
    }

    DWORD consoleMode;

    if (GetConsoleMode(hStdHandle, &consoleMode) != FALSE)
    {
        if ((consoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0)
        {
            // already enabled
            return true;
        }

        return SetConsoleMode(hStdHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)
               != FALSE;
    }

    return false;
}

template <class To, class From>
constexpr To bit_cast(const From &src) noexcept
{
    static_assert(sizeof(To) == sizeof(From));
    static_assert(std::is_trivially_copyable_v<From>);
    static_assert(std::is_trivial_v<To>,
                  "this implementation requires that To is trivially default constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

bool IsTerminal(std::FILE *stream)
{
    return EnableVTMode(bit_cast<HANDLE>(_get_osfhandle(_fileno(stream))));
}

#else

#    include <unistd.h>

bool IsTerminal(std::FILE *stream)
{
    return _isatty(_fileno(stream)) != 0;
}

#endif
