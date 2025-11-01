#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example
{
    using namespace std::literals;
    static constexpr auto triangle = "5 10 25"sv;

    static constexpr auto triangles2 = R"(101 301 501
102 302 502
103 303 503
201 401 601
202 402 602
203 403 603)"sv;
}
