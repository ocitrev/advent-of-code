#include "day7.hpp"

#include "../cpp-utils/assert.hpp"
#include "../cpp-utils/string.hpp"

#include <functional>
#include <map>
#include <print>
#include <string>
#include <variant>
#include <vector>

class Circuit
{
    std::map<std::string, std::function<unsigned short()>> wires;
    std::map<std::string, unsigned short> cache;

    std::function<unsigned short()> Resolve(std::string_view text)
    {
        if (std::isdigit(text[0]) != 0)
        {
            return [n = svtoi(text)]()
            {
                return static_cast<unsigned short>(n);
            };
        }

        return [this, n = text]()
        {
            return GetWire(n);
        };
    }

public:
    explicit Circuit(std::string_view instructions)
    {
        for (auto const &line : Split(instructions, '\n'))
        {
            auto parts = Split(line, ' ');
            std::size_t const nb = parts.size();

            if (nb == 3)
            {
                std::string out{parts.back()};
                auto const &in = parts.front();
                wires[out] = Resolve(in);
                // std::println("{} = {}", out, in);
            }
            else if (nb == 4)
            {
                auto const &op = parts[0];
                std::string out{parts.back()};
                if (op != "NOT")
                {
                    throw std::runtime_error("Unknown instruction");
                }

                // std::println("{} = NOT {}", out, parts[1]);
                wires[out] = [val = Resolve(parts[1])]()
                {
                    return static_cast<unsigned short>(~val());
                };
            }
            else if (nb == 5)
            {
                auto const &op = parts[1];
                std::string out{parts.back()};
                // std::println("{} = {} {} {}", out, parts[0], parts[1], parts[2]);

                if (op == "AND")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]() -> unsigned short
                    {
                        return left() & right();
                    };
                }
                else if (op == "OR")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]() -> unsigned short
                    {
                        return left() | right();
                    };
                }
                else if (op == "LSHIFT")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]()
                    {
                        return static_cast<unsigned short>(left() << right());
                    };
                }
                else if (op == "RSHIFT")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]()
                    {
                        return static_cast<unsigned short>(left() >> right());
                    };
                }
                else
                {
                    throw std::runtime_error("Unknown instruction");
                }
            }
        }
    }

    [[nodiscard]] unsigned short GetWire(std::string_view name)
    {
        std::string key{name};
        if (auto itCache = cache.find(key); itCache != end(cache))
        {
            return itCache->second;
        }

        auto iter = wires.find(key);

        if (iter == end(wires))
        {
            throw std::runtime_error("Invalid wire");
        }

        unsigned short const value = iter->second();
        // std::println("{} = {}", name, value);
        cache[key] = value;
        return value;
    }

    void SetWire(std::string const &name, unsigned short value)
    {
        cache[name] = value;
    }

    void Reset()
    {
        cache.clear();
    }
};

static int Part1()
{
    Circuit c(GetInput());
    return c.GetWire("a");
}

static int Part2()
{
    Circuit c(GetInput());
    auto const a = c.GetWire("a");
    c.Reset();
    c.SetWire("b", a);
    return c.GetWire("a");
}

int main()
{
    // https://adventofcode.com/2015/day/7
    std::println("Day 7, 2015: Some Assembly Required");

#ifndef NDEBUG
    Circuit c{input::example};
    Assert(72 == c.GetWire("d"));
    Assert(507 == c.GetWire("e"));
    Assert(492 == c.GetWire("f"));
    Assert(114 == c.GetWire("g"));
    Assert(65'412 == c.GetWire("h"));
    Assert(65'079 == c.GetWire("i"));
    Assert(123 == c.GetWire("x"));
    Assert(456 == c.GetWire("y"));
#endif

    int const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(46'065 == part1);

    int const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(14'134 == part2);
}
