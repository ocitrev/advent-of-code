#include "day15.hpp"
#include "../common.hpp"
#include "../common/point3d.hpp"

template <typename KeyT, typename ValueT>
struct DictWithDefault
{
    std::unordered_map<KeyT, ValueT> dict;
    ValueT defaultValue;

    explicit DictWithDefault(ValueT defaultValue_)
        : defaultValue(std::move(defaultValue_))
    {
    }

    void Set(KeyT const &key, ValueT value)
    {
        dict[key] = std::move(value);
    }

    ValueT Get(KeyT const &key) const
    {
        if (auto iter = dict.find(key); iter != end(dict))
            return iter->second;

        return defaultValue;
    }
};

struct Map
{
    std::unordered_map<Point2d, int> map;
    int width = 0;
    int height = 0;

    void Parse(std::string_view text)
    {
        int y = 0;

        for (auto line : Split(text, '\n'))
        {
            int x = 0;

            for (char c : line)
            {
                map[{x, y}] = c - '0';
                ++x;
            }

            width = x;
            ++y;
        }

        height = y;
    }

    struct Score
    {
        double value = std::numeric_limits<double>::infinity();

        Score() = default;
        Score(double d)
            : value(d)
        {
        }

        operator double() const
        {
            return value;
        }
    };

    static std::array<Point2d, 4> Neighbors(Point2d current)
    {
        return {current + Point2d::WEST, current + Point2d::NORTH, current + Point2d::EAST, current + Point2d::SOUTH};
    }

    double Weight([[maybe_unused]] Point2d const &from, Point2d to) const
    {
        int const offset = to.x / width + to.y / height;
        to.x %= width;
        to.y %= height;
        auto iterTo = map.find(to);

        if (iterTo == end(map))
            return std::numeric_limits<double>::infinity();

        return LimitScore(iterTo->second + offset);
    }

    static bool Inbound(Point2d const &p, Point2d const &start, Point2d const &goal)
    {
        return p.x >= start.x && p.y >= start.y && p.x <= goal.x && p.y <= goal.y;
    }

    std::vector<Point2d> Dijkstra(Point2d start, Point2d goal)
    {
        DictWithDefault<Point2d, double> dist{std::numeric_limits<double>::infinity()};
        std::unordered_map<Point2d, Point2d> prev;
        std::vector<std::tuple<Point2d, double>> queue;

        auto updatePriority = [&queue](Point2d const &p, double c)
        {
            std::erase_if(queue,
                [&p](auto const &t)
                {
                    return get<Point2d>(t) == p;
                });

            auto t = std::make_tuple(p, c);
            auto iter = std::lower_bound(begin(queue), end(queue), t,
                [](std::tuple<Point2d, double> const &a, std::tuple<Point2d, double> const &b)
                {
                    return get<double>(b) < get<double>(a);
                });

            queue.insert(iter, t);
        };

        for (int y = start.y; y <= goal.y; ++y)
        {
            for (int x = start.x; x <= goal.x; ++x)
            {
                Point2d const p{x, y};

                if (p == start)
                    continue;

                queue.push_back(std::make_tuple(p, std::numeric_limits<double>::infinity()));
            }
        }

        dist.Set(start, 0);
        queue.push_back(std::make_tuple(start, 0.0));

        while (not queue.empty())
        {
            auto const &[current, d] = queue.back();
            queue.pop_back();

            if (current == goal)
            {
                std::vector<Point2d> path;
                path.push_back(goal);

                for (auto iter = prev.find(goal); iter != end(prev);)
                {
                    path.push_back(iter->second);
                    iter = prev.find(iter->second);
                }

                return path;
            }

            for (auto neighbor : Neighbors(current))
            {
                if (not Inbound(neighbor, start, goal))
                    continue;

                if (auto const alt = dist.Get(current) + Weight(current, neighbor); alt < dist.Get(neighbor))
                {
                    dist.Set(neighbor, alt);
                    prev[neighbor] = current;
                    updatePriority(neighbor, alt);
                }
            }
        }

        return {};
    }

    static int LimitScore(int score)
    {
        return ((score - 1) % 9) + 1;
    }

    int GetRiskScore(std::vector<Point2d> const &path) const
    {
        if (path.empty())
            return 0;

        return std::accumulate(begin(path), std::prev(end(path)), 0,
            [this](int total, Point2d p)
            {
                int const offset = p.x / width + p.y / height;
                p.x %= width;
                p.y %= height;

                if (auto iter = map.find(p); iter != end(map))
                    return total + LimitScore(iter->second + offset);

                return total;
            });
    }

    [[maybe_unused]] void Print(std::vector<Point2d> const &path) const
    {
        double total = 0;
        std::string output;
        for (int y = 0; y < height * 5; ++y)
        {
            for (int x = 0; x < width * 5; ++x)
            {
                Point2d const p{x, y};
                auto const w = Weight({}, p);
                char const c = static_cast<char>(w) + '0';

                if (std::find(begin(path), end(path), p) != end(path))
                {
                    output.append(fmt::format("\033[37m{}\033[m", c));
                    total += w;
                }
                else
                    output.append(fmt::format("\033[30;1m{}\033[m", c));
            }

            output.push_back('\n');
        }

        fmt::print("{}\nScore: {}\n", output, total);
    }
};

static int Part1(std::string_view text)
{
    Map m;
    m.Parse(text);
    auto const path = m.Dijkstra({0, 0}, {m.width - 1, m.height - 1});
    return m.GetRiskScore(path);
}

static int Part2(std::string_view text)
{
    Map m;
    m.Parse(text);
    auto const path = m.Dijkstra({0, 0}, {m.width * 5 - 1, m.height * 5 - 1});
    return m.GetRiskScore(path);
}

int main()
{
    // https://adventofcode.com/2021/day/15
    fmt::print("Day 15, 2021: Chiton\n");

    Assert(40 == Part1(example::map));
    Assert(315 == Part2(example::map));

    auto const part1 = Part1(input::map);
    fmt::print("  Part 1: {}\n", part1);
    Assert(447 == part1);

    auto const part2 = Part2(input::map);
    fmt::print("  Part 2: {}\n", part2);
    Assert(2825 == part2);
}
