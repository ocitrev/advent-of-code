#include "day14.hpp"
#include "assert.hpp"
#include "string.hpp"
#include "terminal.hpp"
#include <fmt/format.h>
#include <map>

struct Elem
{
    std::int64_t count = 0;
    std::string name;

    static Elem Parse(std::string_view data)
    {
        Elem el;
        auto pos = data.find(' ');
        el.count = std::stoi(std::string{data.substr(0, pos)});
        el.name = data.substr(pos + 1);
        trim(el.name);
        return el;
    }
};

struct NanoFactory
{
    std::map<std::string, std::pair<Elem, std::vector<Elem>>> reactions;
    std::map<std::string, std::int64_t> storage;

    static NanoFactory Parse(std::string_view data)
    {
        NanoFactory f;

        for (auto const &line : Split(data, '\n'))
        {
            auto pos = line.find("=>");
            auto left = line.substr(0, pos);
            trim(left);

            std::vector<Elem> composants;

            for (auto &c : Split(left, ','))
            {
                trim(c);
                composants.push_back(Elem::Parse(c));
            }

            auto right = line.substr(pos + 2);
            trim(right);
            auto result = Elem::Parse(right);

            f.reactions[result.name] = std::make_pair(result, std::move(composants));
        }

        return f;
    }

    bool Produce(std::string const &name, std::int64_t count = 1)
    {
        if (storage[name] >= count)
            return true;

        if (name == "ORE")
            return false;

        auto const &[result, composants] = reactions[name];
        auto n = static_cast<std::int64_t>(
            ceil((static_cast<double>(count) - static_cast<double>(storage[name]))
                 / static_cast<double>(result.count)));
#if 0
        fmt::print("{} = {} - {} / {}\n", n, count, storage[name], result.count);
#endif

        bool producing = true;

        for (auto const &c : composants)
        {
            producing &= Produce(c.name, n * c.count);
            storage[c.name] -= n * c.count;
        }

        if (producing)
        {
            storage[name] += n * result.count;
            return true;
        }

        return false;
    }

    std::int64_t ComputeOre()
    {
        std::int64_t low = 0;
        std::int64_t high = std::numeric_limits<std::int64_t>::max();

        while (low < high)
        {
            std::int64_t pivot = low + (high - low) / 2;
            storage.clear();
            storage["ORE"] = pivot;

            if (Produce("FUEL"))
                high = pivot;
            else
                low = pivot + 1;
        }

        return low;
    }

    std::int64_t ComputeFuel(std::int64_t totalOre = 1000000000000)
    {
        std::int64_t low = 0;
        std::int64_t high = totalOre;

        while (low < high - 1)
        {
            std::int64_t pivot = low + (high - low) / 2;
            storage.clear();
            storage["ORE"] = totalOre;

            if (Produce("FUEL", pivot))
                low = pivot;
            else
                high = pivot - 1;
        }

        if (Produce("FUEL", high))
            return high;

        return low;
    }
};

int main()
{
    fmt::print("Day 14: Space Stoichiometry\n");

    Assert(31 == NanoFactory::Parse(example::ex1).ComputeOre());
    Assert(165 == NanoFactory::Parse(example::ex2).ComputeOre());
    Assert(13312 == NanoFactory::Parse(example::ex3).ComputeOre());
    Assert(180697 == NanoFactory::Parse(example::ex4).ComputeOre());
    Assert(2210736 == NanoFactory::Parse(example::ex5).ComputeOre());

    Assert(82892753 == NanoFactory::Parse(example::ex3).ComputeFuel());
    Assert(5586022 == NanoFactory::Parse(example::ex4).ComputeFuel());
    Assert(460664 == NanoFactory::Parse(example::ex5).ComputeFuel());

    auto part1 = NanoFactory::Parse(input::data).ComputeOre();
    fmt::print("  Part1: {}\n", part1);
    Assert(1967319 == part1);

    auto part2 = NanoFactory::Parse(input::data).ComputeFuel();
    fmt::print("  Part2: {}\n", part2);
    Assert(1122036 == part2);
}
