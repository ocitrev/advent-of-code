#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    constexpr std::string_view map1{R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)"};

    constexpr std::string_view map2{R"(dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc)"};

    constexpr std::string_view map3{R"(fs-end
he-DX
fs-he
start-DX
pj-DX
end-zg
zg-sl
zg-pj
pj-he
RW-he
fs-DX
pj-RW
zg-RW
start-pj
he-WI
zg-he
pj-fs
start-RW)"};
}
