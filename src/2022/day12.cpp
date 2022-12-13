#include "day12.hpp"
#include "../common.hpp"

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
    Point2d start;
    Point2d target;

    void Parse(std::string_view mapText)
    {
        int y = 0;
        
        for (auto line : Split(mapText, '\n'))
        {
            int x = 0;
            
            for (char c : line)
            {
                Point2d p{x++, y};

                if (c == 'S')
                {
                    start = p;
                    c = 'a';
                }
                else if (c == 'E')
                {
                    target = p;
                    c = 'z';
                }

                map[p] = c - 'a';
            }

            ++y;
        }
    }

    static constexpr int kInvalidHeight = -1;
    
    int GetHeight(Point2d p) const
    {
        if (auto it = map.find(p); it != end(map))
        {
            return it->second;
        }

        return kInvalidHeight;
    }

    static bool InRange(int from, int to)
    {
        if (from == kInvalidHeight || to == kInvalidHeight)
            return false;

        int const distance = to - from;
        return distance <= 1;
    }

    std::vector<Point2d> Neighbors(Point2d p) const
    {
        std::vector<Point2d> neigh;
        neigh.reserve(4);

        auto const h = GetHeight(p);
        auto const east = p + Point2d::EAST;
        auto const south = p + Point2d::SOUTH;
        auto const west = p + Point2d::WEST;
        auto const north = p + Point2d::NORTH;

        if (InRange(h, GetHeight(east)))
        {
            neigh.push_back(east);
        }

        if (InRange(h, GetHeight(south)))
        {
            neigh.push_back(south);
        }

        if (InRange(h, GetHeight(west)))
        {
            neigh.push_back(west);
        }

        if (InRange(h, GetHeight(north)))
        {
            neigh.push_back(north);
        }

        return neigh;
    }

    std::vector<Point2d> Dijkstra() const
    {
        static constexpr auto limit = std::numeric_limits<int>::max();
        DictWithDefault<Point2d, int> dist{limit};
        std::unordered_map<Point2d, Point2d> prev;
        std::vector<std::tuple<Point2d, int>> queue;

        auto updatePriority = [&queue](Point2d const &p, int c)
        {
            std::erase_if(queue,
                [&p](auto const &t)
                {
                    return get<Point2d>(t) == p;
                });

            auto t = std::make_tuple(p, c);
            auto iter = std::lower_bound(begin(queue), end(queue), t,
                [](auto const &a, auto const &b)
                {
                    return std::get<int>(b) < std::get<int>(a);
                });

            queue.insert(iter, t);
        };

        queue.reserve(map.size());

        for (auto const &[p,_] : map)
        {
            if (p == start)
                continue;

            queue.push_back(std::make_tuple(p, limit));
        }

        dist.Set(start, 0);
        queue.push_back(std::make_tuple(start, 0));

        while (not queue.empty())
        {
            auto const &[current, d] = queue.back();
            queue.pop_back();

            if (current == target)
            {
                std::vector<Point2d> path;
                path.push_back(target);

                for (auto iter = prev.find(target); iter != end(prev);)
                {
                    path.push_back(iter->second);
                    iter = prev.find(iter->second);
                }

                return path;
            }

            for (auto neighbor : Neighbors(current))
            {
                if (auto const alt = dist.Get(current) + current.Distance(neighbor); alt < dist.Get(neighbor))
                {
                    dist.Set(neighbor, alt);
                    prev[neighbor] = current;
                    updatePriority(neighbor, alt);
                }
            }
        }

        return {};
    }

    auto Solve()
    {
        auto const path = Dijkstra();
        return path.size() - 1;
    }
};

static auto Solve(std::string_view mapText)
{
    Map map;
    map.Parse(mapText);
    return map.Solve();
}

static auto Part1()
{
    return Solve(input::map);
}

static auto Part2()
{
    return 0;
}

int main()
{
    // https://adventofcode.com/2022/day/12
    fmt::print("Day 12, 2022: Hill Climbing Algorithm\n");

    Assert(31 == Solve(example::map));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(412 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
