#include "day12.hpp"

#include "../common/assert.hpp"
#include "../common/combinations.hpp"
#include "../common/string.hpp"
#include "../common/terminal.hpp"

#include <array>
#include <numeric>
#include <print>
#include <regex>

class Moon
{
public:
    std::array<long long, 3> pos{};
    std::array<long long, 3> vel{};

    explicit Moon(std::string const &data)
    {
        std::regex re(R"(<x=(-?\d+), y=(-?\d+), z=(-?\d+)>)");
        std::smatch m;

        if (!std::regex_match(data, m, re))
        {
            throw std::invalid_argument("Invalid data");
        }

        pos[0] = std::stoi(m[1]);
        pos[1] = std::stoi(m[2]);
        pos[2] = std::stoi(m[3]);
    }

    static std::vector<Moon> ReadMoons(std::string_view data)
    {
        std::vector<Moon> moons;

        for (auto const &line : Split(data, '\n'))
        {
            moons.emplace_back(std::string{line});
        }

        return moons;
    }

    bool operator==(Moon const &other) const
    {
        return pos == other.pos && vel == other.vel;
    }

    void ApplyGravityAxis(Moon &other, size_t axis)
    {
        if (pos.at(axis) < other.pos.at(axis))
        {
            ++vel.at(axis);
            --other.vel.at(axis);
        }
        else if (pos.at(axis) > other.pos.at(axis))
        {
            --vel.at(axis);
            ++other.vel.at(axis);
        }
    }

    void ApplyGravity(Moon &other)
    {
        ApplyGravityAxis(other, 0);
        ApplyGravityAxis(other, 1);
        ApplyGravityAxis(other, 2);
    }

    void AccelerateAxis(size_t axis)
    {
        pos.at(axis) += vel.at(axis);
    }

    void Accelerate()
    {
        AccelerateAxis(0);
        AccelerateAxis(1);
        AccelerateAxis(2);
    }

    [[nodiscard]] long long GetEnergy() const
    {
        long long pot = std::abs(pos[0]) + std::abs(pos[1]) + std::abs(pos[2]);
        long long kin = std::abs(vel[0]) + std::abs(vel[1]) + std::abs(vel[2]);
        return pot * kin;
    }
};

static void StepAxis(std::vector<Moon> &moons, size_t axis)
{
    ForEachCombinations(moons,
        [axis](Moon &a, Moon &b)
        {
            a.ApplyGravityAxis(b, axis);
        });

    for (Moon &m : moons)
    {
        m.AccelerateAxis(axis);
    }
}

static void Step(std::vector<Moon> &moons)
{
    ForEachCombinations(moons,
        [](Moon &a, Moon &b)
        {
            a.ApplyGravity(b);
        });

    for (Moon &m : moons)
    {
        m.Accelerate();
    }
}

static long long Simulate(std::string_view data, int steps)
{
    std::vector moons = Moon::ReadMoons(data);

    for (int i = 0; i != steps; ++i)
    {
        Step(moons);
    }

    return std::accumulate(begin(moons), end(moons), 0LL,
        [](long long total, Moon const &m)
        {
            return total + m.GetEnergy();
        });
}

static long long CountCycle(std::string_view data)
{
    std::vector const moons = Moon::ReadMoons(data);
    std::array<long long, 3> count{};

    for (size_t axis = 0; axis != 3; ++axis)
    {
        std::vector<Moon> copies = moons;

        while (true)
        {
            StepAxis(copies, axis);
            ++count.at(axis);

            if (copies == moons)
            {
                break;
            }
        }
    }

    return std::lcm(std::lcm(count[0], count[1]), count[2]);
}

int main()
{
    std::println("Day 12, 2019: The N-Body Problem");

    Assert(179 == Simulate(example::one, 10));
    Assert(1940 == Simulate(example::two, 100));

    auto part1 = Simulate(GetInput(), 1000);
    std::println("  Part1: {}", part1);
    Assert(8625 == part1);

    Assert(2772 == CountCycle(example::one));
    Assert(4'686'774'924 == CountCycle(example::two));

    auto part2 = CountCycle(GetInput());
    std::println("  Part2: {}", part2);
    Assert(332'477'126'821'644 == part2);
}
