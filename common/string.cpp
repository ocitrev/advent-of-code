#include "string.hpp"
#include <fcntl.h>
#include <io.h>

std::vector<std::string> Split(std::string_view text, char const sep)
{
    std::vector<std::string> result;

    auto first = begin(text);
    auto iter = first;
    auto const last = end(text);

    for (; iter != last; ++iter)
    {
        if (*iter != sep)
            continue;

        result.emplace_back(first, iter);
        first = iter + 1;
    }

    result.emplace_back(first, iter);

    return result;
}

struct UnicodeScope
{
    int old;

    UnicodeScope()
    {
        old = _setmode(_fileno(stdout), _O_U16TEXT);
    }

    ~UnicodeScope()
    {
        (void)_setmode(_fileno(stdout), old);
    }
};

void PrintUnicode(std::wstring_view text)
{
    UnicodeScope _;
    wprintf(text.data());
}
