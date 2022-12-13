#pragma once
#include "defaultdict.hpp"
#include <type_traits>

template <class Map, class WeightFunction, class ValidNeighborFunction, class GoalCheckFunction>
class Dijkstra
{
    Map const *map_;
    WeightFunction calcWeight_;
    ValidNeighborFunction validNeighbor_;
    GoalCheckFunction isGoal_;
    using Weight = std::invoke_result_t<WeightFunction, Point2d, Point2d>;

private:
    static void UpdatePriority(std::vector<std::tuple<Point2d, Weight>> &queue, Point2d p, Weight c)
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
                return std::get<Weight>(b) < std::get<Weight>(a);
            });

        queue.insert(iter, t);
    }

public:
    Dijkstra(Map const &map, WeightFunction &&weightFn, ValidNeighborFunction &&validNeighborFn,
        GoalCheckFunction &&goalCheckFn)
        : map_{std::addressof(map)}
        , calcWeight_{std::move(weightFn)}
        , validNeighbor_{std::move(validNeighborFn)}
        , isGoal_{std::move(goalCheckFn)}
    {
    }

    std::vector<Point2d> Solve(Point2d start)
    {
        static constexpr auto limit = std::numeric_limits<Weight>::max();
        DictWithDefault<Point2d, Weight> dist{limit};
        std::unordered_map<Point2d, Point2d> prev;
        std::vector<std::tuple<Point2d, Weight>> queue;

        queue.reserve(map_->size());

        for (auto const &[p, _] : *map_)
        {
            if (p == start)
                continue;

            queue.push_back(std::make_tuple(p, limit));
        }

        dist.Set(start, 0);
        queue.push_back(std::make_tuple(start, Weight{}));

        while (not queue.empty())
        {
            auto const &[current, d] = queue.back();
            queue.pop_back();

            if (isGoal_(current))
            {
                std::vector<Point2d> path;

                for (auto iter = prev.find(current); iter != end(prev);)
                {
                    path.push_back(iter->second);
                    iter = prev.find(iter->second);
                }

                return path;
            }

            std::array const neighbors{
                current + Point2d::EAST, current + Point2d::SOUTH, current + Point2d::WEST, current + Point2d::NORTH};

            for (auto neighbor : neighbors)
            {
                if (not validNeighbor_(current, neighbor))
                    continue;

                if (auto const alt = dist.Get(current) + calcWeight_(current, neighbor); alt < dist.Get(neighbor))
                {
                    dist.Set(neighbor, alt);
                    prev[neighbor] = current;
                    UpdatePriority(queue, neighbor, alt);
                }
            }
        }

        return {};
    }
};
