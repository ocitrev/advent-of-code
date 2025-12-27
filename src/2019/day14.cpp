#include "day14.hpp"

#include "../cpp-utils/assert.hpp"
#include "../cpp-utils/string.hpp"
#include "../cpp-utils/terminal.hpp"

#include <map>
#include <print>

struct Elem
{
    std::uint64_t count = 0;
    std::string name;

    static Elem Parse(std::string_view data)
    {
        Elem el;
        auto pos = data.find(' ');
        el.count = std::stoull(std::string{data.substr(0, pos)});
        el.name = data.substr(pos + 1);
        trim(el.name);
        return el;
    }
};

struct NanoFactory
{
    std::map<std::string, std::pair<Elem, std::vector<Elem>>> reactions;
    std::map<std::string, std::uint64_t> storage;

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

    bool Produce(std::string const &name, std::uint64_t count = 1)
    {
        if (storage[name] >= count)
        {
            return true;
        }

        if (name == "ORE")
        {
            return false;
        }

        auto const &[result, composants] = reactions[name];
        auto n = static_cast<std::uint64_t>(
            ceil((static_cast<double>(count) - static_cast<double>(storage[name])) / static_cast<double>(result.count)));
#if 0
        std::println("{} = {} - {} / {}", n, count, storage[name], result.count);
#endif

        bool producing = true;

        for (auto const &c : composants)
        {
            producing &= Produce(c.name, n * c.count);
            storage[c.name] -= std::min(storage[c.name], n * c.count);
        }

        if (producing)
        {
            storage[name] += n * result.count;
            return true;
        }

        return false;
    }

    std::uint64_t ComputeOre()
    {
        std::uint64_t low = 0;
        std::uint64_t high = std::numeric_limits<std::uint64_t>::max();

        while (low < high)
        {
            std::uint64_t pivot = low + (high - low) / 2;
            storage.clear();
            storage["ORE"] = pivot;

            if (Produce("FUEL"))
            {
                high = pivot;
            }
            else
            {
                low = pivot + 1;
            }
        }

        return low;
    }

    std::uint64_t ComputeFuel(std::uint64_t totalOre = 1'000'000'000'000)
    {
        std::uint64_t low = 0;
        std::uint64_t high = totalOre;

        while (low < high - 1)
        {
            std::uint64_t pivot = low + (high - low) / 2;
            storage.clear();
            storage["ORE"] = totalOre;

            if (Produce("FUEL", pivot))
            {
                low = pivot;
            }
            else
            {
                high = pivot - 1;
            }
        }

        if (Produce("FUEL", high))
        {
            return high;
        }

        return low;
    }
};

int main()
{
    std::println("Day 14, 2019: Space Stoichiometry");

    Assert(31 == NanoFactory::Parse(example::ex1).ComputeOre());
    Assert(165 == NanoFactory::Parse(example::ex2).ComputeOre());
    Assert(13'312 == NanoFactory::Parse(example::ex3).ComputeOre());
    Assert(180'697 == NanoFactory::Parse(example::ex4).ComputeOre());
    Assert(2'210'736 == NanoFactory::Parse(example::ex5).ComputeOre());

    Assert(82'892'753 == NanoFactory::Parse(example::ex3).ComputeFuel());
    Assert(5'586'022 == NanoFactory::Parse(example::ex4).ComputeFuel());
    Assert(460'664 == NanoFactory::Parse(example::ex5).ComputeFuel());

    auto const part1 = NanoFactory::Parse(GetInput()).ComputeOre();
    std::println("  Part1: {}", part1);
    Assert(1'967'319 == part1);

    auto const part2 = NanoFactory::Parse(GetInput()).ComputeFuel();
    std::println("  Part2: {}", part2);
    Assert(1'122'036 == part2);
}
