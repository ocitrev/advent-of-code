#include "day07.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

class Circuit
{
    std::map<std::string, std::function<unsigned short()>> wires;
    std::map<std::string, unsigned short> cache;

    std::function<unsigned short()> Resolve(std::string const &text)
    {
        if (std::isdigit(text[0]) != 0)
        {
            return [n = std::stoi(text)]() {
                return n;
            };
        }

        return [this, n = text]() {
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
                auto const &out = parts.back();
                auto const &in = parts.front();
                wires[out] = Resolve(in);
                //fmt::print("{} = {}\n", out, in);
            }
            else if (nb == 4)
            {
                auto const &op = parts[0];
                auto const &out = parts.back();
                if (op != "NOT")
                    throw std::runtime_error("Unknown instruction");

                //fmt::print("{} = NOT {}\n", out, parts[1]);
                wires[out] = [val = Resolve(parts[1])]() {
                    return ~val();
                };
            }
            else if (nb == 5)
            {
                auto const &op = parts[1];
                auto const &out = parts.back();
                //fmt::print("{} = {} {} {}\n", out, parts[0], parts[1], parts[2]);

                if (op == "AND")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]() {
                        return left() & right();
                    };
                }
                else if (op == "OR")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]() {
                        return left() | right();
                    };
                }
                else if (op == "LSHIFT")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]() {
                        return left() << right();
                    };
                }
                else if (op == "RSHIFT")
                {
                    wires[out] = [left = Resolve(parts[0]), right = Resolve(parts[2])]() {
                        return left() >> right();
                    };
                }
                else
                {
                    throw std::runtime_error("Unknown instruction");
                }
            }
        }
    }

    [[nodiscard]] unsigned short GetWire(std::string const &name)
    {
        if (auto itCache = cache.find(name); itCache != end(cache))
            return itCache->second;

        auto iter = wires.find(name);

        if (iter == end(wires))
            throw std::runtime_error("Invalid wire");

        unsigned short const value = iter->second();
        //fmt::print("{} = {}\n", name, value);
        cache[name] = value;
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

int Part1()
{
    Circuit c(input::data);
    return c.GetWire("a");
}

int Part2()
{
    Circuit c(input::data);
    auto const a = c.GetWire("a");
    c.Reset();
    c.SetWire("b", a);
    return c.GetWire("a");
}

int main()
{
    // https://adventofcode.com/2015/day/7
    fmt::print("Day 7: Some Assembly Required\n");

#ifndef NDEBUG
    Circuit c{input::example};
    assert(72 == c.GetWire("d"));
    Assert(507 == c.GetWire("e"));
    Assert(492 == c.GetWire("f"));
    Assert(114 == c.GetWire("g"));
    Assert(65412 == c.GetWire("h"));
    Assert(65079 == c.GetWire("i"));
    Assert(123 == c.GetWire("x"));
    Assert(456 == c.GetWire("y"));
#endif

    fmt::print("  Part 1: {}\n", Part1());
    fmt::print("  Part 2: {}\n", Part2());
}