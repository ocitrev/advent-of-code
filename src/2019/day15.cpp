#include "day15.hpp"

#include "../common.hpp"
#include "../common/intcode.hpp"

#include <chrono>
#include <optional>
#include <set>
#include <thread>

static constexpr std::string_view kWall{"█"};
static constexpr std::string_view kFloor{"."};
static constexpr std::string_view kDroid{"@"};
static constexpr std::string_view kOxygen{"O"};
static constexpr std::string_view kStart{"S"};

struct Robot
{
    Intcode cpu;
    Point2d pos;
    std::unordered_map<Point2d, std::string_view> grid;
    static constexpr std::array<Point2d, 4> dirs{{{0, -1}, {0, +1}, {-1, 0}, {+1, 0}}};
    std::optional<Point2d> oxygen;

    Robot()
        : cpu(ParseInputNumbers<Int, ','>())
    {
    }

    [[nodiscard]] auto Backup()
    {
        return std::make_tuple(cpu.Backup(), pos);
    }

    void Restore(std::tuple<Intcode::State, Point2d> const &backup)
    {
        grid[pos] = kFloor;
        pos = std::get<Point2d>(backup);
        grid[pos] = kDroid;
        cpu.Restore(std::get<Intcode::State>(backup));
    }

    [[nodiscard]] Point2d GetNextPos(int dir) const
    {
        return pos + dirs.at(static_cast<std::size_t>(dir));
    }

    Int Move(int dir)
    {
        auto r = cpu.RunUntilOuput(
            [dir]()
            {
                return dir + 1;
            });

        Assert(r.has_value());
        Point2d p = GetNextPos(dir);

        if (*r != 0)
        {
            grid[pos] = kFloor;
            grid[p] = kDroid;
            pos = p;
        }
        else
        {
            grid[p] = kWall;
        }

        return *r;
    }

    [[nodiscard]] std::pair<Point2d, Point2d> GetBounds() const
    {
        std::pair<Point2d, Point2d> bounds{{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()},
            {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()}};

        for (auto const &elem : grid)
        {
            bounds.first.x = std::min(bounds.first.x, elem.first.x);
            bounds.first.y = std::min(bounds.first.y, elem.first.y);
            bounds.second.x = std::max(bounds.second.x, elem.first.x);
            bounds.second.y = std::max(bounds.second.y, elem.first.y);
        }

        return bounds;
    }

    void PrintFrame() const
    {
        auto const bounds = GetBounds();
        std::string frame;

        for (int y = bounds.first.y; y <= bounds.second.y; ++y)
        {
            for (int x = bounds.first.x; x <= bounds.second.x; ++x)
            {
                frame.append(GetTile({x, y}));
            }

            frame.append("\n");
        }

        std::print("{}", frame);
    }

    void Print(bool isTerminal)
    {
        if (isTerminal)
        {
            std::print(CSI("?1049h"));
        }

        if (GetTile({0, 0}) != kDroid)
        {
            grid[{0, 0}] = kStart;
        }

        PrintFrame();

        if (isTerminal)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5s);
            std::print(CSI("?1049l"));
        }
    }

    [[nodiscard]] std::string_view GetTile(Point2d p) const
    {
        auto iter = grid.find(p);
        if (iter != end(grid))
        {
            return iter->second;
        }

        return {" "};
    }

    [[nodiscard]] std::vector<int> GetChoices() const
    {
        std::vector<int> choices;

        for (int i = 0; i != 4; ++i)
        {
            if (grid.find(GetNextPos(i)) == end(grid))
            {
                choices.push_back(i);
            }
        }

        return choices;
    }

    [[nodiscard]] std::vector<int> LookAround()
    {
        constexpr std::array inverse{1, 0, 3, 2};
        std::vector<int> avail;

        for (auto c : GetChoices())
        {
            if (0 != Move(c))
            {
                avail.push_back(c);
                Move(inverse.at(static_cast<std::size_t>(c)));
            }
        }

        return avail;
    }

    void ExploreAll(bool root = true)
    {
        for (auto p : LookAround())
        {
            auto backup = Backup();

            if (Move(p) == 2)
            {
                oxygen = pos;
            }
            ExploreAll(false);
            Restore(backup);
        }

        if (root)
        {
            Assert(oxygen.has_value());
            grid[*oxygen] = kOxygen;
        }
    }

    bool RecursiveSolve(Point2d p, std::set<Point2d> &visited, std::vector<Point2d> &path,
        std::pair<Point2d, Point2d> const &bounds) const
    {
        if (p == *oxygen)
        {
            path.push_back(p);
            return true;
        }

        if (GetTile(p) == kWall || visited.find(p) != end(visited))
        {
            return false;
        }

        visited.insert(p);
        auto const &[min, max] = bounds;

        if (p.x != min.x)
        {
            if (RecursiveSolve({p.x - 1, p.y}, visited, path, bounds))
            {
                path.push_back(p);
                return true;
            }
        }

        if (p.x != max.x)
        {
            if (RecursiveSolve({p.x + 1, p.y}, visited, path, bounds))
            {
                path.push_back(p);
                return true;
            }
        }

        if (p.y != min.y)
        {
            if (RecursiveSolve({p.x, p.y - 1}, visited, path, bounds))
            {
                path.push_back(p);
                return true;
            }
        }

        if (p.y != max.y)
        {
            if (RecursiveSolve({p.x, p.y + 1}, visited, path, bounds))
            {
                path.push_back(p);
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] std::vector<Point2d> Solve() const
    {
        std::set<Point2d> visited;
        std::vector<Point2d> path;
        [[maybe_unused]] bool const solved = RecursiveSolve({0, 0}, visited, path, GetBounds());
        Assert(solved);
        return path;
    }

    [[nodiscard]] bool IsFilling() const
    {
        return std::any_of(begin(grid), end(grid),
            [](auto const &elem)
            {
                return elem.second == kFloor;
            });
    }

    std::vector<Point2d> FindCandidates()
    {
        std::vector<Point2d> candidates;

        for (auto const &cell : grid)
        {
            if (cell.second != kOxygen)
            {
                continue;
            }

            for (std::size_t d = 0; d != 4; ++d)
            {
                auto p = cell.first + dirs.at(d);

                if (GetTile(p) == kFloor)
                {
                    candidates.push_back(p);
                }
            }
        }

        return candidates;
    }

    int FillOxygen()
    {
        int count = 0;

        while (IsFilling())
        {
            ++count;

            for (auto c : FindCandidates())
            {
                grid[c] = kOxygen;
            }
        }

        return count;
    }
};

int main()
{
    [[maybe_unused]] bool const isTerminal = IsTerminal(stdout);

    std::print("Day 15, 2019: Oxygen System\n");

    Robot r;
    r.ExploreAll();

    // r.Print();

    auto const commands = r.Solve();
    std::print("  Part1: {} commands\n", commands.size() - 1);
    Assert(254 == commands.size() - 1);

    auto const part2 = r.FillOxygen();
    std::print("  Part2: {} minutes\n", part2);
    Assert(268 == part2);
}
