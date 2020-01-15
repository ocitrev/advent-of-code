#include "day12.hpp"
#include "common/main.hpp"
#include "common/string.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <regex>

class Moon
{
public:
    std::array<Int, 3> pos{};
    std::array<Int, 3> vel{};

    explicit Moon(std::string const &data)
    {
        std::regex re("<x=(-?\\d+), y=(-?\\d+), z=(-?\\d+)>");
        std::smatch m;

        if (std::regex_match(data, m, re) == false)
            throw std::invalid_argument("Invalid data");

        pos[0] = std::stoi(m[1]);
        pos[1] = std::stoi(m[2]);
        pos[2] = std::stoi(m[3]);
    }

    static std::vector<Moon> ReadMoons(std::string_view data)
    {
        std::vector<Moon> moons;

        for (auto line : Split(data, '\n'))
        {
            moons.emplace_back(line);
        }

        return moons;
    }

    friend std::ostream &operator<<(std::ostream &out, Moon const &m)
    {
        return out << "pos{" << m.pos[0] << ", " << m.pos[1] << ", " << m.pos[2] << "}, vel{"
                   << m.vel[0] << ", " << m.vel[1] << ", " << m.vel[2] << "}";
    }

    bool operator==(Moon const &other) const
    {
        return pos == other.pos && vel == other.vel;
    }

    void ApplyGravityAxis(Moon &other, size_t axis)
    {
        if (pos[axis] < other.pos[axis])
        {
            ++vel[axis];
            --other.vel[axis];
        }
        else if (pos[axis] > other.pos[axis])
        {
            --vel[axis];
            ++other.vel[axis];
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
        pos[axis] += vel[axis];
    }

    void Accelerate()
    {
        AccelerateAxis(0);
        AccelerateAxis(1);
        AccelerateAxis(2);
    }

    Int GetEnergy() const
    {
        Int pot = abs(pos[0]) + abs(pos[1]) + abs(pos[2]);
        Int kin = abs(vel[0]) + abs(vel[1]) + abs(vel[2]);
        return pot * kin;
    }
};

template <typename ContainerT, typename CallableT>
static void ForEachCombinations(ContainerT &&cont, CallableT &&callable)
{
    auto first = begin(cont);
    auto last = end(cont);

    for (auto iterOuter = first; iterOuter != last; ++iterOuter)
    {
        for (auto iterInner = std::next(iterOuter); iterInner != last; ++iterInner)
        {
            callable(*iterOuter, *iterInner);
        }
    }
}

void StepAxis(std::vector<Moon> &moons, size_t axis)
{
    ForEachCombinations(moons, [axis](Moon &a, Moon &b)
    {
        a.ApplyGravityAxis(b, axis);
    });

    for (Moon &m : moons)
    {
        m.AccelerateAxis(axis);
    }
}

void Step(std::vector<Moon> &moons)
{
    ForEachCombinations(moons, [](Moon &a, Moon &b) {
        a.ApplyGravity(b);
    });

    for (Moon &m : moons)
    {
        m.Accelerate();
    }
}

Int Simulate(std::string_view data, int steps)
{
    std::vector moons = Moon::ReadMoons(data);

    for (int i = 0; i != steps; ++i)
    {
        Step(moons);
    }

    return std::accumulate(begin(moons), end(moons), Int{0}, [](Int total, Moon const &m) {
        return total + m.GetEnergy();
    });
}

Int CountCycle(std::string_view data)
{
    std::vector moons = Moon::ReadMoons(data);
    std::array<Int, 3> count{};

    for (size_t axis = 0; axis != 3; ++axis)
    {
        std::vector<Moon> copies = moons;

        while (true)
        {
            StepAxis(copies, axis);
            ++count[axis];

            if (copies == moons)
                break;
        }
    }

    return std::lcm(std::lcm(count[0], count[1]), count[2]);
}

void Main()
{
    std::cout << "Day 12: The N-Body Problem\n";

    assert(179 == Simulate(example::one, 10));
    assert(1940 == Simulate(example::two, 100));

    std::cout << "Part1: " << Simulate(input::data, 1000) << '\n';

    assert(2772 == CountCycle(example::one));
    assert(4686774924 == CountCycle(example::two));

    std::cout << "Part2: " << CountCycle(input::data) << '\n';
}
