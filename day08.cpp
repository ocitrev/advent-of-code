#include "day08.hpp"
#include "common/main.hpp"
#include "common/string.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

struct Layer
{
    std::string_view pixels;
    std::array<int, 3> count{};

    Layer(std::string_view pixels)
        : pixels(pixels)
    {
        for (auto const &p : pixels)
        {
            assert(p == '0' || p == '1' || p == '2');
            ++count[static_cast<size_t>(p) - '0'];
        }
    }
};

static std::vector<Layer> DecodeLayers(std::string_view pixels, size_t w, size_t h)
{
    std::vector<Layer> ret;
    auto const *last = pixels.data() + pixels.size();
    size_t chunkSize = w * h;

    for (auto iter = pixels.data(); iter < last; iter += chunkSize)
    {
        ret.emplace_back(std::string_view{iter, chunkSize});
    }

    return ret;
}

static std::string Render(std::string_view pixels, size_t w, size_t h)
{
    auto layers = DecodeLayers(pixels, w, h);
    auto p = layers.front().pixels;
    std::string canvas{p.data(), p.size()};

    {
        auto last = end(layers);

        for (auto iter = begin(layers) + 1; iter != last; ++iter)
        {
            std::transform(begin(iter->pixels), end(iter->pixels), begin(canvas), begin(canvas),
                           [](auto source, auto dest) {
                               if (dest == '2')
                                   return source;
                               return dest;
                           });
        }
    }

    return canvas;
}

static constexpr size_t W = 25;
static constexpr size_t H = 6;

void Part1()
{
    auto layers = DecodeLayers(input::data, W, H);

    auto iterMin = std::min_element(begin(layers), end(layers), [](Layer const &a, Layer const &b) {
        return a.count[0] < b.count[0];
    });

    assert(iterMin != end(layers));
    std::cout << "  Part1: " << iterMin->count[1] * iterMin->count[2] << '\n';
}

void Part2()
{
    auto const canvas = Render(input::data, W, H);

    std::wstring result;
    result.reserve(canvas.size());
    auto iter = cbegin(canvas);

    for (size_t y = 0; y != H; ++y)
    {
        for (size_t x = 0; x != W; ++x, ++iter)
        {
            if (*iter == L'0')
                result.append(1, L' ');
            else if (*iter == L'1')
                result.append(1, L'█');
        }

        result.append(1, L'\n');
    }

    using namespace std::string_view_literals;
    result.insert(0, L"  Part2:\n"sv);
    PrintUnicode(result);
}

void Main()
{
    std::cout << "Day 8: Space Image Format\n";
    assert("0110" == Render("0222112222120000", 2, 2));

    Part1();
    Part2();
}
