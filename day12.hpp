#pragma once
#include <string_view>

namespace example
{
    static constexpr std::string_view one{R"(<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>)"};

    static constexpr std::string_view two{R"(<x=-8, y=-10, z=0>
<x=5, y=5, z=10>
<x=2, y=-7, z=3>
<x=9, y=-8, z=-3>)"};
}

namespace input
{
    static constexpr std::string_view data{R"(<x=-15, y=1, z=4>
<x=1, y=-10, z=-8>
<x=-5, y=4, z=9>
<x=4, y=6, z=-2>)"};
}
